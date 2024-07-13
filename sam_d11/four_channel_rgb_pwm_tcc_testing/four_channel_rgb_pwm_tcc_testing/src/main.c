/*

auth : jatan pandya

atmel asf code for setting led via pwm

utilizes a switch mechanism to set pwm channels ( and combine ) to get colors

tinker with duty cycles to tweak intensity of primary colors ( and thus get new secondary colors) 


*/



#include <asf.h>

struct tcc_config config_tcc;
struct tcc_module tcc_instance;

#define CONF_PWM_MODULE             TCC0
#define CONF_DEFAULT_PERIOD         0x3FF
#define CONF_DEFAULT_MATCH_COMPARE  0x1FF

#define RED_CHANNEL    TCC_CHANNEL_NUM_0
#define GREEN_CHANNEL  TCC_CHANNEL_NUM_1
#define BLUE_CHANNEL   TCC_CHANNEL_NUM_2
#define WHITE_CHANNEL  TCC_CHANNEL_NUM_5

enum eTCC_Channel {
	TCC_CHANNEL_NUM_0 = 0,
	TCC_CHANNEL_NUM_1,
	TCC_CHANNEL_NUM_2,
	TCC_CHANNEL_NUM_3,
	TCC_CHANNEL_NUM_4,
	TCC_CHANNEL_NUM_5,
	TCC_CHANNEL_NUM_6
};



void turn_off_all(void);
void configure_tcc(void);
void set_color(int color);
void set_color_channel(uint8_t channel, bool enable);



void set_color_channel(uint8_t channel, bool enable) {
	tcc_reset(&tcc_instance);
	config_tcc.pins.enable_wave_out_pin[channel] = enable;
	tcc_init(&tcc_instance, CONF_PWM_MODULE, &config_tcc);
	tcc_enable(&tcc_instance);
}

void turn_off_all(void) {
	tcc_disable(&tcc_instance);
}


void set_color(int color) {
	turn_off_all();  // Ensure all are off before setting new color

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
		default:
		turn_off_all();
		break;
	}
}

void configure_tcc(void)
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




int main(void) {
	
	/*
	0	r
	1	g
	2	b
	3	w
	
	4	y	(r + g)
	5	p	(r + b)
	6	c	(b + g)
	*/
	
	configure_tcc();
	
	
	set_color(0);
	set_color(1);
	set_color(2);
	set_color(3);
	set_color(4);
	set_color(5);	
	set_color(6);

}