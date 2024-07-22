/*
 * system_logic.c
 *
 * Created: 7/22/2024 5:19:03 PM
 *  Author: jatan
 */ 

 #include <system_logic.h>

 void system_shutdown(void) {
	 pwm_motor_cleanup(); // shutdown pwm motor
	 pwm_led_system_cleanup(); // shutdown illumination led
 }



 void regular_routine(void) {
	 static bool motor_status_changed = false;
	 static bool led_button_status_changed = false;

	 //-------------------------------------------------------------

	 if (is_button_one_pressed()) {
		 if (LongPressB1Flag) {
			 system_shutdown();
			 LongPressB1Flag = false; // ALLOW IT TO CYCLE AGAIN
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
			 system_shutdown();
			 LongPressB2Flag = false; // ALLOW IT TO CYCLE AGAIN
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


 
 /************************************************************************/
 /* LOGIC MACHINE		                                                */
 /************************************************************************/

 void system_logic(void) {
	 if (!VBUS_STATE) {
		 reset_chip();
		 configure_pwm_generator();						// Enable Motor PWM
		 } else {
		 system_shutdown();
		 if (!CHARGN_ON_STATE) {							// battery charging (plugged in)
			 BATTERY_CHARGING = true;						// show battery charge routine
			 BATTERY_CHARGED = false;
			 } else if (!CHARGN_OFF_STATE) {
			 BATTERY_CHARGING = false;
			 BATTERY_CHARGED = true;
		 }
	 }
	 if (SYS_TICK_10MS) {
		 SYS_TICK_10MS = false;
		 regular_routine();
	 }
	 if (SYS_TICK_200MS) {
		 SYS_TICK_200MS = false;
		 toggle_nsleep();
	 }
 }