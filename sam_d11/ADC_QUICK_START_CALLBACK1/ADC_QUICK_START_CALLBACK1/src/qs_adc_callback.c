
#include <asf.h>

void configure_port_pins(void)
{
	struct port_config config_port_pin;
	port_get_config_defaults(&config_port_pin);
	
	config_port_pin.direction  = PORT_PIN_DIR_INPUT;
	config_port_pin.input_pull = PORT_PIN_PULL_DOWN;
	port_pin_set_config(PIN_PA27, &config_port_pin);
	
}





int main(void)
{
	system_init();
	system_interrupt_enable_global();
	configure_port_pins();
	
    while (true) {
	    bool pin_state = port_pin_get_input_level(PIN_PA27);
	    if (pin_state){
		    LED_On(LED0_PIN);
		    }else{
		    LED_Off(LED0_PIN);
	    }
    }
}
