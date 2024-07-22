/*
 * led_driver.c
 *
 * Created: 7/9/2024 4:31:23 PM
 *  Author: jatan
 */ 

#include "led_driver.h"

struct i2c_master_module i2c_master_instance;

static uint8_t buffer_disable_green_channel[DATA_LENGTH_PRIMARY]						=	{0x04,0x00};
static uint8_t buffer_disable_flash_period[DATA_LENGTH_PRIMARY]							=	{0x01,0x00};
static uint8_t buffer_disable_trise_tfall[DATA_LENGTH_PRIMARY]							=	{0x05,0x00};
static uint8_t buffer_disable_pwm1_timer[DATA_LENGTH_PRIMARY]							=	{0x02,0x00};






static uint8_t buffer_set_color_green[DATA_LENGTH_PRIMARY]						=	{0x04,0x04};
static uint8_t buffer_set_color_red[DATA_LENGTH_PRIMARY]						=	{0x04,0x01};
static uint8_t buffer_set_color_blue[DATA_LENGTH_PRIMARY]						=	{0x04,0x10};
static uint8_t buffer_set_color_yellow[DATA_LENGTH_PRIMARY]						=	{0x04,0x05};
static uint8_t buffer_set_color_purple[DATA_LENGTH_PRIMARY]						=	{0x04,0x11};
static uint8_t buffer_set_color_cyan[DATA_LENGTH_PRIMARY]						=	{0x04,0x14};
static uint8_t buffer_set_color_white[DATA_LENGTH_PRIMARY]						=	{0x04,0x15};



static uint8_t buffer_set_color_green_PWM[DATA_LENGTH_PRIMARY]					=	{0x04,0x08};
static uint8_t buffer_set_charge_routine_flash_period[DATA_LENGTH_PRIMARY]		=	{0x01,0x25};
static uint8_t buffer_set_charge_routine_trise_tfall[DATA_LENGTH_PRIMARY]		=	{0x05,0xEF};
static uint8_t buffer_set_charge_routine_PWM1_timer[DATA_LENGTH_PRIMARY]		=	{0x02,0x1C};





static uint8_t buffer_set_color_red_PWM[DATA_LENGTH_PRIMARY]					=	{0x04,0x02};
static uint8_t buffer_set_low_routine_flash_period[DATA_LENGTH_PRIMARY]			=	{0x05,0x21};
static uint8_t buffer_set_low_routine_trise_tfall[DATA_LENGTH_PRIMARY]			=	{0x02,0x2D};
	



void set_color_green(void){	
	struct i2c_master_packet packet = {
		.address = KTD2026_DEVICE_ADDRESS,
		.data_length = DATA_LENGTH_PRIMARY,
		.data = buffer_set_color_green,
	};
	
	while ((i2c_master_write_packet_wait(&i2c_master_instance, &packet)) != STATUS_OK)
	{
	}
	
}

void set_color_red(void){
	struct i2c_master_packet packet = {
		.address = KTD2026_DEVICE_ADDRESS,
		.data_length = DATA_LENGTH_PRIMARY,
		.data = buffer_set_color_red,
	};
	
	while ((i2c_master_write_packet_wait(&i2c_master_instance, &packet)) != STATUS_OK)
	{
	}
}

void set_color_blue(void){
	struct i2c_master_packet packet = {
		.address = KTD2026_DEVICE_ADDRESS,
		.data_length = DATA_LENGTH_PRIMARY,
		.data = buffer_set_color_blue,
	};
	
	while ((i2c_master_write_packet_wait(&i2c_master_instance, &packet)) != STATUS_OK)
	{
	}
}

void set_color_yellow(void){
	struct i2c_master_packet packet = {
		.address = KTD2026_DEVICE_ADDRESS,
		.data_length = DATA_LENGTH_PRIMARY,
		.data = buffer_set_color_yellow,
	};
	
	while ((i2c_master_write_packet_wait(&i2c_master_instance, &packet)) != STATUS_OK)
	{
	}
}

void set_color_purple(void){
	struct i2c_master_packet packet = {
		.address = KTD2026_DEVICE_ADDRESS,
		.data_length = DATA_LENGTH_PRIMARY,
		.data = buffer_set_color_purple,
	};
	
	while ((i2c_master_write_packet_wait(&i2c_master_instance, &packet)) != STATUS_OK)
	{
	}
}

void set_color_cyan(void){
	struct i2c_master_packet packet = {
		.address = KTD2026_DEVICE_ADDRESS,
		.data_length = DATA_LENGTH_PRIMARY,
		.data = buffer_set_color_cyan,
	};
	
	while ((i2c_master_write_packet_wait(&i2c_master_instance, &packet)) != STATUS_OK)
	{
	}
}

void set_color_white(void){
	struct i2c_master_packet packet = {
		.address = KTD2026_DEVICE_ADDRESS,
		.data_length = DATA_LENGTH_PRIMARY,
		.data = buffer_set_color_white,
	};
	
	while ((i2c_master_write_packet_wait(&i2c_master_instance, &packet)) != STATUS_OK)
	{
	}
}

void reset_chip(void){
	// Disable PWM GREEN
	struct i2c_master_packet packet = {
		.address = KTD2026_DEVICE_ADDRESS,
		.data_length = DATA_LENGTH_PRIMARY,
		.data = buffer_disable_green_channel,
	};
	
	// Disable Ramp (flash)
	struct i2c_master_packet disable_ramp = {
		.address = KTD2026_DEVICE_ADDRESS,
		.data_length = DATA_LENGTH_PRIMARY,
		.data = buffer_disable_flash_period,
	};
	

	// Disable Pulse
	struct i2c_master_packet disable_trise_tfall = {
		.address = KTD2026_DEVICE_ADDRESS,
		.data_length = DATA_LENGTH_PRIMARY,
		.data = buffer_disable_trise_tfall,
	};

	// Disable pwm's timer
	struct i2c_master_packet disable_PWM1_timer = {
		.address = KTD2026_DEVICE_ADDRESS,
		.data_length = DATA_LENGTH_PRIMARY,
		.data = buffer_disable_pwm1_timer,
	};




	while ((i2c_master_write_packet_wait(&i2c_master_instance, &disable_PWM1_timer)) != STATUS_OK)
	{
	}
}






void set_battery_charge_routine(void){

	// Enable PWM GREEN
	struct i2c_master_packet packet = {
		.address = KTD2026_DEVICE_ADDRESS,
		.data_length = DATA_LENGTH_PRIMARY,
		.data = buffer_set_color_green_PWM,
	};
	
	// Enable Ramp (flash)
	struct i2c_master_packet set_ramp = {
		.address = KTD2026_DEVICE_ADDRESS,
		.data_length = DATA_LENGTH_PRIMARY,
		.data = buffer_set_charge_routine_flash_period,
	};
	

	// Enable Pulse
	struct i2c_master_packet set_trise_tfall = {
		.address = KTD2026_DEVICE_ADDRESS,
		.data_length = DATA_LENGTH_PRIMARY,
		.data = buffer_set_charge_routine_trise_tfall,
	};

	// Enable pwm's timer
	struct i2c_master_packet set_PWM1_timer = {
		.address = KTD2026_DEVICE_ADDRESS,
		.data_length = DATA_LENGTH_PRIMARY,
		.data = buffer_set_charge_routine_PWM1_timer,
	};


	while ((i2c_master_write_packet_wait(&i2c_master_instance, &packet)) != STATUS_OK)
	{
	}
	
	while ((i2c_master_write_packet_wait(&i2c_master_instance, &set_ramp)) != STATUS_OK)
	{
	}
	
	while ((i2c_master_write_packet_wait(&i2c_master_instance, &set_trise_tfall)) != STATUS_OK)
	{
	}

	while ((i2c_master_write_packet_wait(&i2c_master_instance, &set_PWM1_timer)) != STATUS_OK)
	{
	}
	
	
}

void set_battery_low_routine(void){

	// Enable PWM RED
	struct i2c_master_packet packet = {
		.address = KTD2026_DEVICE_ADDRESS,
		.data_length = DATA_LENGTH_PRIMARY,
		.data = buffer_set_color_red_PWM,
	};
	
	// Enable Ramp (flash)
	struct i2c_master_packet set_ramp = {
		.address = KTD2026_DEVICE_ADDRESS,
		.data_length = DATA_LENGTH_PRIMARY,
		.data = buffer_set_low_routine_flash_period,
	};
	
	// Enable Pulse
	struct i2c_master_packet set_trise_tfall = {
		.address = KTD2026_DEVICE_ADDRESS,
		.data_length = DATA_LENGTH_PRIMARY,
		.data = buffer_set_low_routine_trise_tfall,
	};



	while ((i2c_master_write_packet_wait(&i2c_master_instance, &packet)) != STATUS_OK)
	{
	}
	
	while ((i2c_master_write_packet_wait(&i2c_master_instance, &set_ramp)) != STATUS_OK)
	{
	}
	
	while ((i2c_master_write_packet_wait(&i2c_master_instance, &set_trise_tfall)) != STATUS_OK)
	{
	}
	
}





void i2c_master_setup(void){
	struct i2c_master_config config_i2c_master;
	i2c_master_get_config_defaults(&config_i2c_master);
	config_i2c_master.buffer_timeout = 10000;
	i2c_master_init(&i2c_master_instance, MASTER_MODULE, &config_i2c_master);
	i2c_master_enable(&i2c_master_instance);
	
}
