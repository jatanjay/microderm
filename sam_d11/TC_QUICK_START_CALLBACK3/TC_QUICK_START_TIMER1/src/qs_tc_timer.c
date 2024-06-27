#include <asf.h>
#include "conf_qs_tc_timer.h"





struct tc_module tc_instance;
bool tick_flag_10ms;

void configure_tc(void);
void configure_tc_callbacks(void);
void tc_callback_to_toggle_led(
struct tc_module *const module_inst);

void tc_callback_to_toggle_led(
		struct tc_module *const module_inst)
{
		
	static int tick_count_1ms;
	static int tick_count_10ms;
	tick_count_1ms++;
	if(tick_count_1ms>10){
		tick_count_10ms++;
		tick_count_1ms = 0;
		tick_flag_10ms = true;
		port_pin_toggle_output_level(LED0_PIN);
	}
	
}

void configure_tc(void)
{
	struct tc_config config_tc;
	tc_get_config_defaults(&config_tc);
	config_tc.counter_size = TC_COUNTER_SIZE_8BIT;
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV64;
	config_tc.counter_8_bit.period = 124;
	tc_init(&tc_instance, CONF_TC_MODULE, &config_tc);
	tc_enable(&tc_instance);
}

void configure_tc_callbacks(void)
{
	tc_register_callback(&tc_instance, tc_callback_to_toggle_led, TC_CALLBACK_OVERFLOW);
	tc_enable_callback(&tc_instance, TC_CALLBACK_OVERFLOW);
}


int main(void)
{
	system_init();
	configure_tc();
	configure_tc_callbacks();
	system_interrupt_enable_global();
	while (true) {
		if(tick_flag_10ms)
		{
			tick_flag_10ms = false;
		}
	}
}
