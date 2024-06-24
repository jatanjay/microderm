/**
 * \file
 *
 * \brief SAM TC - Timer Counter Callback Driver Quick Start
 *
 * Copyright (c) 2013-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#include <conf_quick_start_callback.h>
#include <asf.h>


void configure_tc(void);
void configure_tc_callbacks(void);
void tc_callback_to_change_duty_cycle(
		struct tc_module *const module_inst);


void configure_extint_channel(void);
void configure_extint_callbacks(void);
void extint_detection_callback(void);

#define PWM_PERIOD 0x210 // DEC 528
#define CCx ((int)lround(PWM_PERIOD / 2.0))

// Global Variables
static bool pwm_started = false;
static uint32_t pwm_callback_delay = 0;
static bool pin_state = false;


//! [module_inst]
struct tc_module tc_instance;
//! [module_inst]

//! [callback_funcs]
void tc_callback_to_change_duty_cycle(struct tc_module *const module_inst)
{
	static uint16_t duty_cycle = 0;
	duty_cycle += 128; // Adjust duty cycle increment as needed
	tc_set_compare_value(module_inst, TC_COMPARE_CAPTURE_CHANNEL_0, duty_cycle);
}

//! [callback_funcs]

//! [setup]
void configure_tc(void)
{
	//! [setup_config]
	struct tc_config config_tc;
	//! [setup_config]
	//! [setup_config_defaults]
	tc_get_config_defaults(&config_tc);
	//! [setup_config_defaults]

	//! [setup_change_config]
	config_tc.counter_size    = TC_COUNTER_SIZE_16BIT;
	config_tc.wave_generation = TC_WAVE_GENERATION_NORMAL_PWM;
	config_tc.counter_16_bit.compare_capture_channel[0] = 0xFFFF;
	//! [setup_change_config]

	//! [setup_change_config_pwm]
	config_tc.pwm_channel[0].enabled = true;
	config_tc.pwm_channel[0].pin_out = PWM_OUT_PIN;
	config_tc.pwm_channel[0].pin_mux = PWM_OUT_MUX;
	//! [setup_change_config_pwm]

	//! [setup_set_config]
	tc_init(&tc_instance, PWM_MODULE, &config_tc);
	//! [setup_set_config]

	//! [setup_enable]
	tc_enable(&tc_instance);
	//! [setup_enable]
}

void configure_tc_callbacks(void)
{
	//! [setup_register_callback]
	tc_register_callback(
			&tc_instance,
			tc_callback_to_change_duty_cycle,
			TC_CALLBACK_CC_CHANNEL0);
	//! [setup_register_callback]

	//! [setup_enable_callback]
	tc_enable_callback(&tc_instance, TC_CALLBACK_CC_CHANNEL0);
	//! [setup_enable_callback]
}
//! [setup]


// Configure EXTINT channel for button press detection
void configure_extint_channel(void) {
	struct extint_chan_conf config_extint_chan;
	extint_chan_get_config_defaults( & config_extint_chan);
	config_extint_chan.gpio_pin = BUTTON_0_EIC_PIN;
	config_extint_chan.gpio_pin_mux = BUTTON_0_EIC_MUX;
	config_extint_chan.gpio_pin_pull = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria = EXTINT_DETECT_BOTH;
	extint_chan_set_config(BUTTON_0_EIC_LINE, & config_extint_chan);
}

// Configure EXTINT callbacks for button press events
void configure_extint_callbacks(void) {
	extint_register_callback(extint_detection_callback,
	BUTTON_0_EIC_LINE,
	EXTINT_CALLBACK_TYPE_DETECT);
	extint_chan_enable_callback(BUTTON_0_EIC_LINE,
	EXTINT_CALLBACK_TYPE_DETECT);
}


void extint_detection_callback(void) {
	bool pin_state = port_pin_get_input_level(BUTTON_0_PIN);
	pwm_started = pin_state;
	port_pin_set_output_level(LED_0_PIN, false);
	static uint32_t delay = 0;
	delay = (delay + CCx) & 0xFFFF;
	if (pin_state) {
		pwm_callback_delay = delay;
	}
}

int main(void)
{
	system_init();
	configure_tc();
	configure_tc_callbacks();
	configure_extint_channel();
	configure_extint_callbacks();
	system_interrupt_enable_global();
	while (true) {	
	
	}
}
