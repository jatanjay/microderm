/*
 * pwm_led.c
 *
 * Created: 7/16/2024 2:41:09 PM
 *  Author: jatan
 */ 

#include "pwm_led.h"

uint8_t pwm_led_toggle_count = 0;


void set_pwm_color_channel(uint8_t channel, bool enable) {
	
	
	if (enable){
		tcc_set_compare_value(&tcc_instance,
		channel,
		0x3FF);
	}else{
		tcc_set_compare_value(&tcc_instance,
		channel,
		ZERO_DUTY_CYCLE);
	}
	
	tcc_force_double_buffer_update(&tcc_instance);
}



void pwm_led_system_cleanup(void) {
	set_pwm_color_channel(RED_CHANNEL, false);
	set_pwm_color_channel(BLUE_CHANNEL, false);
	set_pwm_color_channel(GREEN_CHANNEL, false);
	set_pwm_color_channel(WHITE_CHANNEL, false);
}



void set_pwm_color(int color) {
	
	pwm_led_system_cleanup();
	
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
		case 6:  // Cyan (Blue + Green)
		set_pwm_color_channel(BLUE_CHANNEL, true);
		set_pwm_color_channel(GREEN_CHANNEL, true);
		break;
		default:
		break;
	}
}


void configure_pwm_tcc(void)
{
	tcc_get_config_defaults(&config_tcc, CONF_PWM_MODULE);
	config_tcc.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;
	config_tcc.counter.period											= CONF_DEFAULT_PERIOD;
	
	
	config_tcc.compare.match[RED_CHANNEL]								= ZERO_DUTY_CYCLE;
	config_tcc.compare.match[BLUE_CHANNEL]								= ZERO_DUTY_CYCLE;
	config_tcc.compare.match[GREEN_CHANNEL]								= ZERO_DUTY_CYCLE;
	config_tcc.compare.match[WHITE_CHANNEL]								= ZERO_DUTY_CYCLE;

	config_tcc.pins.wave_out_pin[RED_CHANNEL]							= PIN_PA04F_TCC0_WO0;		// RED
	config_tcc.pins.wave_out_pin_mux[RED_CHANNEL]						= MUX_PA04F_TCC0_WO0;
	config_tcc.pins.enable_wave_out_pin[RED_CHANNEL] =					true;
	
	config_tcc.pins.wave_out_pin[GREEN_CHANNEL]							= PIN_PA05F_TCC0_WO1;		// GREEN
	config_tcc.pins.wave_out_pin_mux[GREEN_CHANNEL]						= MUX_PA05F_TCC0_WO1;
	config_tcc.pins.enable_wave_out_pin[GREEN_CHANNEL] = true;

	config_tcc.pins.wave_out_pin[BLUE_CHANNEL]							= PIN_PA08E_TCC0_WO2;		// BLUE
	config_tcc.pins.wave_out_pin_mux[BLUE_CHANNEL]						= MUX_PA08E_TCC0_WO2;
	config_tcc.pins.enable_wave_out_pin[BLUE_CHANNEL] = true;

	config_tcc.pins.wave_out_pin[WHITE_CHANNEL]							= PIN_PA09E_TCC0_WO3;		// WHITE
	config_tcc.pins.wave_out_pin_mux[WHITE_CHANNEL]						= MUX_PA09E_TCC0_WO3;
	config_tcc.pins.enable_wave_out_pin[WHITE_CHANNEL] = true;
	
	tcc_init(&tcc_instance, CONF_PWM_MODULE, &config_tcc);
	tcc_enable(&tcc_instance);
	
}


void cycle_pwm_led(void) {
	switch (pwm_led_toggle_count) {
		case 1:
		SET_RED;
		set_color_red();
		break;
		case 2:
		set_color_blue();
		SET_GRN;
		break;
		case 3:
		SET_BLU;
		break;
		case 4:
		SET_YLW;
		break;
		case 5:
		SET_PLE;
		break;
		case 6:
		SET_CYN;
		break;
		case 7:
		SET_WHT;
		break;
		default:
		case 8:
		pwm_led_system_cleanup();							// Reset to 1 for red
		pwm_led_toggle_count = 0;
		break;
	}
}
