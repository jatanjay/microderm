
/************************************************************************/
/* PWM_MOTOR_CONTROL :: 3 - SPEED, with system 10ms TC				    */
/* auth: jatan pandya												    */
/* date: 6/28/2024														*/
/************************************************************************/

#include <asf.h>
#include "conf_example.h"
#include "led_driver.h"
#include "pwm_led.h"

struct tc_module system_timer_instance; // instance for system timer counter (TC1)
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
#define SWITCH_OFF_PIN													PIN_PA07
#define CHARGN_ON_PIN													PIN_PA11
#define CHARGN_OFF_PIN													PIN_PA03
#define BUTTON_2														PIN_PA15
#define BUTTON_1														SW0_PIN



#define DELAY_PRESS_CN													400
#define DELAY_DEBOUNCE_CN												2



static uint8_t motor_toggle_count = 0;
static uint8_t pwm_led_toggle_count = 0;

static bool BUTTON_ONE_PRESS_STATUS = false;
static bool BUTTON_ONE_RELEASE_STATUS = false;

static bool BUTTON_TWO_PRESS_STATUS = false;
static bool BUTTON_TWO_RELEASE_STATUS = false;

static bool PWM_RUNNING = false;

static bool VBUS_STATE;
static bool CHARGN_ON_STATE;
static bool CHARGN_OFF_STATE;

static bool BATTERY_CHARGING;
static bool BATTERY_CHARGED;
static bool BATTERY_LOW;
static bool BATTERY_LOWEST;
static bool PULSATING_MOTOR_ROUTINE = false;



static bool SYS_TICK_10MS;
static bool SYS_TICK_50MS;
static bool SYS_TICK_100MS;
static bool SYS_TICK_200MS;
static bool LongPressB1Flag = false;
static bool LongPressB2Flag = false;

static int press_B1_delay_count = DELAY_DEBOUNCE_CN;
static int press_B2_delay_count = DELAY_DEBOUNCE_CN;
static int long_press_B1_delay_count = DELAY_PRESS_CN;
static int long_press_B2_delay_count = DELAY_PRESS_CN;







 void configure_pwm_generator (void);

 void regular_routine (void);
 void cycle_pwm_motor (void);
 void cycle_pwm_led(void);
 void pwm_motor_cleanup(void);

 bool is_button_one_pressed(void);
 bool is_button_two_pressed(void);
































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
	
	
	
	/*
	
	shutdown routine -- turn PA07 low to turn off peripherals
	
	*/
	
	config_port_pin.direction  = PORT_PIN_DIR_OUTPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_UP;							// START AT PULL UP.
	port_pin_set_config(SWITCH_OFF_PIN, &config_port_pin);
	
	
	/*
	
	BUTTON 2
	
	*/
	
	config_port_pin.direction  = PORT_PIN_DIR_INPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_DOWN;							// START AT PULL DOWN.
	port_pin_set_config(BUTTON_2, &config_port_pin);
	
	
	

	/*
	GET_CHARGING_STATUS
	*/
	
	config_port_pin.direction  = PORT_PIN_DIR_INPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_DOWN;							// START AT PULL DOWN.
	port_pin_set_config(CHARGN_ON_PIN, &config_port_pin);
	
	
	/*
	GET_CHARGING_OFF_STATUS
	*/
	
	config_port_pin.direction  = PORT_PIN_DIR_OUTPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_DOWN;							// START AT PULL DOWN.
	port_pin_set_config(CHARGN_OFF_PIN, &config_port_pin);
	
	
	
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
	config_tc.pwm_channel[TC_COMPARE_CAPTURE_CHANNEL_0].pin_out = PIN_PA16E_TC1_WO0; // PA15 FOR TESTING ; OG PA16 (LED)
	config_tc.pwm_channel[TC_COMPARE_CAPTURE_CHANNEL_0].pin_mux = MUX_PA16E_TC1_WO0; // PA15 FOR TESTING ; OG PA16 (LED)
	
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


 void configure_pwm_generator (void)
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


 bool is_button_one_pressed (void)
{

	if (!port_pin_get_input_level (BUTTON_1))
	{
		BUTTON_ONE_PRESS_STATUS = true;
		press_B1_delay_count--;
		long_press_B1_delay_count--;
		
	}else
	{
		BUTTON_ONE_PRESS_STATUS = false;
		press_B1_delay_count = DELAY_DEBOUNCE_CN;
		long_press_B1_delay_count = DELAY_PRESS_CN;
		
	}
	
	// long press delay logic
	if (long_press_B1_delay_count <= 0){
		LongPressB1Flag = true;
		long_press_B1_delay_count = 0 ;
	}
	
	// debounce logic
	if (press_B1_delay_count <= 0)
	{
		BUTTON_ONE_RELEASE_STATUS = false;
		press_B1_delay_count = 0;
		return true;

	}
	else
	{
		BUTTON_ONE_RELEASE_STATUS = true;
		return false;
	}
}


 bool is_button_two_pressed (void)
{

	if (!port_pin_get_input_level (BUTTON_2))
	{
		BUTTON_TWO_PRESS_STATUS = true;
		press_B2_delay_count--;
		long_press_B2_delay_count--;
		
	}else
	{
		BUTTON_TWO_PRESS_STATUS = false;
		press_B2_delay_count = DELAY_DEBOUNCE_CN;
		long_press_B2_delay_count = DELAY_PRESS_CN;
		
	}
	
	// long press delay logic
	if (long_press_B2_delay_count <= 0){
		LongPressB2Flag = true;
		long_press_B2_delay_count = 0 ;
	}
	
	// debounce logic
	if (press_B2_delay_count <= 0)
	{
		BUTTON_TWO_RELEASE_STATUS = false;
		press_B2_delay_count = 0;
		return true;

	}
	else
	{
		BUTTON_TWO_RELEASE_STATUS = true;
		return false;
	}
}


void pwm_motor_cleanup(void){
	PULSATING_MOTOR_ROUTINE = false;
	motor_toggle_count = 0;
	tc_set_compare_value (&pwm_generator_instance,
	TC_COMPARE_CAPTURE_CHANNEL_0,
	INITIAL_DUTY_CYCLE);
	PWM_RUNNING = false;
	tc_disable (&pwm_generator_instance);
	port_pin_set_output_level(MOTOR_NSLEEP_PIN,LOW);
}


void cycle_pwm_led(void) {
	switch (pwm_led_toggle_count) {
		case 1:
		set_pwm_red();
		set_color_red();
		break;
		case 2:
		set_pwm_green();
		set_color_green();
		break;
		case 3:
		set_pwm_blue();
		set_color_blue();
		break;
		case 4:
		set_pwm_yellow();
		set_color_yellow();
		break;
		case 5:
		set_pwm_purple();
		set_color_purple();
		break;
		case 6:
		set_pwm_cyan();
		set_color_cyan();
		break;
		case 7:
		set_pwm_white();
		set_color_white();
		break;
		default:
		case 8:
		turn_off_all();							// Reset to 1 for red
		pwm_led_toggle_count = 0;
		break;
	}
}


void cycle_pwm_motor (void)
{
	{
		if (PWM_RUNNING)
		{
			
			if (motor_toggle_count == 2)
			{
				tc_set_compare_value (&pwm_generator_instance,
				TC_COMPARE_CAPTURE_CHANNEL_0, FIRST_DUTY_CYCLE);


			}
			else if (motor_toggle_count == 3)
			{
				tc_set_compare_value (&pwm_generator_instance,
				TC_COMPARE_CAPTURE_CHANNEL_0,
				SECOND_DUTY_CYCLE);

			}
			
			else if (motor_toggle_count == 4)
			{

				PULSATING_MOTOR_ROUTINE = true;
				tc_set_compare_value (&pwm_generator_instance,
				TC_COMPARE_CAPTURE_CHANNEL_0,
				SECOND_DUTY_CYCLE);

			}
			
			else if (motor_toggle_count > 4)
			{
				pwm_motor_cleanup();
			}
		}
	}

}


void regular_routine(void) {
	static bool motor_status_changed = false;
	static bool led_button_status_changed = false;

	//-------------------------------------------------------------

	if (is_button_one_pressed()) {
		if (LongPressB1Flag) {
			pwm_motor_cleanup();
			} else {
			if (!motor_status_changed) {
				motor_toggle_count++;
				motor_status_changed = true;
				if (!PWM_RUNNING) {
					PWM_RUNNING = true;
					tc_enable( & pwm_generator_instance);
					port_pin_set_output_level(MOTOR_NSLEEP_PIN, HIGH);
					} else {
					cycle_pwm_motor();
				}
			}
		}
	}
		
	if (BUTTON_ONE_RELEASE_STATUS) {
		motor_status_changed = false;
	}

	//-------------------------------------------------------

	if (is_button_two_pressed()) {
		if (LongPressB2Flag) {
			pwm_motor_cleanup();
			} else {
			if (!led_button_status_changed) {
				pwm_led_toggle_count++;
				led_button_status_changed = true;
				cycle_pwm_led();
			}
		}
	}
	if (BUTTON_TWO_RELEASE_STATUS) {
		led_button_status_changed = false;
	}
}

//------------------------------------------------------------------




/************************************************************************/
/* GET SYSTEM STATES			                                        */
/************************************************************************/


 void get_vbus_state(void);

 void get_vbus_state(void){
	VBUS_STATE = port_pin_get_input_level(VBUS_PIN);
}



 void get_charging_on_status_state(void);

 void get_charging_on_status_state(void){
	CHARGN_ON_STATE = port_pin_get_input_level(CHARGN_ON_PIN);
}



 void get_charging_off_status_state(void);

 void get_charging_off_status_state(void){
	CHARGN_OFF_STATE = port_pin_get_input_level(CHARGN_OFF_PIN);
}


 void update_battery_states(void);

 void update_battery_states(void){
	
	/*
	Update Global Var based on state (using VBUS & CHRGHN)
	*/
	
	get_vbus_state();
	get_charging_on_status_state();
	
	
	
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
		set_battery_low_routine();
	}
	else if (BATTERY_LOW){
		set_color_red();
	}	
	else if (BATTERY_CHARGED){
		set_color_green();
	}	
	else if (BATTERY_CHARGING){
		//set_battery_charge_routine();
	}
	
}


 void toggle_nsleep(void);

 void toggle_nsleep(void){
	static bool PULSATING_MOTOR = false;
	if (PULSATING_MOTOR_ROUTINE){
		if (PULSATING_MOTOR){
			port_pin_set_output_level(MOTOR_NSLEEP_PIN,LOW);
			PULSATING_MOTOR = false;
			}else{
			PULSATING_MOTOR = true;
			port_pin_set_output_level(MOTOR_NSLEEP_PIN,HIGH);
		}
	}
}


/************************************************************************/
/* STATE MACHINE		                                                */
/************************************************************************/

 void system_state(void);

 void system_state(void){
	update_battery_states();
	display_battery_state();
}


/************************************************************************/
/* LOGIC MACHINE		                                                */
/************************************************************************/

 void system_logic(void);

 void system_logic(void){
	if (!VBUS_STATE){
		configure_pwm_generator();					// Enable Motor PWM
	}
	else{				
		if (!CHARGN_ON_STATE){
			BATTERY_CHARGING = true;
			BATTERY_CHARGED = false;
				
		}
		else{
			BATTERY_CHARGING = false;
			BATTERY_CHARGED = true;
		}		
	}
	

	
	
	// get low , and lowest logic
	
	//if (VBUS_STATE && CHARGN_ON_STATE){
		//BATTERY_CHARGING = true;
	//}
	//
		//
	//if (VBUS_STATE && CHARGN_ON_STATE){
		//BATTERY_CHARGING = true;
	//}
	//
	//
	
	
	
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
	port_pin_set_output_level(SWITCH_OFF_PIN,LOW);
	port_pin_set_output_level(BUTTON_2,LOW);
	port_pin_set_output_level(CHARGN_ON_PIN,LOW);
	port_pin_set_output_level(CHARGN_OFF_PIN,HIGH);
	
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
	configure_pwm_tcc();
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





