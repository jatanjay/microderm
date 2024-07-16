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
struct tcc_config config_tcc;
struct tcc_module tcc_instance;

#define CONF_PWM_MODULE             TCC0
#define CONF_DEFAULT_PERIOD         0x3FF
#define CONF_DEFAULT_MATCH_COMPARE  0x1FF

#define RED_CHANNEL    TCC_CHANNEL_NUM_0
#define GREEN_CHANNEL  TCC_CHANNEL_NUM_1
#define BLUE_CHANNEL   TCC_CHANNEL_NUM_2
#define WHITE_CHANNEL  TCC_CHANNEL_NUM_5

//enum eTCC_Channel {
	//TCC_CHANNEL_NUM_0 = 0,
	//TCC_CHANNEL_NUM_1,
	//TCC_CHANNEL_NUM_2,
	//TCC_CHANNEL_NUM_3,
	//TCC_CHANNEL_NUM_4,
	//TCC_CHANNEL_NUM_5,
	//TCC_CHANNEL_NUM_6
//};



void turn_off_all(void);
void configure_pwm_tcc(void);
void set_color(int color);
void set_color_channel(uint8_t channel, bool enable);


void set_red(void);
void set_green(void);
void set_blue(void);
void set_white(void);

void set_yellow(void);
void set_purple(void);
void set_cyan(void);




#endif /* PWM_LED_H_ */