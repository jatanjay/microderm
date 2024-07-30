/*
 * led_driver.c
 *
 * Created: 7/9/2024 4:31:23 PM
 *  Author: jatan
 */

#include "led_driver.h"

struct i2c_master_module i2c_master_instance;
struct i2c_master_packet packet = {KTD2026_DEVICE_ADDRESS, DATA_LENGTH_PRIMARY};


#define BUFFER_TURN_OFF_MAIN_DIGITAL					{0x04, 0x00}
#define BUFFER_DISABLE_PWM1_TIMER						{0x02, 0x00}


#define BUFFER_SET_COLOR_GREEN							{0x04, 0x04}
#define BUFFER_SET_COLOR_RED							{0x04, 0x01}
#define BUFFER_SET_COLOR_BLUE							{0x04, 0x10}
#define BUFFER_SET_COLOR_YELLOW							{0x04, 0x05}
#define BUFFER_SET_COLOR_CYAN							{0x04, 0x14}
#define BUFFER_SET_COLOR_WHITE							{0x04, 0x15}
#define BUFFER_SET_COLOR_PURPLE							{0x04, 0x11}



#define BUFFER_SET_COLOR_GREEN_PWM						{0x04, 0x08}
#define BUFFER_SET_CHARGE_ROUTINE_FLASH_PERIOD			{0x01, 0x25}
#define BUFFER_SET_CHARGE_ROUTINE_TRISE_TFALL			{0x05, 0xEF}
#define BUFFER_SET_CHARGE_ROUTINE_PWM1_TIMER			{0x02, 0x1C}

#define BUFFER_SET_COLOR_RED_PWM						{0x04, 0x02}
#define BUFFER_SET_COLOR_RED_BRIGHTNESS					{0x06, 0x05}
#define BUFFER_SET_LOW_ROUTINE_FLASH_PERIOD				{0x01, 0x82}
#define BUFFER_SET_LOW_ROUTINE_TRISE_TFALL				{0x02, 0x32}


#define BUFFER_SET_COLOR_BLUE_LOWEST_BRIGTHNESS			{0x08, 0x03}
#define BUFFER_SET_COLOR_BLUE_LOW_BRIGTHNESS			{0x08, 0x09}
#define BUFFER_SET_COLOR_BLUE_HIGH_BRIGTHNESS			{0x08, 0x15}

#define BUFFER_SET_COLOR_BLUE_PWM						{0x04, 0x20}
#define BUFFER_SET_COLOR_BLUE_PWM_TIMER					{0x02, 0x19}
#define BUFFER_SET_COLOR_BLUE_PWM_FLASH					{0x01, 0x82}


void set_color_green_indication(void) {
  uint8_t buffer_set_color_green[DATA_LENGTH_PRIMARY] = BUFFER_SET_COLOR_GREEN;
  packet.data = buffer_set_color_green;
  packet.address = KTD2026_DEVICE_ADDRESS;
  packet.data_length = DATA_LENGTH_PRIMARY;
  while ((i2c_master_write_packet_wait(&i2c_master_instance, &packet)) !=
         STATUS_OK) {
  }
}

void set_color_red_indication(void) {
  uint8_t buffer_set_color_red[DATA_LENGTH_PRIMARY] = BUFFER_SET_COLOR_RED;
  packet.data = buffer_set_color_red;
  packet.address = KTD2026_DEVICE_ADDRESS;
  packet.data_length = DATA_LENGTH_PRIMARY;
  while ((i2c_master_write_packet_wait(&i2c_master_instance, &packet)) !=
         STATUS_OK) {
  }
}

void set_color_blue_indication(void) {
  uint8_t buffer_set_color_blue[DATA_LENGTH_PRIMARY] = BUFFER_SET_COLOR_BLUE;
  packet.data = buffer_set_color_blue;
  packet.address = KTD2026_DEVICE_ADDRESS;
  packet.data_length = DATA_LENGTH_PRIMARY;
  while ((i2c_master_write_packet_wait(&i2c_master_instance, &packet)) !=
         STATUS_OK) {
  }
}

void set_color_yellow_indication(void) {
	uint8_t buffer_set_color_yellow[DATA_LENGTH_PRIMARY] = BUFFER_SET_COLOR_YELLOW;
	packet.data = buffer_set_color_yellow;
	packet.address = KTD2026_DEVICE_ADDRESS;
	packet.data_length = DATA_LENGTH_PRIMARY;

	while ((i2c_master_write_packet_wait(&i2c_master_instance, &packet)) !=
	STATUS_OK) {
	}
}

void set_color_cyan_indication(void) {
	uint8_t buffer_set_color_cyan[DATA_LENGTH_PRIMARY] = BUFFER_SET_COLOR_CYAN;
	packet.data = buffer_set_color_cyan;
	packet.address = KTD2026_DEVICE_ADDRESS;
	packet.data_length = DATA_LENGTH_PRIMARY;

	while ((i2c_master_write_packet_wait(&i2c_master_instance, &packet)) !=
	STATUS_OK) {
	}
}

void set_color_white_indication(void) {
	uint8_t buffer_set_color_white[DATA_LENGTH_PRIMARY] = BUFFER_SET_COLOR_WHITE;
	packet.data = buffer_set_color_white;
	packet.address = KTD2026_DEVICE_ADDRESS;
	packet.data_length = DATA_LENGTH_PRIMARY;

	while ((i2c_master_write_packet_wait(&i2c_master_instance, &packet)) !=
	STATUS_OK) {
	}
}

void reset_chip(void) {

  uint8_t buffer_reset_green_red_channel[DATA_LENGTH_PRIMARY] = BUFFER_TURN_OFF_MAIN_DIGITAL;
  packet.data = buffer_reset_green_red_channel;
  packet.address = KTD2026_DEVICE_ADDRESS;
  packet.data_length = DATA_LENGTH_PRIMARY;

  while ((i2c_master_write_packet_wait(&i2c_master_instance,
                                       &packet)) != STATUS_OK) {
  }
}

void set_battery_charge_routine(void) {

  uint8_t buffer_set_color_green_PWM[DATA_LENGTH_PRIMARY] = BUFFER_SET_COLOR_GREEN_PWM;
  uint8_t buffer_set_charge_routine_flash_period[DATA_LENGTH_PRIMARY] = BUFFER_SET_CHARGE_ROUTINE_FLASH_PERIOD;
  uint8_t buffer_set_charge_routine_trise_tfall[DATA_LENGTH_PRIMARY] = BUFFER_SET_CHARGE_ROUTINE_TRISE_TFALL;
  uint8_t buffer_set_charge_routine_PWM1_timer[DATA_LENGTH_PRIMARY] = BUFFER_SET_CHARGE_ROUTINE_PWM1_TIMER;

  packet.address = KTD2026_DEVICE_ADDRESS;
  packet.data_length = DATA_LENGTH_PRIMARY;

  packet.data = buffer_set_color_green_PWM;
  while ((i2c_master_write_packet_wait(&i2c_master_instance, &packet)) !=
         STATUS_OK) {
  }
  packet.data = buffer_set_charge_routine_flash_period;
  while ((i2c_master_write_packet_wait(&i2c_master_instance, &packet)) !=
         STATUS_OK) {
  }
  packet.data = buffer_set_charge_routine_trise_tfall;
  while ((i2c_master_write_packet_wait(&i2c_master_instance,
                                       &packet)) != STATUS_OK) {
  }
  packet.data = buffer_set_charge_routine_PWM1_timer;
  while ((i2c_master_write_packet_wait(&i2c_master_instance,
                                       &packet)) != STATUS_OK) {
  }
}

void set_battery_low_routine(void) {

  uint8_t buffer_set_color_red_PWM[DATA_LENGTH_PRIMARY]				= BUFFER_SET_COLOR_RED_PWM;
  uint8_t buffer_set_low_routine_flash_period[DATA_LENGTH_PRIMARY]	= BUFFER_SET_LOW_ROUTINE_FLASH_PERIOD;
  uint8_t buffer_set_low_routine_trise_tfall[DATA_LENGTH_PRIMARY]	= BUFFER_SET_LOW_ROUTINE_TRISE_TFALL;
  uint8_t buffer_set_low_routine_brightness[DATA_LENGTH_PRIMARY]	= BUFFER_SET_COLOR_RED_BRIGHTNESS;

  packet.address = KTD2026_DEVICE_ADDRESS;
  packet.data_length = DATA_LENGTH_PRIMARY;

  packet.data = buffer_set_color_red_PWM;
  while ((i2c_master_write_packet_wait(&i2c_master_instance, &packet)) !=
         STATUS_OK) {
  }
  packet.data = buffer_set_low_routine_flash_period;
  while ((i2c_master_write_packet_wait(&i2c_master_instance, &packet)) !=
         STATUS_OK) {
  }
  packet.data = buffer_set_low_routine_brightness;
  while ((i2c_master_write_packet_wait(&i2c_master_instance,
                                       &packet)) != STATUS_OK) {
  }

	packet.data = buffer_set_low_routine_trise_tfall;
	while ((i2c_master_write_packet_wait(&i2c_master_instance,
	&packet)) != STATUS_OK) {
	}
}









void set_motor_speed_1_indication(void){

  uint8_t buffer_set_blue[DATA_LENGTH_PRIMARY] = BUFFER_SET_COLOR_BLUE_PWM;
  uint8_t buffer_set_blue_lowest_brightness[DATA_LENGTH_PRIMARY] = BUFFER_SET_COLOR_BLUE_LOWEST_BRIGTHNESS;

  packet.address = KTD2026_DEVICE_ADDRESS;
  packet.data_length = DATA_LENGTH_PRIMARY;
  
  packet.data = buffer_set_blue;
  while ((i2c_master_write_packet_wait(&i2c_master_instance,
  &packet)) != STATUS_OK) {
  }

  packet.data = buffer_set_blue_lowest_brightness;
  while ((i2c_master_write_packet_wait(&i2c_master_instance,
  &packet)) != STATUS_OK) {
  }

}

void set_motor_speed_2_indication(void){

	uint8_t buffer_set_blue[DATA_LENGTH_PRIMARY] = BUFFER_SET_COLOR_BLUE_PWM;
	uint8_t buffer_set_blue_low_brightness[DATA_LENGTH_PRIMARY] = BUFFER_SET_COLOR_BLUE_LOW_BRIGTHNESS;

	packet.address = KTD2026_DEVICE_ADDRESS;
	packet.data_length = DATA_LENGTH_PRIMARY;

	packet.data = buffer_set_blue;
	while ((i2c_master_write_packet_wait(&i2c_master_instance,
	&packet)) != STATUS_OK) {
	}

	packet.data = buffer_set_blue_low_brightness;
	while ((i2c_master_write_packet_wait(&i2c_master_instance,
	&packet)) != STATUS_OK) {
	}

}

void set_motor_speed_3_indication(void){
	uint8_t buffer_set_blue[DATA_LENGTH_PRIMARY] = BUFFER_SET_COLOR_BLUE_PWM;
	uint8_t buffer_set_blue_high_brightness[DATA_LENGTH_PRIMARY] = BUFFER_SET_COLOR_BLUE_HIGH_BRIGTHNESS;

	packet.address = KTD2026_DEVICE_ADDRESS;
	packet.data_length = DATA_LENGTH_PRIMARY;

	packet.data = buffer_set_blue;
	while ((i2c_master_write_packet_wait(&i2c_master_instance,
	&packet)) != STATUS_OK) {
	}

	packet.data = buffer_set_blue_high_brightness;
	while ((i2c_master_write_packet_wait(&i2c_master_instance,
	&packet)) != STATUS_OK) {
	}
}

void set_motor_pulsating_indication(void){
	uint8_t buffer_set_blue[DATA_LENGTH_PRIMARY] = BUFFER_SET_COLOR_BLUE_PWM;
	uint8_t buffer_set_blue_high_brightness[DATA_LENGTH_PRIMARY] = BUFFER_SET_COLOR_BLUE_HIGH_BRIGTHNESS;
	uint8_t buffer_set_blue_pwm_timer[DATA_LENGTH_PRIMARY] = BUFFER_SET_COLOR_BLUE_PWM_TIMER;
	uint8_t buffer_set_blue_pwm_flash[DATA_LENGTH_PRIMARY] = BUFFER_SET_COLOR_BLUE_PWM_FLASH;


	packet.address = KTD2026_DEVICE_ADDRESS;
	packet.data_length = DATA_LENGTH_PRIMARY;

	packet.data = buffer_set_blue;
	while ((i2c_master_write_packet_wait(&i2c_master_instance,
	&packet)) != STATUS_OK) {
	}

	packet.data = buffer_set_blue_high_brightness;
	while ((i2c_master_write_packet_wait(&i2c_master_instance,
	&packet)) != STATUS_OK) {
	}

	packet.data = buffer_set_blue_pwm_timer;
	while ((i2c_master_write_packet_wait(&i2c_master_instance,
	&packet)) != STATUS_OK) {
	}
	
	packet.data = buffer_set_blue_pwm_flash;
	while ((i2c_master_write_packet_wait(&i2c_master_instance,
	&packet)) != STATUS_OK) {
	}

}


void i2c_master_setup(void) {
  struct i2c_master_config config_i2c_master;
  i2c_master_get_config_defaults(&config_i2c_master);
  config_i2c_master.buffer_timeout = 10000;
  i2c_master_init(&i2c_master_instance, MASTER_MODULE, &config_i2c_master);
  i2c_master_enable(&i2c_master_instance);
}


/*


#include "led_driver.h"

struct i2c_master_module i2c_master_instance;
static struct i2c_master_packet packet = {KTD2026_DEVICE_ADDRESS, DATA_LENGTH_PRIMARY};

// Command buffer definitions
static const uint8_t command_buffers[][DATA_LENGTH_PRIMARY] = {
	[0] = {0x04, 0x00}, // BUFFER_TURN_OFF_MAIN_DIGITAL
	[1] = {0x02, 0x00}, // BUFFER_DISABLE_PWM1_TIMER
	[2] = {0x04, 0x04}, // BUFFER_SET_COLOR_GREEN
	[3] = {0x04, 0x01}, // BUFFER_SET_COLOR_RED
	[4] = {0x04, 0x10}, // BUFFER_SET_COLOR_BLUE
	[5] = {0x04, 0x05}, // BUFFER_SET_COLOR_YELLOW
	[6] = {0x04, 0x14}, // BUFFER_SET_COLOR_CYAN
	[7] = {0x04, 0x15}, // BUFFER_SET_COLOR_WHITE
	[8] = {0x04, 0x08}, // BUFFER_SET_COLOR_GREEN_PWM
	[9] = {0x01, 0x25}, // BUFFER_SET_CHARGE_ROUTINE_FLASH_PERIOD
	[10] = {0x05, 0xEF},// BUFFER_SET_CHARGE_ROUTINE_TRISE_TFALL
	[11] = {0x02, 0x1C},// BUFFER_SET_CHARGE_ROUTINE_PWM1_TIMER
	[12] = {0x04, 0x02},// BUFFER_SET_COLOR_RED_PWM
	[13] = {0x06, 0x05},// BUFFER_SET_COLOR_RED_BRIGHTNESS
	[14] = {0x01, 0x82},// BUFFER_SET_LOW_ROUTINE_FLASH_PERIOD
	[15] = {0x02, 0x32},// BUFFER_SET_LOW_ROUTINE_TRISE_TFALL
	[16] = {0x08, 0x03},// BUFFER_SET_COLOR_BLUE_LOWEST_BRIGTHNESS
	[17] = {0x08, 0x09},// BUFFER_SET_COLOR_BLUE_LOW_BRIGTHNESS
	[18] = {0x08, 0x15},// BUFFER_SET_COLOR_BLUE_HIGH_BRIGTHNESS
	[19] = {0x04, 0x20},// BUFFER_SET_COLOR_BLUE_PWM
	[20] = {0x02, 0x19},// BUFFER_SET_COLOR_BLUE_PWM_TIMER
	[21] = {0x01, 0x82} // BUFFER_SET_COLOR_BLUE_PWM_FLASH
};

// Common function to send I2C command
static void send_i2c_command(const uint8_t *command_buffer) {
	packet.data = (uint8_t *)command_buffer;
	while (i2c_master_write_packet_wait(&i2c_master_instance, &packet) != STATUS_OK) {
		// Optionally add a delay or error handling
	}
}

void set_color_indication(uint8_t color_index) {
	if (color_index < sizeof(command_buffers) / DATA_LENGTH_PRIMARY) {
		send_i2c_command(command_buffers[color_index]);
	}
}

void reset_chip(void) {
	send_i2c_command(command_buffers[0]); // BUFFER_TURN_OFF_MAIN_DIGITAL
}

void set_battery_charge_routine(void) {
	static const uint8_t routines[] = {
		8, 9, 10, 11 // Indices for the charge routine commands
	};
	for (size_t i = 0; i < sizeof(routines); ++i) {
		send_i2c_command(command_buffers[routines[i]]);
	}
}

void set_battery_low_routine(void) {
	static const uint8_t routines[] = {
		12, 14, 15, 13 // Indices for the low routine commands
	};
	for (size_t i = 0; i < sizeof(routines); ++i) {
		send_i2c_command(command_buffers[routines[i]]);
	}
}

void set_motor_speed_indication(uint8_t speed_index) {
	static const uint8_t speeds[][2] = {
		{19, 16}, // SPEED_1
		{19, 17}, // SPEED_2
		{19, 18}  // SPEED_3
	};
	if (speed_index < sizeof(speeds) / sizeof(speeds[0])) {
		send_i2c_command(command_buffers[speeds[speed_index][0]]);
		send_i2c_command(command_buffers[speeds[speed_index][1]]);
	}
}

void set_motor_pulsating_indication(void) {
	static const uint8_t pulsating_commands[] = {
		19, 18, 20, 21 // Indices for pulsating commands
	};
	for (size_t i = 0; i < sizeof(pulsating_commands); ++i) {
		send_i2c_command(command_buffers[pulsating_commands[i]]);
	}
}

void i2c_master_setup(void) {
	struct i2c_master_config config_i2c_master;
	i2c_master_get_config_defaults(&config_i2c_master);
	config_i2c_master.buffer_timeout = 10000;
	i2c_master_init(&i2c_master_instance, MASTER_MODULE, &config_i2c_master);
	i2c_master_enable(&i2c_master_instance);
}

*/