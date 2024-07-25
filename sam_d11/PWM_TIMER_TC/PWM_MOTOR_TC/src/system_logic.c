/*
 * system_logic.c
 *
 * Created: 7/22/2024 5:19:03 PM
 *  Author: jatan
 */ 

 #include <system_logic.h>

 bool BATTERY_CHARGING = false;
 bool BATTERY_CHARGED = false;
 bool BATTERY_LOW = false;
 bool BATTERY_LOWEST = false;


bool Vbus_State;
bool Chargn_On_State;
bool Chargn_Off_State;


 void system_inactive(void) {

	 motor_disable();						// shutdown pwm motor
	 //pwm_led_system_cleanup();				// shutdown illumination led

 }



 void regular_routine(void) {
	 static bool motor_status_changed = false;
	 static bool led_button_status_changed = false;

	 //-------------------------------------------------------------

	 if (is_button_one_pressed()) {
		if (LongPressB1Flag) {
			system_inactive();
			LongPressB1Flag = false; // ALLOW IT TO CYCLE AGAIN
		} else {
			if (!motor_status_changed) {
				motor_toggle_count++;
				motor_status_changed = true;
				if (!motor_running) {
					motor_enable();
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
			 system_inactive();
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

	 if (Vbus_State == false) {
		;																					// Enable Motor PWM
	 } else {
	
		if (motor_running){
			system_inactive();
		}
																			// ITS PLUGGED IN
		if (Chargn_On_State == false) {															// battery charging (plugged in)
			BATTERY_CHARGING = true;													// show battery charge routine
		} else {
			BATTERY_CHARGING = false;
			//BATTERY_CHARGED = false;
		}
		
	}
 }








/*
adc_result

*/




/*


Voltage		Voltage Drop			Dec Value		Hex Value
4.2			0.91					3727.36			E8F
3.8			0.8233333333			3372.373333		D2C
3.6			0.78					3194.88			C7A


adc_result : 0.856

*/

//BatteryState get_battery_level(void) {
	//BatteryState state;
//
	//if (adc_result < VOLTAGE_THRESH_LOWEST) {
		//state = BATTERY_STATE_LOWEST;
	//}
	//else if (adc_result < VOLTAGE_THRESH_LOW) {
		//state = BATTERY_STATE_LOW;
	//}
	//else if (adc_result < VOLTAGE_THRESH_MAX) {
		//state = BATTERY_STATE_CHARGED;
	//}
	//else {
	//}
//
	//return state;
//}


void get_battery_level(void) {
	if (adc_result < VOLTAGE_THRESH_LOWEST) {
		BATTERY_LOWEST = true;
		BATTERY_LOW = false;
		BATTERY_CHARGED = false;

	}
	else if (adc_result < VOLTAGE_THRESH_LOW) {
		BATTERY_LOWEST = false;
		BATTERY_LOW = true;
		BATTERY_CHARGED = false;

	}
	else if (adc_result < VOLTAGE_THRESH_MAX){
		BATTERY_LOWEST = false;
		BATTERY_LOW = false;
		BATTERY_CHARGED = true;
	}
	else{
	}
}

 /************************************************************************/
 /* LOGIC MACHINE		                                                */
 /************************************************************************/



 



  void blink_leds(void){
	  if (BATTERY_LOW || BATTERY_CHARGING){
		  toggle_red_led();
	  }
  }

 void system_logic(void) {

	 
	 if (SYS_TICK_10MS) {
		 SYS_TICK_10MS = false;
		 system_state();						// Get latest system_state
		 regular_routine();

	 }

	 if (SYS_TICK_200MS) {
		 SYS_TICK_200MS = false;
		 toggle_nsleep();
		 sample_adc();
		 get_battery_level();
		 blink_leds();
	 }




 }