/*
 * adc_sample.c
 *
 * Created: 7/23/2024 11:00:15 AM
 *  Author: jatan
 */ 


 #include <adc_sample.h>




uint16_t adc_result;

void configure_adc(void)
{
	struct adc_config config_adc;
	adc_get_config_defaults(&config_adc);
	

	adc_init(&adc_instance, ADC, &config_adc);
	adc_enable(&adc_instance);
}


void sample_adc(void){
	adc_start_conversion(&adc_instance);
	do {
	} while (adc_read(&adc_instance, &adc_result) == STATUS_BUSY);
}