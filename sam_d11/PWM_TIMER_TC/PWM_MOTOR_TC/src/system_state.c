/*
 * system_state.c
 *
 * Created: 7/22/2024 5:13:32 PM
 *  Author: jatan
 */ 

 #include "system_state.h"
 
bool VBUS_STATE;
bool CHARGN_ON_STATE;
bool CHARGN_OFF_STATE;
bool BATTERY_CHARGING;
bool BATTERY_CHARGED;
bool BATTERY_LOW;
bool BATTERY_LOWEST;


/************************************************************************/
/* GET SYSTEM STATES			                                        */
/************************************************************************/


void get_vbus_state(void) {
  VBUS_STATE = port_pin_get_input_level(VBUS_PIN);
}

void get_charging_on_status_state(void) {
  CHARGN_ON_STATE = port_pin_get_input_level(CHARGN_ON_PIN);
}

void get_charging_off_status_state(void) {
  CHARGN_OFF_STATE = port_pin_get_input_level(CHARGN_OFF_PIN);
}

void update_battery_states(void) {
  get_vbus_state();
  get_charging_on_status_state();
  get_charging_off_status_state();

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

  if (BATTERY_LOWEST) {
    set_battery_low_routine();
  } else if (BATTERY_LOW) {
    set_color_red();
  } else if (BATTERY_CHARGED) {
    set_color_cyan(); // set_color_green(); (currently flipped hence,)
  } else if (BATTERY_CHARGING) {
    set_battery_charge_routine();
  }

}

/************************************************************************/
/* STATE MACHINE		                                                */
/************************************************************************/

void system_state(void) {
  update_battery_states();
  display_battery_state();
}