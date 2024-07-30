/*
 * system_logic.h
 *
 * Created: 7/22/2024 5:19:18 PM
 *  Author: jatan
 */ 


#ifndef SYSTEM_LOGIC_H_
#define SYSTEM_LOGIC_H_

#include "asf.h"

#include "system_setup.h"

#include "pwm_motor.h"

#include "led_driver.h"

#include "pwm_led.h"

#include "button.h"

#include "system_state.h"

#include "adc_sample.h"



typedef enum {
	BATTERY_STATE_LOWEST,
	BATTERY_STATE_LOW,
	BATTERY_STATE_CHARGED,
} BatteryState;

extern bool BATTERY_CHARGING;
extern bool BATTERY_CHARGED;
extern bool BATTERY_LOW;
extern bool BATTERY_LOWEST;

void regular_routine(void);
void system_inactive(void);
void system_logic(void);
void get_battery_level(void);

#define VOLTAGE_THRESH_MAX		0x0E8F
#define VOLTAGE_THRESH_LOW		0x0D2C
#define VOLTAGE_THRESH_LOWEST	0x0C7A

/*
0x0C7A (3194.88) -- Calculated (3.6v)
0x0D2C (3372.37) -- LWO THRESHOLD (3.8)
0x0D1E (3358.00) -- Close to low threshold for testing
*/


#endif /* SYSTEM_LOGIC_H_ */