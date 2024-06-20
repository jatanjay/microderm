
#include <asf.h>

#include <system_interrupt.h>

void i2c_write_complete_callback(
  struct i2c_master_module *
  const module);
void configure_i2c(void);
void configure_i2c_callbacks(void);

#define DATA_LENGTH 16

static uint8_t wr_buffer[DATA_LENGTH];

static uint8_t rd_buffer[DATA_LENGTH];

#define SLAVE_ADDRESS 0x77

struct i2c_master_packet wr_packet;
struct i2c_master_packet rd_packet;


struct i2c_master_module i2c_master_instance;



void i2c_write_complete_callback(
  struct i2c_master_module *
  const module) {

}

void i2c_read_complete_callback(
  struct i2c_master_module *
  const module) {
}

void configure_i2c(void) {

  struct i2c_master_config config_i2c_master;
  i2c_master_get_config_defaults( & config_i2c_master);

  config_i2c_master.buffer_timeout = 65535;
  #if SAMR30
  config_i2c_master.pinmux_pad0 = CONF_MASTER_SDA_PINMUX;
  config_i2c_master.pinmux_pad1 = CONF_MASTER_SCK_PINMUX;
  #endif


  while (i2c_master_init( & i2c_master_instance, CONF_I2C_MASTER_MODULE, & config_i2c_master) !=
    STATUS_OK);

  i2c_master_enable( & i2c_master_instance);

}



void configure_i2c_callbacks(void) {

  i2c_master_register_callback( & i2c_master_instance, i2c_write_complete_callback,
    I2C_MASTER_CALLBACK_WRITE_COMPLETE);

  i2c_master_register_callback( & i2c_master_instance, i2c_read_complete_callback,
    I2C_MASTER_CALLBACK_READ_COMPLETE);
	
  i2c_master_enable_callback( & i2c_master_instance,
    I2C_MASTER_CALLBACK_WRITE_COMPLETE);

}


int main(void) {
  system_init();
  configure_i2c();
  configure_i2c_callbacks();

  wr_packet.address = SLAVE_ADDRESS;
  wr_packet.data_length = 1;
  wr_packet.data = wr_buffer;
  rd_packet.address = SLAVE_ADDRESS;
  //rd_packet.data_length = 2;
  rd_packet.data = rd_buffer;


  while (true) {

    if (!port_pin_get_input_level(BUTTON_0_PIN)) {
      while (!port_pin_get_input_level(BUTTON_0_PIN)) {}

      //wr_buffer[0] = 0xAA;
      //i2c_master_write_packet_job( & i2c_master_instance, & wr_packet);
      //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
      //rd_packet.data_length = 2;
      //i2c_master_read_packet_job( & i2c_master_instance, & rd_packet);
      //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
	  //
      //wr_buffer[0] = 0xAC;
      //i2c_master_write_packet_job( & i2c_master_instance, & wr_packet);
      //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
      //rd_packet.data_length = 2;
      //i2c_master_read_packet_job( & i2c_master_instance, & rd_packet);
      //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
//
      //wr_buffer[0] = 0xAE;
      //i2c_master_write_packet_job( & i2c_master_instance, & wr_packet);
      //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
      //rd_packet.data_length = 2;
      //i2c_master_read_packet_job( & i2c_master_instance, & rd_packet);
      //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
//
      //wr_buffer[0] = 0xB0;
      //i2c_master_write_packet_job( & i2c_master_instance, & wr_packet);
      //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
      //rd_packet.data_length = 2;
      //i2c_master_read_packet_job( & i2c_master_instance, & rd_packet);
      //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
//
      //wr_buffer[0] = 0xB2;
      //i2c_master_write_packet_job( & i2c_master_instance, & wr_packet);
      //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
      //rd_packet.data_length = 2;
      //i2c_master_read_packet_job( & i2c_master_instance, & rd_packet);
      //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
//
      //wr_buffer[0] = 0xB4;
      //i2c_master_write_packet_job( & i2c_master_instance, & wr_packet);
      //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
      //rd_packet.data_length = 2;
      //i2c_master_read_packet_job( & i2c_master_instance, & rd_packet);
      //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
//
      //wr_buffer[0] = 0xB6;
      //i2c_master_write_packet_job( & i2c_master_instance, & wr_packet);
      //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
      //rd_packet.data_length = 2;
      //i2c_master_read_packet_job( & i2c_master_instance, & rd_packet);
      //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
//
      //wr_buffer[0] = 0xB8;
      //i2c_master_write_packet_job( & i2c_master_instance, & wr_packet);
      //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
      //rd_packet.data_length = 2;
      //i2c_master_read_packet_job( & i2c_master_instance, & rd_packet);
      //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
//
      //wr_buffer[0] = 0xBA;
      //i2c_master_write_packet_job( & i2c_master_instance, & wr_packet);
      //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
      //rd_packet.data_length = 2;
      //i2c_master_read_packet_job( & i2c_master_instance, & rd_packet);
      //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
//
      //wr_buffer[0] = 0xBC;
      //i2c_master_write_packet_job( & i2c_master_instance, & wr_packet);
      //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
      //rd_packet.data_length = 2;
      //i2c_master_read_packet_job( & i2c_master_instance, & rd_packet);
      //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);

      wr_buffer[0] = 0xF4;
      i2c_master_write_packet_job( & i2c_master_instance, & wr_packet);
      while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
      rd_packet.data_length = 2;
      i2c_master_read_packet_job( & i2c_master_instance, & rd_packet);
      while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
	  
	  
	  wr_buffer[0] = 0x2E;
	  i2c_master_write_packet_job( & i2c_master_instance, & wr_packet);
	  while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
	  rd_packet.data_length = 2;
	  i2c_master_read_packet_job( & i2c_master_instance, & rd_packet);
	  while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
	  
	  delay_us(100);
	  
	  wr_buffer[0] = 0xF6;
	  i2c_master_write_packet_job( & i2c_master_instance, & wr_packet);
	  while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
	  rd_packet.data_length = 2;
	  i2c_master_read_packet_job( & i2c_master_instance, & rd_packet);
	  while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
	  
	  //
	  //wr_buffer[0] = 0xF7;
	  //i2c_master_write_packet_job( & i2c_master_instance, & wr_packet);
	  //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
	  //rd_packet.data_length = 2;
	  //i2c_master_read_packet_job( & i2c_master_instance, & rd_packet);
	  //while (i2c_master_get_job_status( & i2c_master_instance) != STATUS_OK);
	  //
	  
    }
  }

}