/*
 * system_state.c
 *
 * Created: 7/22/2024 5:13:32 PM
 *  Author: jatan
 */ 

 #include "system_state.h"
 
bool Vbus_State;
bool Charged_State;
bool Chargn_Off_State;

bool BATTERY_CHARGING;
bool BATTERY_CHARGED;
bool BATTERY_LOW;
bool BATTERY_LOWEST;


/************************************************************************/
/* GET SYSTEM STATES			                                        */
/************************************************************************/


#define VBUS_STATE port_pin_get_input_level(VBUS_PIN)
#define CHARGED_STATE port_pin_get_input_level(CHARGING_PIN)
//#define CHARGN_OFF_STATE port_pin_get_input_level(CHARGN_OFF_PIN)


void update_battery_states(void) {
  Vbus_State = VBUS_STATE;
  Charged_State = CHARGED_STATE;
}






/************************************************************************/
/* Indication LED Control                                               */
/************************************************************************/













void display_battery_state(void) {

  /*
  LED charging indicator light:
  
  1. blinking red when device is dead
  2. Steady red light when device has a low battery
  3. Blinking green light when device is charging
  4. Steady green light when the device is at least 100% charged.
  */

  if (Vbus_State) {
	  if (BATTERY_CHARGING) {
		  set_color_green();  //  blink
		  } else {
		  set_color_green();
		  }
	  } else {
	  if (BATTERY_LOWEST) {
		  //set_color_red();	//  blink
		  } else if (BATTERY_LOW) {
		  //set_color_red();
		  } else if (BATTERY_CHARGED) {
		  set_color_green();
	  }
  }

        
 

}

/************************************************************************/
/* STATE MACHINE		                                                */
/************************************************************************/

void system_state(void) {
  update_battery_states();
  display_battery_state();
}