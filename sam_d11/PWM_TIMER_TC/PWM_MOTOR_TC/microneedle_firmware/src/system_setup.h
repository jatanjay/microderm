/*
 * system_setup.h
 *
 * Created: 7/22/2024 2:43:17 PM
 *  Author: jatan
 */ 


#ifndef SYSTEM_SETUP_H_
#define SYSTEM_SETUP_H_


#include "asf.h"
#include "led_driver.h"
#include "pwm_led.h"
#include "adc_sample.h"
#include "pwm_motor.h"
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

extern bool SYS_TICK_10MS;
extern bool SYS_TICK_50MS;
extern bool SYS_TICK_100MS;
extern bool SYS_TICK_200MS;
//extern bool SYS_TICK_500MS;
//extern bool SYS_TICK_1000MS;


/************************************************************************/
/* Function Prototypes													*/
/************************************************************************/


void configure_port_pins(void);										// setup and configure GPIOs
void configure_system_tc (void);									// setup system timer (System Tick)
void system_tc_callbacks (void);									// system timer (system tick) handler
void sys_tc_callback (struct tc_module *const module_inst);			// system timer callback function to set up system ticks
void startup_default_pin_state(void);								// system startup GPIO pin set ups
void startup_sys_configs(void);										// Function calls to set up start up config functions



#endif /* SYSTEM_SETUP_H_ */