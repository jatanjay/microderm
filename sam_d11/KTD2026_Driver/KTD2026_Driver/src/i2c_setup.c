/*
 * i2c_setup.c
 *
 * Created: 7/3/2024 1:18:34 PM
 *  Author: jatan
 */ 



#include "asf.h"



#define MASTER_MODULE SERCOM2


/************************************************************************/
/* I2C config, setup and callback functions. To be used by KTD2026 driver */
/************************************************************************/



struct i2c_master_module i2c_master_instance;



void i2c_master_setup(void){
	struct i2c_master_config config_i2c_master;
	i2c_master_get_config_defaults(config_i2c_master);
	
	/* Change buffer timeout to something longer. */
	config_i2c_master.buffer_timeout = 10000;
	
	i2c_master_init(&i2c_master_instance, MASTER_MODULE, &config_i2c_master);
	i2c_master_enable(&i2c_master_instance);
	
}



