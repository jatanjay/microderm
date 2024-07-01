
/************************************************************************/
/* PWM_MOTOR_CONTROL :: 3 - SPEED, with system 10ms TC				    */
/* auth: jatan pandya												    */
/* date: 6/28/2024														*/
/************************************************************************/

#include <asf.h>
#include "conf_example.h"
#include "string.h"

struct tc_module
system_timer_instance; // instance for system timer counter (TC1)
struct tc_module pwm_generator_instance; // instance for PWM Motor Control (TC0)

#define SYSTEM_TC														TC1
#define PWM_GENERATOR													TC2

#define TC_COUNTER_SIZE													TC_COUNTER_SIZE_8BIT
#define TC_CLOCK_PRESCALER 											    TC_CLOCK_PRESCALER_DIV64
#define TC_CLOCK_SOURCE  											    GCLK_GENERATOR_0

#define SYSTEM_TC_PERIOD_VALUE  										124

#define PWM_PIN_OUT  													PIN_PA10E_TC2_WO0;
#define PWM_MUX_OUT  													MUX_PA10E_TC2_WO0;

#define PWM_PERIOD_VALUE  											    85
#define INITIAL_DUTY_CYCLE  											((int)lround (PWM_PERIOD_VALUE / 4.0))
#define FIRST_DUTY_CYCLE  											    ((int)lround (PWM_PERIOD_VALUE / 2.5))
#define SECOND_DUTY_CYCLE  											    ((int)lround (PWM_PERIOD_VALUE / 1.10))
#define MAX_TOGGLE_COUNT  											    3

#define VBUS_PIN														PIN_PA27

uint8_t toggle_count = 0;
bool BUTTON_PRESS_STATUS = false;
bool BUTTON_RELEASE_STATUS = false;
bool PWM_RUNNING = false;
static bool SYS_TICK_10MS;
static bool SYS_TICK_50MS;
static bool SYS_TICK_100MS;


static void configure_pwm_generator (void);
void check_button_press (void);
void cycle_pwm_duty (void);
void turn_led_on (void);
bool is_button_pressed (void);





























/************************************************************************/
/* GPIO - PIN SETUP
*/
/************************************************************************/

void configure_port_pins(void);

void configure_port_pins(void)
{
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	
	/* PA27 is shared between EDBG
	SS and VBUS detection
	
	Usage:

	-Disable pull-up
	- check level on line to detect if VBUS is high and USB cable is present.
	
	*/
	
	config_port_pin.direction  = PORT_PIN_DIR_INPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_DOWN;
	port_pin_set_config(VBUS_PIN, &config_port_pin);
	
	/*
	
	Configure your own pins as PULL_UP OR PULL_DOWN
	
	*/
}















/************************************************************************/
/* TC - TIMER CODE START
*/
/************************************************************************/

void configure_system_tc (void);
void system_tc_callbacks (void);
void sys_tc_callback (struct tc_module *const module_inst);

void sys_tc_callback(struct tc_module *const module_inst)
{
	static int tick_count_1ms;
	static int tick_count_10ms;
	static int tick_count_50ms;
	static int tick_count_100ms;
	
	tick_count_1ms++;
	
	// Check for 10ms interval
	if (tick_count_1ms > 10)
	{
		tick_count_10ms++;
		tick_count_1ms = 0;
		SYS_TICK_10MS = true;  // Flag for 10ms interval
	}
	
	// Check for 50ms interval
	if (tick_count_10ms > 5)
	{
		tick_count_50ms++;
		tick_count_10ms = 0;
		SYS_TICK_50MS = true;  // Flag for 50ms interval
	}
	
	// Check for 100ms interval
	if (tick_count_50ms > 2)
	{
		tick_count_100ms++;
		tick_count_50ms = 0;
		SYS_TICK_100MS = true;  // Flag for 100ms interval
	}
	
	
}

void configure_system_tc (void)
{
	struct tc_config config_tc;
	tc_get_config_defaults (&config_tc);
	config_tc.clock_source = TC_CLOCK_SOURCE;
	config_tc.counter_size = TC_COUNTER_SIZE;
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER;
	config_tc.counter_8_bit.period = SYSTEM_TC_PERIOD_VALUE;
	tc_init (&system_timer_instance, SYSTEM_TC, &config_tc);
	tc_enable (&system_timer_instance);
}

void system_tc_callbacks (void)
{
	tc_register_callback (&system_timer_instance, sys_tc_callback,
	TC_CALLBACK_OVERFLOW);
	tc_enable_callback (&system_timer_instance, TC_CALLBACK_OVERFLOW);
}

/************************************************************************/
/* TC - SYSTEM TIMER													*/
/************************************************************************/


static void configure_pwm_generator (void)
{
	struct tc_config config_tc;
	tc_get_config_defaults (&config_tc);

	config_tc.clock_source = TC_CLOCK_SOURCE;
	config_tc.counter_size = TC_COUNTER_SIZE;
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER;

	config_tc.counter_8_bit.value = 0;
	config_tc.counter_8_bit.period = PWM_PERIOD_VALUE;

	config_tc.counter_8_bit.compare_capture_channel[0] = INITIAL_DUTY_CYCLE;

	config_tc.wave_generation = TC_WAVE_GENERATION_NORMAL_PWM;
	config_tc.pwm_channel[0].pin_out = PIN_PA10E_TC2_WO0;
	config_tc.pwm_channel[0].pin_mux = MUX_PA10E_TC2_WO0;

	config_tc.pwm_channel[0].enabled = true;

	tc_init (&pwm_generator_instance, PWM_GENERATOR, &config_tc);
}








/*
Test long button press
*/


bool is_button_pressed (void)
{
	static int press_delay_count = 5;

	
	if (!port_pin_get_input_level (SW0_PIN))
	{
		BUTTON_PRESS_STATUS = true;
		press_delay_count--;
		
	}else
	{
		BUTTON_PRESS_STATUS = false;
		press_delay_count = 5;
	}
	if (press_delay_count <= 0)
	{
		BUTTON_RELEASE_STATUS = false;
		return true;
		press_delay_count = 0;
	}
	else
	{
		BUTTON_RELEASE_STATUS = true;
		return false;
	}
	
}



bool LONG_BUTTON_PRESS;

void long_button_press(void){
	static int long_press = 165;
	if (!port_pin_get_input_level (SW0_PIN))
	{
		long_press--;
	}else
	{
		LONG_BUTTON_PRESS = false;
		long_press = 165;
	}
	if (long_press <= 0)
	{
		LONG_BUTTON_PRESS = true;
		
		long_press = 0;
	}
}



void check_button_press (void)
{
	static bool motor_status_changed = false;
	if (is_button_pressed () & !motor_status_changed)
	{
		toggle_count++;
		motor_status_changed = true;
		if (!PWM_RUNNING)
		{
			PWM_RUNNING = true;
			tc_enable (&pwm_generator_instance);
		}
		else
		{
			cycle_pwm_duty ();
		}
	}
	if (BUTTON_RELEASE_STATUS)
	{
		motor_status_changed = false;
	}
}

void cycle_pwm_duty (void)
{
	if (PWM_RUNNING)
	{
		if (toggle_count == 2)
		{
			tc_set_compare_value (&pwm_generator_instance,
			TC_COMPARE_CAPTURE_CHANNEL_0, FIRST_DUTY_CYCLE);
		}
		else if (toggle_count == 3)
		{
			tc_set_compare_value (&pwm_generator_instance,
			TC_COMPARE_CAPTURE_CHANNEL_0,
			SECOND_DUTY_CYCLE);
		}
		else if (toggle_count > 3)
		{
			toggle_count = 0;
			tc_set_compare_value (&pwm_generator_instance,
			TC_COMPARE_CAPTURE_CHANNEL_0,
			INITIAL_DUTY_CYCLE);
			PWM_RUNNING = false;
			tc_disable (&pwm_generator_instance);
		}
	}
}




/************************************************************************/
/* GET SYSTEM STATES			                                        */
/************************************************************************/


bool VBUS_STATE;
void get_vbus_state(void);

void get_vbus_state(void){
	VBUS_STATE = port_pin_get_input_level(VBUS_PIN);
}



bool BATTERY_CHARGING;
bool BATTERY_CHARGED;
void update_battery_states(void);

void update_battery_states(void){
	
	/*
	Update Global Var based on state (using VBUS & CHRGHN)
	*/
	
}


bool BATTERY_LOW;
bool BATTERY_LOWEST;
void sample_battery_states(void);


void sample_battery_states(void){

	/*
	
	When unplugged,
	Sample from ADC, spit out 1/0 based on math
	
	
	1. read from ADC
	2. do math to bring in Voltage (V)
	3. based on threshold, set global flags
	
	*/
}






















/************************************************************************/
/* Indication LED Control                                               */
/************************************************************************/


void led_control(const char* manner, const char* color);

void led_control(const char* manner, const char* color) {
	if (strcmp(manner, "blink") == 0 && strcmp(color, "yellow") == 0) {
		LED_On(LED0_PIN);
	}
}


/************************************************************************/
/* Indication LED Control                                               */
/************************************************************************/

void display_battery_state(void);

void display_battery_state(void){
	
	/*
	LED charging indicator light:
	
	1. blinking red when device is dead
	2. Steady red light when device has a low battery
	3. Blinking green light when device is charging
	4. Steady green light when the device is at least 100% charged.
	
	
	algorithm:
	
	1. Sense VBUS level
	2. Map to following states and call led_control()
	
	a. level_lowest				: led_control(blink,	red)		!! CHUCK FOR NOW
	a. level_low				: led_control(steady,	red)
	a. level_charging			: led_control(blink,	green)
	a. level_charged			: led_control(steady,	green)
	*/
	
	
	if (BATTERY_LOWEST){
		
	}
	
	if (BATTERY_LOW){
	}
	
	if (BATTERY_CHARGED){
		led_control("blink","yellow");
	}
	
	if (BATTERY_CHARGING){
		
	}
	
}





/************************************************************************/
/* STATE MACHINE		                                                */
/************************************************************************/

void system_state(void);

void system_state(void){
	get_vbus_state();
	update_battery_states();
	sample_battery_states();
}



/************************************************************************/
/* LOGIC MACHINE		                                                */
/************************************************************************/

void system_logic(void);

void system_logic(void){
	if (!VBUS_STATE){
		configure_pwm_generator();					// Enable Motor PWM
	}
	
	if (VBUS_STATE){
		// LED_On(LED0_PIN);						// Verify VBUS Connection
		display_battery_state();					// Display colors mapped to battery state
	}
	
	if (BATTERY_LOW){
		display_battery_state();					// Steady Red
	}
	
	if (BATTERY_LOWEST){
		display_battery_state();					// Blink Red
	}
	
	if (SYS_TICK_10MS){
		SYS_TICK_10MS = false;
		check_button_press ();						// Poll Button Press State
		long_button_press();
	}
	if (LONG_BUTTON_PRESS){
		LED_On(LED0_PIN);
	}
}


/************************************************************************/
/* Start Up Configurations                                              */
/************************************************************************/

void startup_sys_configs(void);


void startup_sys_configs(void){
	system_init ();									// System Initialize
	system_interrupt_enable_global ();				// System Interrupts
	configure_port_pins ();							// System PORTs
	configure_system_tc ();							// System Clock
	system_tc_callbacks ();							// System Clock Callback
}



int main (void)
{
	startup_sys_configs ();
	
	while (true)
	{
		system_state();								// Get latest system_state
		system_logic();								// Invoke System Logic
	}
}