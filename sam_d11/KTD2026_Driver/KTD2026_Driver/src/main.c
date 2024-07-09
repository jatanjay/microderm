#include <asf.h>

#define MASTER_MODULE SERCOM2
#define KTD2026_DEVICE_ADDRESS			0x30		// pg. 17 of Data sheet
#define DATA_LENGTH_PRIMARY				2
#define DATA_LENGTH_SECONDARY			8

struct i2c_master_module i2c_master_instance;

/*

// WriteBuffer_FOR_JOB_X = {0xREGISTER_TO_BE_WRITTEN, 0xDATA}
first, prepare packet. 

for that,
	
struct i2c_master_packet PACKET_TO_JOB_X = {
	.address     = KTD2026_DEVICE_ADDRESS,
	.DATA_LENGTH_PRIMARY = DATA_LENGTH_PRIMARY_FOR_JOB_X,
	.data        = WriteBuffer_FOR_JOB_X,
	.ten_bit_address = false,
	.high_speed      = false,
	.hs_master_code  = 0x0,
};

for that do the following
	
REGISTER_TO_BE_WRITTEN_TO = X;
static unint8_t DATA_LENGTH_PRIMARY = X;
static unint8_t WRITE_BUFFER[DATA_LENGTH_PRIMARY] = {REGISTER_TO_BE_WRITTEN_TO, DATA}

	
*/	
	
	
static uint8_t buffer_reset_chip[DATA_LENGTH_PRIMARY]						=	{0x00,0x05};
static uint8_t buffer_set_color_green[DATA_LENGTH_PRIMARY]					=	{0x04,0x04};
static uint8_t buffer_set_color_red[DATA_LENGTH_PRIMARY]					=	{0x04,0x01};
static uint8_t buffer_set_color_blue[DATA_LENGTH_PRIMARY]					=	{0x04,0x10};
static uint8_t buffer_set_color_yellow[DATA_LENGTH_PRIMARY]					=	{0x04,0x05};
static uint8_t buffer_set_color_purple[DATA_LENGTH_PRIMARY]					=	{0x04,0x11};
static uint8_t buffer_set_color_cyan[DATA_LENGTH_PRIMARY]					=	{0x04,0x14};
static uint8_t buffer_set_color_white[DATA_LENGTH_PRIMARY]					=	{0x04,0x15};
static uint8_t buffer_set_color_green_PWM[DATA_LENGTH_PRIMARY]				=	{0x04,0x08};
static uint8_t buffer_set_color_red_PWM[DATA_LENGTH_PRIMARY]				=	{0x04,0x02};
static uint8_t buffer_set_charge_routine_flash_period[DATA_LENGTH_PRIMARY]	=	{0x01,0x10};
static uint8_t buffer_set_charge_routine_trise_tfall[DATA_LENGTH_PRIMARY]	=	{0x05,0xF0};
static uint8_t buffer_set_low_routine_flash_period[DATA_LENGTH_PRIMARY]		=	{0x05,0x21};
static uint8_t buffer_set_low_routine_trise_tfall[DATA_LENGTH_PRIMARY]		=	{0x02,0x2D};


//#ifdef SCALE_UP
	//static uint8_t buffer_set_color_green_intensity[DATA_LENGTH_PRIMARY]		=	{0x07,0x5F + SCALE_INTENSITY_CHANGE};																					// g
	//static uint8_t buffer_set_color_red_intensity[DATA_LENGTH_PRIMARY]			=	{0x06,0x5F + SCALE_INTENSITY_CHANGE};																					// r
	//static uint8_t buffer_set_color_blue_intensity[DATA_LENGTH_PRIMARY]			=	{0x08,0x5F + SCALE_INTENSITY_CHANGE};																					// b
	//
	//static uint8_t buffer_set_color_yellow_intensity[DATA_LENGTH_SECONDARY]		=	{0x06,0x5F + SCALE_INTENSITY_CHANGE, 0x07, 0x5F + SCALE_INTENSITY_CHANGE};												// r + g = yellow
	//static uint8_t buffer_set_color_purple_intensity[DATA_LENGTH_SECONDARY]		=	{0x08,0x5F + SCALE_INTENSITY_CHANGE, 0x06, 0x5F + SCALE_INTENSITY_CHANGE};												// b + r = purple
	//static uint8_t buffer_set_color_cyan_intensity[DATA_LENGTH_SECONDARY]		=	{0x07,0x5F + SCALE_INTENSITY_CHANGE, 0x08, 0x5F + SCALE_INTENSITY_CHANGE};												// g + b = cyan
	////static uint8_t buffer_set_color_white_intensity[DATA_LENGTH_SECONDARY]		=	{0x06,0x5F + SCALE_INTENSITY_CHANGE, 0x07, 0x5F + SCALE_INTENSITY_CHANGE, 0x08, 0x5F + SCALE_INTENSITY_CHANGE};			// r + g + b = w
//#else
	//static uint8_t buffer_set_color_green_intensity[DATA_LENGTH_PRIMARY]		=	{0x07,0x5F - SCALE_INTENSITY_CHANGE};
	//static uint8_t buffer_set_color_red_intensity[DATA_LENGTH_PRIMARY]			=	{0x07,0x5F - SCALE_INTENSITY_CHANGE};
	//static uint8_t buffer_set_color_blue_intensity[DATA_LENGTH_PRIMARY]			=	{0x08,0x5F - SCALE_INTENSITY_CHANGE};
	//
	//static uint8_t buffer_set_color_yellow_intensity[DATA_LENGTH_SECONDARY]		=	{0x06,0x5F - SCALE_INTENSITY_CHANGE, 0x07, 0x5F - SCALE_INTENSITY_CHANGE};
	//static uint8_t buffer_set_color_purple_intensity[DATA_LENGTH_SECONDARY]		=	{0x06,0x5F - SCALE_INTENSITY_CHANGE, 0x07, 0x5F - SCALE_INTENSITY_CHANGE};
	//static uint8_t buffer_set_color_cyan_intensity[DATA_LENGTH_SECONDARY]		=	{0x06,0x5F - SCALE_INTENSITY_CHANGE, 0x07, 0x5F - SCALE_INTENSITY_CHANGE};
	////static uint8_t buffer_set_color_white_intensity[DATA_LENGTH_SECONDARY]		=	{0x06,0x5F - SCALE_INTENSITY_CHANGE, 0x07, 0x5F - SCALE_INTENSITY_CHANGE, 0x07, 0x5F - SCALE_INTENSITY_CHANGE};
//#endif



void set_color_green(void);
void set_color_red(void);
void set_color_blue(void);
void set_color_yellow(void);
void set_color_purple(void);
void set_color_cyan(void);
void set_color_white(void);
void set_green(void);
void set_red(void);
void set_blue(void);
void set_yellow(void);
void set_purple(void);
void set_cyan(void);
void set_white(void);
void reset_chip(void);
void set_battery_charge_routine(void);
void set_battery_low_routine(void);
void i2c_master_setup(void);



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
	struct i2c_master_packet packet = {
		.address = KTD2026_DEVICE_ADDRESS,
		.data_length = DATA_LENGTH_PRIMARY,
		.data = buffer_reset_chip,
	};
	
	while ((i2c_master_write_packet_wait(&i2c_master_instance, &packet)) != STATUS_OK)
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


/************************************************************************/
/* I2C config, setup and callback functions. To be used by KTD2026 driver */
/************************************************************************/




void i2c_master_setup(void){
	struct i2c_master_config config_i2c_master;
	i2c_master_get_config_defaults(&config_i2c_master);
	config_i2c_master.buffer_timeout = 10000;
	i2c_master_init(&i2c_master_instance, MASTER_MODULE, &config_i2c_master);
	i2c_master_enable(&i2c_master_instance);
	
}




int main (void)
{
	system_init();
	
	

	while (1) {

	}
	return 0;
}
