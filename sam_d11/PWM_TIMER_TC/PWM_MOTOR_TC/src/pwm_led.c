/*
 * pwm_led.c
 *
 * Created: 7/16/2024 2:41:09 PM
 *  Author: jatan
 */ 

#include "pwm_led.h"

//void set_color_channel(uint8_t channel, bool enable) {	
	////tcc_reset(&tcc_instance);
	//config_tcc.pins.enable_wave_out_pin[channel] = enable;
	//tcc_init(&tcc_instance, CONF_PWM_MODULE, &config_tcc);
	//tcc_enable(&tcc_instance);
//}


void set_color_channel(uint8_t channel, bool enable) {
	// Disable all channels first
	for (int i = 0; i < 4; i++) {
		config_tcc.pins.enable_wave_out_pin[i] = false;
	}

	// Enable only the specified channel
	if (enable) {
		config_tcc.pins.enable_wave_out_pin[channel] = true;
	}

	// Reinitialize and enable TCC
	tcc_init(&tcc_instance, CONF_PWM_MODULE, &config_tcc);
	tcc_enable(&tcc_instance);
}



void turn_off_all(void) {
	tcc_disable(&tcc_instance);
}


void set_color(int color) {
	turn_off_all();  

	switch (color) {
		case 0:  // Red
		set_color_channel(RED_CHANNEL, true);
		break;
		case 1:  // Green
		set_color_channel(GREEN_CHANNEL, true);
		break;
		case 2:  // Blue
		set_color_channel(BLUE_CHANNEL, true);
		break;
		case 3:  // White
		set_color_channel(WHITE_CHANNEL, true);
		break;
		case 4:  // Yellow (Red + Green)
		set_color_channel(RED_CHANNEL, true);
		set_color_channel(GREEN_CHANNEL, true);
		break;
		case 5:  // Purple (Red + Blue)
		set_color_channel(RED_CHANNEL, true);
		set_color_channel(BLUE_CHANNEL, true);
		break;
		case 6:  // Cyan (Green + Blue)
		set_color_channel(GREEN_CHANNEL, true);
		set_color_channel(BLUE_CHANNEL, true);
		break;
	}
}

void configure_pwm_tcc(void)
{
	tcc_get_config_defaults(&config_tcc, CONF_PWM_MODULE);
	config_tcc.compare.wave_generation = TCC_WAVE_GENERATION_DOUBLE_SLOPE_TOP;
	
	config_tcc.compare.match[0]								= CONF_DEFAULT_MATCH_COMPARE;
	config_tcc.compare.match[1]								= CONF_DEFAULT_MATCH_COMPARE;
	config_tcc.compare.match[2]								= CONF_DEFAULT_MATCH_COMPARE;
	config_tcc.compare.match[3]								= CONF_DEFAULT_MATCH_COMPARE;
	config_tcc.counter.period                               = CONF_DEFAULT_PERIOD;
	
	config_tcc.compare.wave_polarity[RED_CHANNEL]			= 1;
	config_tcc.compare.wave_polarity[GREEN_CHANNEL]			= 1;
	config_tcc.compare.wave_polarity[BLUE_CHANNEL]			= 1;
	config_tcc.compare.wave_polarity[WHITE_CHANNEL]			= 1;
	
	
	config_tcc.pins.wave_out_pin[RED_CHANNEL]			= PIN_PA04F_TCC0_WO0;		// RED
	config_tcc.pins.wave_out_pin_mux[RED_CHANNEL]		= MUX_PA04F_TCC0_WO0;
	config_tcc.compare.match[0]   = 0x0FF;

	config_tcc.pins.wave_out_pin[GREEN_CHANNEL]			= PIN_PA05F_TCC0_WO1;		// GREEN
	config_tcc.pins.wave_out_pin_mux[GREEN_CHANNEL]		= MUX_PA05F_TCC0_WO1;
	config_tcc.compare.match[1]   = 0x1FF;

	config_tcc.pins.wave_out_pin[BLUE_CHANNEL]			= PIN_PA08E_TCC0_WO2;		// BLUE
	config_tcc.pins.wave_out_pin_mux[BLUE_CHANNEL]		= MUX_PA08E_TCC0_WO2;
	config_tcc.compare.match[2]   = 0x2FF;

	config_tcc.pins.wave_out_pin[WHITE_CHANNEL]			= PIN_PA09F_TCC0_WO5;		// WHITE
	config_tcc.pins.wave_out_pin_mux[WHITE_CHANNEL]		= MUX_PA09F_TCC0_WO5;
	config_tcc.compare.match[3]   = 0X200;
	
	tcc_init(&tcc_instance, CONF_PWM_MODULE, &config_tcc);
	tcc_enable(&tcc_instance);
	
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
	

void set_red(void){
	set_color(0);
}

void set_green(void){
	set_color(1);
	
}

void set_blue(void){
	set_color(2);
}

void set_white(void){
	set_color(3);
}


void set_yellow(void){
	set_color(4);
	
}

void set_purple(void){
	set_color(5);
	
}

void set_cyan(void){
	set_color(6);
	
}

