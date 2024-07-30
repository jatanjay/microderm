/*
 * adc_sample.h
 *
 * Created: 7/23/2024 11:00:30 AM
 *  Author: jatan
 */ 


#ifndef ADC_SAMPLE_H_
#define ADC_SAMPLE_H_

#include <asf.h>

struct adc_module adc_instance;
extern uint16_t adc_result;
void configure_adc(void);
void sample_adc(void);



#endif /* ADC_SAMPLE_H_ */