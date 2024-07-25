/*
 * pwm_led.h
 *
 * Created: 7/16/2024 2:41:22 PM
 *  Author: jatan
 */ 


#ifndef PWM_LED_H_
#define PWM_LED_H_



#include "asf.h"
#include "conf_example.h"
#include "led_driver.h" // For testing i2c led using button 2


struct tcc_config config_tcc;
struct tcc_module tcc_instance;

#define CONF_PWM_MODULE             TCC0
#define CONF_DEFAULT_PERIOD         0xFFF
#define CONF_DEFAULT_MATCH_COMPARE  0x000
#define ZERO_DUTY_CYCLE				0x000

extern uint8_t pwm_led_toggle_count;

enum ColorChannel {
	RED_CHANNEL = TCC_CHANNEL_NUM_0,
	GREEN_CHANNEL = TCC_CHANNEL_NUM_1,
	BLUE_CHANNEL = TCC_CHANNEL_NUM_2,
	WHITE_CHANNEL = TCC_CHANNEL_NUM_3
};


#define SET_RED	set_pwm_color(0)
#define SET_GRN	set_pwm_color(1)
#define SET_BLU	set_pwm_color(2)
#define SET_WHT	set_pwm_color(3)
#define SET_YLW	set_pwm_color(4)
#define SET_PLE	set_pwm_color(5)
#define SET_CYN	set_pwm_color(6)



void pwm_led_system_cleanup(void);
void configure_pwm_tcc(void);
void set_pwm_color(int color);
void set_pwm_color_channel(uint8_t channel, bool enable);


//void set_pwm_red(void);
//void set_pwm_green(void);
//void set_pwm_blue(void);
//void set_pwm_white(void);
//
//void set_pwm_yellow(void);
//void set_pwm_purple(void);
//void set_pwm_cyan(void);

void cycle_pwm_led(void);

#endif /* PWM_LED_H_ */