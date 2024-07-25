#ifndef CONF_EXAMPLE_H_INCLUDED
#define CONF_EXAMPLE_H_INCLUDED

enum eTCC_Channel {
	TCC_CHANNEL_NUM_0 = 0,
	TCC_CHANNEL_NUM_1,
	TCC_CHANNEL_NUM_2,
	TCC_CHANNEL_NUM_3,
	TCC_CHANNEL_NUM_4,
	TCC_CHANNEL_NUM_5,
	TCC_CHANNEL_NUM_6
};


// Configurable TCC Instance
#define CONF_PWM_MODULE             TCC0

// Configurable PWM Channels
#define CONF_PWM_CHANNEL            1
#define CONF_PWM_OUTPUT             0
//
//// Configurable Default PWM PIN and MUX configuration
//#define CONF_PWM_OUT_PIN            PIN_PA04F_TCC0_WO0
//#define CONF_PWM_OUT_MUX            MUX_PA04F_TCC0_WO0

//// Configurable default period and match compare values.
//
//
//// Configurable options for Fault operations
//#define CONF_FAULT_EIC_PIN          PIN_PA02A_EIC_EXTINT2
//#define CONF_FAULT_EIC_PIN_MUX      MUX_PA02A_EIC_EXTINT2
//#define CONF_FAULT_EIC_LINE         2
//
//// Configurable options for Capture Mode
//#define CONF_TEST_PIN_OUT           PIN_PA07
//#define CONF_EVENT_GENERATOR_ID     EVSYS_ID_GEN_EIC_EXTINT_2
//#define CONF_EVENT_CAPTURE          EVSYS_ID_GEN_TCC0_MCX_0
//
//// Configurable Fault events used by FAULT Feature and CAPTURE Features.
//#define CONF_FAULT_EVENT_USER       EVSYS_ID_USER_TCC0_MC_0
//#define CONF_CAPTURE_EVENT_USER     EVSYS_ID_USER_TCC0_MC_1


#endif /* CONF_EXAMPLE_H_INCLUDED */
