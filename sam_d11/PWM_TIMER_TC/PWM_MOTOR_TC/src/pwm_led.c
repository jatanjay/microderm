/*
 * pwm_led.c
 *
 * Created: 7/16/2024 2:41:09 PM
 *  Author: jatan
 */ 

#include "pwm_led.h"

void set_pwm_color_channel(uint8_t channel, bool enable) {	
	//tcc_reset(&tcc_instance);
	
	config_tcc.pins.enable_wave_out_pin[channel] = enable;
	
	tcc_init(&tcc_instance, CONF_PWM_MODULE, &config_tcc);
	tcc_enable(&tcc_instance);
}





void turn_off_all(void) {
	tcc_disable(&tcc_instance);
}


void set_pwm_color(int color) {
	turn_off_all();  

	switch (color) {
		case 0:  // Red
		set_pwm_color_channel(RED_CHANNEL, true);
		break;
		case 1:  // Green
		set_pwm_color_channel(GREEN_CHANNEL, true);
		break;
		case 2:  // Blue
		set_pwm_color_channel(BLUE_CHANNEL, true);
		break;
		case 3:  // White
		set_pwm_color_channel(WHITE_CHANNEL, true);
		break;
		case 4:  // Yellow (Red + Green)
		set_pwm_color_channel(RED_CHANNEL, true);
		set_pwm_color_channel(GREEN_CHANNEL, true);
		break;
		case 5:  // Purple (Red + Blue)
		set_pwm_color_channel(RED_CHANNEL, true);
		set_pwm_color_channel(BLUE_CHANNEL, true);
		break;
		case 6:  // Cyan (Green + Blue)
		set_pwm_color_channel(GREEN_CHANNEL, true);
		set_pwm_color_channel(BLUE_CHANNEL, true);
		break;
	}
}

void configure_pwm_tcc(void)
{
	tcc_get_config_defaults(&config_tcc, CONF_PWM_MODULE);
	config_tcc.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;
	
	config_tcc.compare.match[RED_CHANNEL]								= CONF_DEFAULT_MATCH_COMPARE;
	config_tcc.compare.match[BLUE_CHANNEL]								= CONF_DEFAULT_MATCH_COMPARE;
	config_tcc.compare.match[GREEN_CHANNEL]								= CONF_DEFAULT_MATCH_COMPARE;
	config_tcc.compare.match[WHITE_CHANNEL]								= CONF_DEFAULT_MATCH_COMPARE;


	//config_tcc.compare.wave_polarity[RED_CHANNEL] = 1;
	//config_tcc.compare.wave_polarity[BLUE_CHANNEL] = 1;
	//config_tcc.compare.wave_polarity[GREEN_CHANNEL] = 1;
	//config_tcc.compare.wave_polarity[WHITE_CHANNEL] = 1;

	config_tcc.counter.period											= CONF_DEFAULT_PERIOD;
	
	
	config_tcc.pins.wave_out_pin[RED_CHANNEL]							= PIN_PA04F_TCC0_WO0;		// RED
	config_tcc.pins.wave_out_pin_mux[RED_CHANNEL]						= MUX_PA04F_TCC0_WO0;
	config_tcc.compare.match[RED_CHANNEL]											= 0x2FF;

	config_tcc.pins.wave_out_pin[GREEN_CHANNEL]							= PIN_PA05F_TCC0_WO1;		// GREEN
	config_tcc.pins.wave_out_pin_mux[GREEN_CHANNEL]						= MUX_PA05F_TCC0_WO1;
	config_tcc.compare.match[GREEN_CHANNEL]											= 0XFFF;

	config_tcc.pins.wave_out_pin[BLUE_CHANNEL]							= PIN_PA08E_TCC0_WO2;		// BLUE
	config_tcc.pins.wave_out_pin_mux[BLUE_CHANNEL]						= MUX_PA08E_TCC0_WO2;
	config_tcc.compare.match[BLUE_CHANNEL]											= 0x2FF;

	config_tcc.pins.wave_out_pin[WHITE_CHANNEL]							= PIN_PA09E_TCC0_WO3;		// WHITE
	config_tcc.pins.wave_out_pin_mux[WHITE_CHANNEL]						= MUX_PA09E_TCC0_WO3;
	//config_tcc.compare.match[WHITE_CHANNEL]											= 0xFFF;
	
	tcc_init(&tcc_instance, CONF_PWM_MODULE, &config_tcc);
	tcc_enable(&tcc_instance);
	tcc_restart_counter(&tcc_instance);
	
}



/*
0	r
1	g
2	b
3	w
	
4	y	(r + g)
5	p	(r + b)
6	c	(b + g)
*/
	

void set_pwm_red(void){
	set_pwm_color(0);
}

void set_pwm_green(void){
	set_pwm_color(1);
	
}

void set_pwm_blue(void){
	set_pwm_color(2);
}

void set_pwm_white(void){
	set_pwm_color(3);
}


void set_pwm_yellow(void){
	set_pwm_color(4);
	
}

void set_pwm_purple(void){
	set_pwm_color(5);
	
}

void set_pwm_cyan(void){
	set_pwm_color(6);
	
}
