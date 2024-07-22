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


void regular_routine(void);
void system_shutdown(void);
void system_logic(void);

#endif /* SYSTEM_LOGIC_H_ */