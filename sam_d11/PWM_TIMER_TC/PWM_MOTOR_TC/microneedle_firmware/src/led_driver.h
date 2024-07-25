/*
 * led_driver.h
 *
 * Created: 7/9/2024 4:31:06 PM
 *  Author: jatan
 */ 


#ifndef LED_DRIVER_H_
#define LED_DRIVER_H_

#include "asf.h"


#define MASTER_MODULE SERCOM2
#define KTD2026_DEVICE_ADDRESS			0x31		// pg. 17 of Data sheet
#define DATA_LENGTH_PRIMARY				2
#define DATA_LENGTH_SECONDARY			8

void set_color_green(void);
void set_color_red(void);
void set_color_blue(void);
void set_color_yellow(void);
void set_color_purple(void);
void set_color_cyan(void);
void set_color_white(void);
void reset_chip(void);
void set_battery_charge_routine(void);
void set_battery_low_routine(void);
void i2c_master_setup(void);

void  toggle_red_led(void);

#endif /* LED_DRIVER_H_ */