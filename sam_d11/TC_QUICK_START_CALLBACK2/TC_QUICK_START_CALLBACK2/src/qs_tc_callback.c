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



struct tc_module tc_instance;


void configure_tc(void);
void configure_tc_callbacks(void);
void tc_callback_to_change_duty_cycle(
		struct tc_module *const module_inst);

void tc_callback_to_change_duty_cycle(struct tc_module *const module_inst)
{
	LED_On(LED0_PIN);
}


void configure_tc(void)
{
	struct tc_config config_tc;
	
	tc_get_config_defaults(&config_tc);
	
	config_tc.clock_source = GCLK_GENERATOR_1;
	config_tc.counter_size = TC_COUNTER_SIZE_8BIT;
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV64;
	config_tc.counter_8_bit.period = 100;
	
	
	tc_init(&tc_instance, TC1, &config_tc);
	tc_enable(&tc_instance);
	
	
 //struct tc_config config_tc;
 //tc_get_config_defaults(&config_tc);
 //config_tc.counter_size = TC_COUNTER_SIZE_16BIT;
 //config_tc.wave_generation = TC_WAVE_GENERATION_NORMAL_FREQ
 //onfig_tc.counter_16_bit.compare_capture_channel[0] = 0xFFFF;
 //config_tc.pwm_channel[0].enabled = true;
 //config_tc.pwm_channel[0].pin_out = PWM_OUT_PIN;
 //config_tc.pwm_channel[0].pin_mux = PWM_OUT_MUX;
 //tc_init(&tc_instance, PWM_MODULE, &config_tc);
 //tc_enable(&tc_instance);	
	
	
	
	
}

void configure_tc_callbacks(void)
{

	tc_register_callback(
			&tc_instance,
			tc_callback_to_change_duty_cycle,
			TC_CALLBACK_OVERFLOW);

	tc_enable_callback(&tc_instance, TC_CALLBACK_OVERFLOW);
}

int main(void)
{
	system_init();

	configure_tc();
	configure_tc_callbacks();
	system_interrupt_enable_global();
	while (true) {
	}
}
