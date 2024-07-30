//#include <asf.h>
//
//
//// power mgmt
//
//
//
//void configure_extint_channel(void);
//void configure_extint_callbacks(void);
//void extint_detection_callback(void);
//
//
//#define BUTTON_0_EIC_LINE_custom		1
//
//
//void configure_extint_channel(void)
//{
//
	//struct extint_chan_conf config_extint_chan;
	//extint_chan_get_config_defaults(&config_extint_chan);
//
	//config_extint_chan.gpio_pin           = PIN_PA15A_EIC_EXTINT1;
	//config_extint_chan.gpio_pin_mux       = MUX_PA15A_EIC_EXTINT1;
	//config_extint_chan.wake_if_sleeping   = true;
//
	//config_extint_chan.gpio_pin_pull      = EXTINT_PULL_UP;
	//config_extint_chan.detection_criteria = EXTINT_DETECT_BOTH;
	//extint_chan_set_config(BUTTON_0_EIC_LINE_custom, &config_extint_chan);
//
//}
//
//void configure_extint_callbacks(void)
//{
	//extint_register_callback(extint_detection_callback,
	//BUTTON_0_EIC_LINE_custom,
	//EXTINT_CALLBACK_TYPE_DETECT);
//
	//extint_chan_enable_callback(BUTTON_0_EIC_LINE_custom,
	//EXTINT_CALLBACK_TYPE_DETECT);
//}
//
//
//
//void extint_detection_callback(void)
//{
	//bool pin_state = port_pin_get_input_level(BUTTON_0_PIN);
	//port_pin_set_output_level(LED_0_PIN, pin_state);
//}
//
//
//int main(void)
//{
	//system_init();
	//configure_extint_channel();
	//configure_extint_callbacks();
	//system_interrupt_enable_global();
	//system_set_sleepmode(SYSTEM_SLEEPMODE_STANDBY);
	//system_sleep();
	//while (true) {
		////port_pin_set_output_level(LED_0_PIN, false);	
	//}
//}
