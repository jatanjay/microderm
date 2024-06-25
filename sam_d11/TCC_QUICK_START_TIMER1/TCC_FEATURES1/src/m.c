///*
 //* m.c
 //*
 //* Created: 6/25/2024 10:31:24 AM
 //*  Author: jatan
 //*/ 
///**
 //* \file
 //*
 //* \brief SAM D11 TCC Features Example Application
 //*        Refer following application note for details.
 //*        AT07690 - Using the Timer Counter for Control Applications (TCC).
 //*
 //* Copyright (c) 2016-2018 Microchip Technology Inc. and its subsidiaries.
 //*
 //* \license
 //* \asf_license_start
 //*
 //* \page License
 //*
 //* Subject to your compliance with these terms, you may use Microchip
 //* software and any derivatives exclusively with Microchip products.
 //* It is your responsibility to comply with third party license terms applicable
 //* to your use of third party software (including open source software) that
 //* may accompany Microchip software.
 //*
 //* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 //* WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 //* INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 //* AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 //* LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 //* LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 //* SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 //* POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 //* ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 //* RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 //* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 //*
 //* \asf_license_stop
 //*
 //*/
//
///*
 //* Include header files for all drivers that have been imported from
 //* Atmel Software Framework (ASF).
 //*/
///*
 //* Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 //*/
//#include <asf.h>
//#include "string.h"
//#include "conf_example.h"
//
//struct tcc_module tcc_instance;
//enum status_code stat = STATUS_OK;
//
//void configure_extint_callbacks(void);
//void configure_extint_channel(void);
//void extint_detection_callback(void);
//
//
//volatile bool pwm_enabled = false;
//
//void configure_extint_channel(void) {
	//struct extint_chan_conf config_extint_chan;
	//extint_chan_get_config_defaults( & config_extint_chan);
	//config_extint_chan.gpio_pin = BUTTON_0_EIC_PIN;
	//config_extint_chan.gpio_pin_mux = BUTTON_0_EIC_MUX;
	//config_extint_chan.gpio_pin_pull = EXTINT_PULL_UP;
	//config_extint_chan.detection_criteria = EXTINT_DETECT_BOTH;
	//extint_chan_set_config(BUTTON_0_EIC_LINE, & config_extint_chan);
//}
//
//
//void extint_detection_callback(void) {
	//bool pin_state = port_pin_get_input_level(BUTTON_0_PIN);
	//pwm_enabled = pin_state;
	//port_pin_set_output_level(LED_0_PIN, true);
//}
//
//void configure_extint_callbacks(void) {
	//extint_register_callback(extint_detection_callback,
	//BUTTON_0_EIC_LINE,
	//EXTINT_CALLBACK_TYPE_DETECT);
	//extint_chan_enable_callback(BUTTON_0_EIC_LINE,
	//EXTINT_CALLBACK_TYPE_DETECT);
//}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//#ifdef TCC_MODE_PATTERN_GENERATION
///* Generic Pattern for half size Bipolar Stepper Motor */
//uint8_t sm_pattern[PATTERN_SIZE] = {8, 2, 4, 1};
//uint8_t iIndex = 0;
//#endif
//
//#ifdef TCC_MODE_FAULT 
////! [events_resource]
//struct events_resource event_resource;
//#endif
//
//#ifdef TCC_MODE_COUNTER
////counter callback to toggle LED.
//static void tcc_callback_to_toggle_led(struct tcc_module *const module_inst);
//#endif
//
//#ifdef  TCC_MODE_FAULT 
//// configure event, for event trigger.
//static void configure_event(void);
//#endif
//
//#ifdef TCC_MODE_FAULT
//static void configure_eic(void);
//static void eic_callback_to_fault_detect(void);
//#endif
//
//#ifdef  TCC_MODE_CAPTURE
//void configure_usart(void);
//void configu_eic(void);
//void configu_tcc(void);
//void configure_evsys (void);
//#endif
//
////////////////////// END OF FUNCTION DECLARATION ////////////////////////////
///* 
 //* Description: This example project helps you to configure the TCC Module
 //* for different Features/Modes available in TCC module of SAMD11 
 //*/
  //
///* Configure the TCC module as per the application requirement */
//
///* Example -1.  Circular Buffer
			//2.  Oneshot Operation 
			//3.  Pattern Generation
			//4.  PWM with OTMX(Output Matrix) and DTI(Dead Time Insertion) 
			//5.  RAMP2 Operation
			//6.  SWAP Operation
			//----Added for Revision 2 of TCC App Note!!!.
			//7.  RAMP2A Operation
			//8.  Dual Slope Operation
			//9.  DITHERING
			//10. FAULT Operation
			//11. CAPTURE Operation
			//12. COUNTER Operation
//*/
//
///*
 //* PWM Waveforms are generated for most of the features based on the feature
 //* selected by user.The waveform output will match the expected waveform 
 //* desired for the feature chosen by the user.Below are some special features
 //* which needs special mention in which the feature is demonstrated based on
 //* other characteristics like LED state/ USART output. 
//
 //* 1. FAULT Operation - On a button press toggle the Fault Line is toggled to 
 //* trigger/clear fault.The LED state indicates the Fault is available or 
 //* cleared.
 //* 2. COUNTER Operation -> The LED toggles at different speed to indicate 
 //* different triggered events based on different counter values. For counter
 //* there won't be any waveform generation.
 //* 3. CAPTURE Operation -> This captures a fixed waveform and displays the 
 //* captured waveform results to indicate the different period captured is 
 //* one of the period values of the PWM waveform. The captured period values are
 //* displayed using USART of 1152008N1 serial configuration 
 //*/
//
//
///* 
 //* Function to configure usart, eic,tcc and event system. 
 //* configures the usart with CONF_BAUD_RATE baud rate.
 //*/
//struct usart_module usart_instances;
//struct tcc_module tcc_instances;
//struct events_resource event_resources;
//
//uint32_t period, pulse_width;
//
//
///* 
 //* Function to configure TCC, this will configure TCC for other than Capture,
 //* Counter and Fault mode related operations.
 //*/
//void configure_tcc(void)
//{
//// Structure used to store the TCC configuration parameters
	//struct tcc_config config_tcc;
//
//// Fill the Structure with the default values
	//tcc_get_config_defaults(&config_tcc, CONF_PWM_MODULE);
	//config_tcc.compare.match[TCC_MATCH_CAPTURE_CHANNEL_0]   = CONF_DEFAULT_MATCH_COMPARE;
	//config_tcc.counter.period                               = CONF_DEFAULT_PERIOD;
//
//#ifdef TCC_MODE_PATTERN_GENERATION
	//config_tcc.counter.period                               = TCC_PERIOD_VALUE;
//// Configure the TCC Waveform Output pins for waveform generation output
	//config_tcc.pins.enable_wave_out_pin[TCC_CHANNEL_NUM_0]  = true;
	//config_tcc.pins.enable_wave_out_pin[TCC_CHANNEL_NUM_1]  = true;
	//config_tcc.pins.enable_wave_out_pin[TCC_CHANNEL_NUM_2]  = true;
	//config_tcc.pins.enable_wave_out_pin[TCC_CHANNEL_NUM_3]  = true;
	//config_tcc.pins.wave_out_pin[TCC_CHANNEL_NUM_0]         = PIN_PA04F_TCC0_WO0;
	//config_tcc.pins.wave_out_pin[TCC_CHANNEL_NUM_1]         = PIN_PA05F_TCC0_WO1;
	//config_tcc.pins.wave_out_pin[TCC_CHANNEL_NUM_2] = PIN_PA06F_TCC0_WO2;
	//config_tcc.pins.wave_out_pin[TCC_CHANNEL_NUM_3] = PIN_PA07F_TCC0_WO3;
//// Configure the Alternate function of GPIO pins for TCC functionality
	//config_tcc.pins.wave_out_pin_mux[TCC_CHANNEL_NUM_0]     = MUX_PA04F_TCC0_WO0;
	//config_tcc.pins.wave_out_pin_mux[TCC_CHANNEL_NUM_1]     = MUX_PA05F_TCC0_WO1;
	//config_tcc.pins.wave_out_pin_mux[TCC_CHANNEL_NUM_2] = MUX_PA06F_TCC0_WO2;
	//config_tcc.pins.wave_out_pin_mux[TCC_CHANNEL_NUM_3] = MUX_PA07F_TCC0_WO3;
	//config_tcc.double_buffering_enabled                     = true;
///* 
 //* Configure the compare channel values for the duty cycle control 
 //* Load the 0x7FFF value for 50% duty cycle
 //*/
	//config_tcc.compare.match[TCC_MATCH_CAPTURE_CHANNEL_0]   = TCC_MATCH_VALUE_PATTERN_GEN;
//#endif
//
//// The main configuration for PATTERN GENERATION
//#ifdef TCC_MODE_PATTERN_GENERATION
///* 
 //* Configure the Output Matrix Channel for Pattern Generation of 
 //* Stepper Motor 
 //*/
	//CONF_PWM_MODULE->WEXCTRL.reg |= TCC_WEXCTRL_OTMX(2);
///* 
 //* Enable the Pattern Generator Output for 4 Waveform Outputs and 
 //* Load the PATT and PATTB register values respectively for Stepper Motor 
 //* Pattern Generation 
 //*/
	//CONF_PWM_MODULE->PATT.reg = TCC_PATT_PGE(TCC_PATTERN_PAGE_VAL) | TCC_PATT_PGV(sm_pattern[iIndex++]);
	//while (CONF_PWM_MODULE->SYNCBUSY.bit.PATT);
	//CONF_PWM_MODULE->PATTB.reg = TCC_PATTB_PGEB(TCC_PATTERN_PAGE_VAL) | TCC_PATTB_PGVB(sm_pattern[iIndex++]);
	//while (CONF_PWM_MODULE->SYNCBUSY.reg & (1u << 16));
//#endif
//
///*
 //* This Function loads the Stepper Motor Pattern into the Pattern Buffer 
 //* register respectively in the PGVB continuously. Please note that here 
 //* we have not added any delay between the pattern on the wave output pin. 
 //* Hence please add the appropriate delay before load the pattern into 
 //* Pattern Generation Value Buffer (PGVB register) as mentioned 
 //* in the stepper motor  datasheet
 //*/
//
//#ifdef TCC_MODE_PATTERN_GENERATION
//void pattern_generation(void)
//{
	//if (iIndex == PATTERN_SIZE) {
		//iIndex = 0;
	//}
	//while (!(TCC0->INTFLAG.bit.MC0)); 
	//CONF_PWM_MODULE->INTFLAG.reg |= TCC_INTFLAG_MC0; //Equivalent to TCC0->INTFLAG.bit.MC0 = 1; 
	//CONF_PWM_MODULE->PATTB.reg = TCC_PATTB_PGEB(TCC_PATTERN_PAGE_VAL) | TCC_PATTB_PGVB(sm_pattern[iIndex++]);
	//while (CONF_PWM_MODULE->SYNCBUSY.reg & (1u << 16));
//}
//#endif
//
//void configure_tcc_callback(void)
//{
//
	////! [setup_register_callback]
	//tcc_register_callback(&tcc_instance, tcc_callback_to_toggle_led, TCC_CALLBACK_OVERFLOW);
	//tcc_register_callback(&tcc_instance, tcc_callback_to_toggle_led, TCC_CALLBACK_CHANNEL_0);
	//tcc_register_callback(&tcc_instance, tcc_callback_to_toggle_led, TCC_CALLBACK_CHANNEL_1);
	//tcc_register_callback(&tcc_instance, tcc_callback_to_toggle_led, TCC_CALLBACK_CHANNEL_2);
	//tcc_register_callback(&tcc_instance, tcc_callback_to_toggle_led, TCC_CALLBACK_CHANNEL_3);
	////! [setup_enable_callback]
	//tcc_enable_callback(&tcc_instance, TCC_CALLBACK_OVERFLOW);
	//tcc_enable_callback(&tcc_instance, TCC_CALLBACK_CHANNEL_0);
	//tcc_enable_callback(&tcc_instance, TCC_CALLBACK_CHANNEL_1);
	//tcc_enable_callback(&tcc_instance, TCC_CALLBACK_CHANNEL_2);
	//tcc_enable_callback(&tcc_instance, TCC_CALLBACK_CHANNEL_3);
	////! [setup_change_events_faults]
//}
//
//
//int main (void)
//{
//// system initialization - includes, clock and board initialization.
	//system_init();
	//system_interrupt_enable_global();
	//configure_tcc();
	//configure_tcc_callback();
	//while (1){
		//pattern_generation();
//}