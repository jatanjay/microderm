/*
 * system_state.h
 *
 * Created: 7/22/2024 5:13:47 PM
 *  Author: jatan
 */ 


#ifndef SYSTEM_STATE_H_
#define SYSTEM_STATE_H_

#include <asf.h>
#include <system_setup.h>

extern bool VBUS_STATE;
extern bool CHARGN_ON_STATE;
extern bool CHARGN_OFF_STATE;
extern bool BATTERY_CHARGING;
extern bool BATTERY_CHARGED;
extern bool BATTERY_LOW;
extern bool BATTERY_LOWEST;


void get_vbus_state(void);
void get_charging_on_status_state(void);
void update_battery_states(void);
void get_charging_off_status_state(void);
void display_battery_state(void);
void system_state(void);

#endif /* SYSTEM_STATE_H_ */