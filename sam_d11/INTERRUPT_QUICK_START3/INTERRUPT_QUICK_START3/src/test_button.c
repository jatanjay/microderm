/*
 * test_button.c
 *
 * Created: 7/30/2024 1:29:38 PM
 *  Author: jatan
 */ 
 #include <asf.h>
 #include <tc.h>
 #include <math.h>


 void configure_extint_channel(void);
 void configure_extint_callbacks(void);
 void extint_detection_callback(void);


 #define BUTTON_0_EIC_LINE_custom		1


 void configure_extint_channel(void)
 {

	 struct extint_chan_conf config_extint_chan;
	 extint_chan_get_config_defaults(&config_extint_chan);

	 config_extint_chan.gpio_pin           = PIN_PA15A_EIC_EXTINT1;
	 config_extint_chan.gpio_pin_mux       = MUX_PA15A_EIC_EXTINT1;
	 config_extint_chan.wake_if_sleeping   = true;

	 config_extint_chan.gpio_pin_pull      = EXTINT_PULL_UP;
	 config_extint_chan.detection_criteria = EXTINT_DETECT_FALLING;
	 extint_chan_set_config(BUTTON_0_EIC_LINE_custom, &config_extint_chan);

 }




 
int long_press_var;

 long_press_var = 0;


 void configure_extint_callbacks(void)
 {
	 
	 
	 extint_register_callback(extint_detection_callback,
	 BUTTON_0_EIC_LINE_custom,
	 EXTINT_CALLBACK_TYPE_DETECT);

	 extint_chan_enable_callback(BUTTON_0_EIC_LINE_custom,
	 EXTINT_CALLBACK_TYPE_DETECT);
 }



 
 /************************************************************************/
 /* PWM DEFINITIONS														 */
 /************************************************************************/

 #define PWM_GENERATOR													TC2
 #define PWM_PIN_OUT  													PIN_PA10E_TC2_WO0;
 #define PWM_MUX_OUT													MUX_PA10E_TC2_WO0;
 #define PWM_PERIOD_VALUE  											    85
 #define INITIAL_DUTY_CYCLE  											((int)lround (PWM_PERIOD_VALUE / 4.0))
 #define FIRST_DUTY_CYCLE  											    ((int)lround (PWM_PERIOD_VALUE / 2.5))
 #define SECOND_DUTY_CYCLE  											((int)lround (PWM_PERIOD_VALUE / 1.10))
 #define MAX_TOGGLE_COUNT  											    3



 /************************************************************************/
 /* SYSTEM GPIO/PIN DEFINITIONS                                          */
 /************************************************************************/

 #define VBUS_PIN														PIN_PA27
 #define MOTOR_NSLEEP_PIN												PIN_PA06
 #define SWITCH_OFF_PIN													PIN_PA07

 #define CHARGING_PIN													PIN_PA11
 #define CHARGN_OFF_PIN													PIN_PA03		//TELLING BATTERY TO SHUTOFF

 #define SAMPLE_ADC_PIN													PIN_PA02
 #define BUTTON_2														PIN_PA15
 #define BUTTON_1														SW0_PIN
 #define XPLAINED_LED													PIN_PA16


 /************************************************************************/
 /* SYSTEM TICK DEFINITIONS												*/
 /************************************************************************/

 struct tc_module system_timer_instance;									// instance for system timer counter (TC1)

 #define SYSTEM_TC														TC1
 #define TC_COUNTER_SIZE													TC_COUNTER_SIZE_8BIT
 #define TC_CLOCK_PRESCALER 											    TC_CLOCK_PRESCALER_DIV64
 #define TC_CLOCK_SOURCE  											    GCLK_GENERATOR_0
 #define SYSTEM_TC_PERIOD_VALUE  										111												// Tuned for 1.99 -- 2.002 ms (1ms * 2) Period
 #define SYSTEM_TC_PIN_OUT												PIN_PA16E_TC1_WO0
 #define SYSTEM_TC_MUX_OUT												MUX_PA16E_TC1_WO0


 /************************************************************************/
 /* MOTOR CONTROL Variables                                          */
 /************************************************************************/
 extern uint8_t motor_toggle_count;
 extern bool motor_running;
 extern bool pulsating_motor_routine;

struct tc_module pwm_generator_instance;								// instance for PWM Motor Control (TC0)

 /************************************************************************/
 /* Motor control function prototypes                                        */
 /************************************************************************/
 void configure_pwm_generator(void);
 void cycle_pwm_motor (void);
 void motor_disable(void);
 void toggle_nsleep(void);
 void motor_enable(void);


 bool pulsating_motor_routine = false;
 uint8_t motor_toggle_count = 0;
 bool motor_running = false;

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



 void motor_enable(void){
	 tc_enable(&pwm_generator_instance);
	 port_pin_set_output_level(MOTOR_NSLEEP_PIN, HIGH);
	 motor_running = true;
	 //set_motor_speed_1_indication();

 }


 void motor_disable(void){
	 pulsating_motor_routine = false;
	 motor_toggle_count = 0;
	 motor_running = false;

	 tc_set_compare_value (&pwm_generator_instance,TC_COMPARE_CAPTURE_CHANNEL_0,INITIAL_DUTY_CYCLE);
	 tc_disable (&pwm_generator_instance);
	 port_pin_set_output_level(MOTOR_NSLEEP_PIN,LOW);

 }


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
	config_port_pin.input_pull = PORT_PIN_PULL_DOWN;							// START AT PULL DOWN.
	port_pin_set_config(MOTOR_NSLEEP_PIN, &config_port_pin);
	
	
	
	/*
	
	shutdown routine -- turn PA07 low to turn off peripherals
	
	*/
	
	config_port_pin.direction  = PORT_PIN_DIR_OUTPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_UP;								// START AT PULL UP.
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
	port_pin_set_config(CHARGING_PIN, &config_port_pin);
	
	
	/*
	GET_CHARGING_OFF_STATUS
	*/
	
	config_port_pin.direction  = PORT_PIN_DIR_OUTPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_DOWN;							// START AT PULL DOWN.
	port_pin_set_config(CHARGN_OFF_PIN, &config_port_pin);
	
	/*
	SAMPLE ADC PIN
	*/
	
	config_port_pin.direction  = PORT_PIN_DIR_INPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_DOWN;							// START AT PULL DOWN.
	port_pin_set_config(SAMPLE_ADC_PIN, &config_port_pin);
	


	/* Configure LEDs as outputs, turn them off */
	config_port_pin.direction  = PORT_PIN_DIR_OUTPUT;
	port_pin_set_config(XPLAINED_LED, &config_port_pin);

	///* Set buttons as inputs */
	config_port_pin.direction  = PORT_PIN_DIR_INPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_UP;
	port_pin_set_config(BUTTON_1, &config_port_pin);


}




 
 void cycle_pwm_motor (void)
 {


		 if (motor_running)
		 {
			 
			 if (motor_toggle_count == 2)
			 {
				 tc_set_compare_value (&pwm_generator_instance,
				 TC_COMPARE_CAPTURE_CHANNEL_0, FIRST_DUTY_CYCLE);
				 motor_toggle_count += 1;
				 //set_motor_speed_2_indication();
			 }
			 else if (motor_toggle_count == 3)
			 {
				 tc_set_compare_value (&pwm_generator_instance,
				 TC_COMPARE_CAPTURE_CHANNEL_0,
				 SECOND_DUTY_CYCLE);
				 //set_motor_speed_3_indication();
				 motor_toggle_count += 1;

			 }
			 
			 else if (motor_toggle_count == 4)
			 {

				 pulsating_motor_routine = true;
				 tc_set_compare_value (&pwm_generator_instance,
				 TC_COMPARE_CAPTURE_CHANNEL_0,
				 SECOND_DUTY_CYCLE);
				 //set_motor_pulsating_indication();
				 motor_toggle_count += 1;

			 }
			 
			 else if (motor_toggle_count > 4)
			 {

				 motor_disable();
				 
			 }
		 } else {
			motor_enable();
		 }

 }



 void toggle_nsleep(void){
	 static bool PULSATING_MOTOR = false;
	 if (pulsating_motor_routine){
		 if (PULSATING_MOTOR){
			 port_pin_set_output_level(MOTOR_NSLEEP_PIN,LOW);
			 PULSATING_MOTOR = false;
			 }else{
			 PULSATING_MOTOR = true;
			 port_pin_set_output_level(MOTOR_NSLEEP_PIN,HIGH);
		 }
	 }
 }






 void extint_detection_callback(void)
 {
	





	cycle_pwm_motor();

 }


 int main(void)
 {
		system_init();
		configure_port_pins();
		configure_extint_channel();
		configure_extint_callbacks();
		system_interrupt_enable_global();
		configure_pwm_generator();
		motor_running = true;
		motor_toggle_count = 2;
		//motor_enable();
	 //system_set_sleepmode(SYSTEM_SLEEPMODE_STANDBY);
	 //system_sleep();
	 while (true) {
		 //port_pin_set_output_level(LED_0_PIN, false);
	 }
 }
