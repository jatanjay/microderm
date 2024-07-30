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


#endif /* CONF_EXAMPLE_H_INCLUDED */
