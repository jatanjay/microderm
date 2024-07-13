
/************************************************************************/
/* PWM_MOTOR_CONTROL :: 3 - SPEED, with system 10ms TC				    */
/* auth: jatan pandya												    */
/* date: 6/28/2024														*/
/************************************************************************/

#include <asf.h>
#include "conf_example.h"
#include "string.h"
#include "led_driver.h"

struct tc_module
system_timer_instance; // instance for system timer counter (TC1)
struct tc_module pwm_generator_instance; // instance for PWM Motor Control (TC0)

#define SYSTEM_TC														TC1
#define PWM_GENERATOR													TC2

#define TC_COUNTER_SIZE													TC_COUNTER_SIZE_8BIT
#define TC_CLOCK_PRESCALER 											    TC_CLOCK_PRESCALER_DIV64
#define TC_CLOCK_SOURCE  											    GCLK_GENERATOR_0

#define SYSTEM_TC_PERIOD_VALUE  										111												// Tuned for 1.99 -- 2.002 ms (1ms * 2) Period

#define PWM_PIN_OUT  													PIN_PA10E_TC2_WO0;
#define PWM_MUX_OUT  													MUX_PA10E_TC2_WO0;

#define PWM_PERIOD_VALUE  											    85
#define INITIAL_DUTY_CYCLE  											((int)lround (PWM_PERIOD_VALUE / 4.0))
#define FIRST_DUTY_CYCLE  											    ((int)lround (PWM_PERIOD_VALUE / 2.5))
#define SECOND_DUTY_CYCLE  											    ((int)lround (PWM_PERIOD_VALUE / 1.10))
#define MAX_TOGGLE_COUNT  											    3

#define VBUS_PIN														PIN_PA27
#define MOTOR_NSLEEP_PIN												PIN_PA06

#define DELAY_PRESS_CN													400
#define DELAY_DEBOUNCE_CN												2



static uint8_t toggle_count = 0;
static bool BUTTON_PRESS_STATUS = false;
static bool BUTTON_RELEASE_STATUS = false;
static bool PWM_RUNNING = false;

static bool VBUS_STATE;

static bool BATTERY_CHARGING;
static bool BATTERY_CHARGED;
static bool BATTERY_LOW;
static bool BATTERY_LOWEST;
static bool PULSATING_MOTOR_ROUTINE = false;



static bool SYS_TICK_10MS;
static bool SYS_TICK_50MS;
static bool SYS_TICK_100MS;
static bool SYS_TICK_200MS;
static bool LongPressFlag = false;

static int press_delay_count = DELAY_DEBOUNCE_CN;
static int long_press_delay_count = DELAY_PRESS_CN;



/*
shutdown routine -- turn PA07 low to turn off peripherals
*/



static void configure_pwm_generator (void);
void regular_routine (void);
void cycle_pwm_duty (void);
void turn_led_on (void);
bool is_button_pressed (void);





































/************************************************************************/
/* GPIO - PIN SETUP														*/
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
	
	MOTOR'S NSLEEP PIN
	
	*/
	
	config_port_pin.direction  = PORT_PIN_DIR_OUTPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_DOWN;						// START AT PULL DOWN.
	port_pin_set_config(MOTOR_NSLEEP_PIN, &config_port_pin);
	
}



/************************************************************************/
/* TC - TIMER CODE START												*/
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
	static int tick_count_200ms;
	
	
	tick_count_1ms++;
	
	
	//port_pin_toggle_output_level (LED0_PIN);					// visually check sys clock on PA16
	
	// Check for 10ms interval
	if (tick_count_1ms >= 10)
	{
		tick_count_10ms++;
		tick_count_1ms = 0;
		SYS_TICK_10MS = true;									// Flag for 10ms interval
		//port_pin_toggle_output_level (LED0_PIN);				// visually check sys clock on PA16
	}
	
	// Check for 50ms interval
	if (tick_count_10ms >= 5)
	{
		tick_count_50ms++;
		tick_count_10ms = 0;
		SYS_TICK_50MS = true;									// Flag for 50ms interval
		//port_pin_toggle_output_level (LED0_PIN);				// visually check sys clock on PA16
	}
	
	// Check for 100ms interval
	if (tick_count_50ms >= 2)
	{
		tick_count_100ms++;
		tick_count_50ms = 0;
		SYS_TICK_100MS = true;									// Flag for 100ms interval
		//port_pin_toggle_output_level (LED0_PIN);				// visually check sys clock on PA16
	}
	
	
	// Check for 200ms interval
	if (tick_count_100ms >= 2)
	{
		tick_count_200ms++;
		tick_count_100ms = 0;
		SYS_TICK_200MS = true;									// Flag for 200ms interval
		//port_pin_toggle_output_level (LED0_PIN);				// visually check sys clock on PA16
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
	
	config_tc.pwm_channel[TC_COMPARE_CAPTURE_CHANNEL_0].enabled = true;
	config_tc.pwm_channel[TC_COMPARE_CAPTURE_CHANNEL_0].pin_out = PIN_PA15E_TC1_WO1; // PA15 FOR TESTING
	config_tc.pwm_channel[TC_COMPARE_CAPTURE_CHANNEL_0].pin_mux = MUX_PA15E_TC1_WO1; // PA15 FOR TESTING
	
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
	config_tc.pwm_channel[0].pin_out = PWM_PIN_OUT;
	config_tc.pwm_channel[0].pin_mux = PWM_MUX_OUT;

	config_tc.pwm_channel[0].enabled = true;
	
	tc_init (&pwm_generator_instance, PWM_GENERATOR, &config_tc);

	
}




/*
comment is_button_pressed for pwm led routine - 7/9


*/


bool is_button_pressed (void)
{

	if (!port_pin_get_input_level (SW0_PIN))
	{
		BUTTON_PRESS_STATUS = true;
		press_delay_count--;
		long_press_delay_count--;
		
	}else
	{
		BUTTON_PRESS_STATUS = false;
		press_delay_count = DELAY_DEBOUNCE_CN;
		long_press_delay_count = DELAY_PRESS_CN;
		
	}
	
	// long press delay logic
	if (long_press_delay_count <= 0){
		LongPressFlag = true;
		long_press_delay_count = 0 ;
	}
	
	// debounce logic
	if (press_delay_count <= 0)
	{
		BUTTON_RELEASE_STATUS = false;
		press_delay_count = 0;
		return true;

	}
	else
	{
		BUTTON_RELEASE_STATUS = true;
		return false;
	}
}





void pwm_motor_cleanup(void);

void pwm_motor_cleanup(void){
	PULSATING_MOTOR_ROUTINE = false;
	toggle_count = 0;
	tc_set_compare_value (&pwm_generator_instance,
	TC_COMPARE_CAPTURE_CHANNEL_0,
	INITIAL_DUTY_CYCLE);
	PWM_RUNNING = false;
	tc_disable (&pwm_generator_instance);
	port_pin_set_output_level(MOTOR_NSLEEP_PIN,LOW);
}


void regular_routine (void)
{
	static bool motor_status_changed = false;
	
	if (is_button_pressed()){
		if (LongPressFlag){
			
			LED_On(LED0_PIN);
			pwm_motor_cleanup();
			
			/*
			DO 2 second shutdown routine
			*/
			
			} else {
			
			// routine for motor (regular)
			if (!motor_status_changed)
			{
				toggle_count++;
				motor_status_changed = true;
				if (!PWM_RUNNING)
				{
					PWM_RUNNING = true;
					tc_enable (&pwm_generator_instance);
					port_pin_set_output_level(MOTOR_NSLEEP_PIN,HIGH);
				}
				else
				{
					cycle_pwm_duty ();
				}
			}
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
		
		else if (toggle_count == 4)
		{
			PULSATING_MOTOR_ROUTINE = true;
			tc_set_compare_value (&pwm_generator_instance,
			TC_COMPARE_CAPTURE_CHANNEL_0,
			SECOND_DUTY_CYCLE);
		}
		
		else if (toggle_count > 4)
		{
			pwm_motor_cleanup();
		}
	}
}




/************************************************************************/
/* GET SYSTEM STATES			                                        */
/************************************************************************/


void get_vbus_state(void);

void get_vbus_state(void){
	VBUS_STATE = port_pin_get_input_level(VBUS_PIN);
}



void update_battery_states(void);

void update_battery_states(void){
	
	/*
	Update Global Var based on state (using VBUS & CHRGHN)
	*/
	
}


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
	
	a. level_lowest				: set_battery_low_routine();
	a. level_low				: set_color_red();
	a. level_charging			: set_battery_charge_routine();
	a. level_charged			: set_color_green();
	*/
	
	
	if (BATTERY_LOWEST){
		//set_battery_low_routine();
	}
	
	if (BATTERY_LOW){
	}
	
	if (BATTERY_CHARGED){
	}
	
	if (BATTERY_CHARGING){
		//set_battery_charge_routine();
	}
	
}


void toggle_nsleep(void);

void toggle_nsleep(void){
	static bool PULSATING_MOTOR = false;
	if (PULSATING_MOTOR_ROUTINE){
		if (PULSATING_MOTOR){
			port_pin_set_output_level(MOTOR_NSLEEP_PIN,LOW);
			LED_Off(LED0_PIN);
			PULSATING_MOTOR = false;
			}else{
			PULSATING_MOTOR = true;
			port_pin_set_output_level(MOTOR_NSLEEP_PIN,HIGH);
			LED_On(LED0_PIN);
		}
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
		regular_routine();
	}
	
	if (SYS_TICK_200MS){
		SYS_TICK_200MS = false;
		toggle_nsleep();
	}


}


/************************************************************************/
/* Start Up Configurations                                              */
/************************************************************************/

void startup_default_pin_state(void);

void startup_default_pin_state(void){
	port_pin_set_output_level(MOTOR_NSLEEP_PIN,LOW);
}



void startup_sys_configs(void);


void startup_sys_configs(void){
	system_init ();									// System Initialize
	system_interrupt_enable_global ();				// System Interrupts
	configure_port_pins ();							// System PORTs
	startup_default_pin_state();
	configure_system_tc ();							// System Clock
	system_tc_callbacks ();							// System Clock Callback
	i2c_master_setup();
	//reset_chip();
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





/************************************************************************/
/* TCC LED CHANLE TEST CODE                                                                     */
/************************************************************************/


/*

 Define the TCC channel numbers corresponding to the LED colors
#define RED_CHANNEL    TCC_CHANNEL_NUM_0
#define GREEN_CHANNEL  TCC_CHANNEL_NUM_1
#define BLUE_CHANNEL   TCC_CHANNEL_NUM_2
#define WHITE_CHANNEL  TCC_CHANNEL_NUM_5

 Function to turn on Red LED
void turn_on_red(void) {
	 Enable PWM output on Red channel (TCC_CHANNEL_NUM_0)
	tcc_enable_pwm_output(&config_tcc, RED_CHANNEL);
}

 Function to turn on Green LED
void turn_on_green(void) {
	 Enable PWM output on Green channel (TCC_CHANNEL_NUM_1)
	tcc_enable_pwm_output(&config_tcc, GREEN_CHANNEL);
}

 Function to turn on Blue LED
void turn_on_blue(void) {
	 Enable PWM output on Blue channel (TCC_CHANNEL_NUM_2)
	tcc_enable_pwm_output(&config_tcc, BLUE_CHANNEL);
}

 Function to turn on White LED
void turn_on_white(void) {
	 Enable PWM output on White channel (TCC_CHANNEL_NUM_5)
	tcc_enable_pwm_output(&config_tcc, WHITE_CHANNEL);
}

 Function to turn off all LEDs
void turn_off_all(void) {
	 Disable PWM outputs on all channels
	tcc_disable_pwm_output(&config_tcc, RED_CHANNEL);
	tcc_disable_pwm_output(&config_tcc, GREEN_CHANNEL);
	tcc_disable_pwm_output(&config_tcc, BLUE_CHANNEL);
	tcc_disable_pwm_output(&config_tcc, WHITE_CHANNEL);
}

 Function to create specific colors
void create_color(int color) {
	 Color combinations based on the algorithm
	switch (color) {
		case 0:  // Red
		turn_on_red();
		break;
		case 1:  // Green
		turn_on_green();
		break;
		case 2:  // Blue
		turn_on_blue();
		break;
		case 3:  // White
		turn_on_white();
		break;
		case 4:  // Yellow (Red + Green)
		turn_on_red();
		turn_on_green();
		break;
		case 5:  // Purple (Red + Blue)
		turn_on_red();
		turn_on_blue();
		break;
		case 6:  // Cyan (Green + Blue)
		turn_on_green();
		turn_on_blue();
		break;
		default:
		 Turn off all LEDs if an invalid color is selected
		turn_off_all();
		break;
	}
}


int main() {
	 Initialize TCC configuration (assuming config_tcc is globally defined)
	configure_tcc();

	 Create Yellow color (Red + Green)
	create_color(4);  // 4 corresponds to Yellow

	 Delay or perform other operations

	 Create Cyan color (Green + Blue)
	create_color(6);  // 6 corresponds to Cyan

	 Delay or perform other operations

	 Turn off all LEDs
	turn_off_all();

	return 0;
}


*/