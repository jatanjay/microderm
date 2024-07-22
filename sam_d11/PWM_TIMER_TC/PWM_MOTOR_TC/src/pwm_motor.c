/*
 * pwm_motor.c
 *
 * Created: 7/22/2024 3:45:40 PM
 *  Author: jatan
 */ 

 #include "pwm_motor.h"


bool PULSATING_MOTOR_ROUTINE = false;
uint8_t motor_toggle_count = 0;
bool PWM_RUNNING = false;

void configure_pwm_generator (void)
{
	struct tc_config config_tc;
	tc_get_config_defaults (&config_tc);

	config_tc.clock_source = TC_CLOCK_SOURCE;
	config_tc.counter_size = TC_COUNTER_SIZE;
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER;

	config_tc.counter_8_bit.value = 0;
	config_tc.counter_8_bit.period = PWM_PERIOD_VALUE;

	config_tc.counter_8_bit.compare_capture_channel[0] = INITIAL_DUTY_CYCLE;

	config_tc.wave_generation = TC_WAVE_GENERATION_NORMAL_PWM;
	config_tc.pwm_channel[0].pin_out = PWM_PIN_OUT;
	config_tc.pwm_channel[0].pin_mux = PWM_MUX_OUT;

	config_tc.pwm_channel[0].enabled = true;
	
	tc_init (&pwm_generator_instance, PWM_GENERATOR, &config_tc);
}




void pwm_motor_cleanup(void){
	PULSATING_MOTOR_ROUTINE = false;
	motor_toggle_count = 0;
	tc_set_compare_value (&pwm_generator_instance,
	TC_COMPARE_CAPTURE_CHANNEL_0,
	INITIAL_DUTY_CYCLE);
	PWM_RUNNING = false;
	tc_disable (&pwm_generator_instance);
	port_pin_set_output_level(MOTOR_NSLEEP_PIN,LOW);
}



void cycle_pwm_motor (void)
{
	{
		if (PWM_RUNNING)
		{
			
			if (motor_toggle_count == 2)
			{
				tc_set_compare_value (&pwm_generator_instance,
				TC_COMPARE_CAPTURE_CHANNEL_0, FIRST_DUTY_CYCLE);


			}
			else if (motor_toggle_count == 3)
			{
				tc_set_compare_value (&pwm_generator_instance,
				TC_COMPARE_CAPTURE_CHANNEL_0,
				SECOND_DUTY_CYCLE);

			}
			
			else if (motor_toggle_count == 4)
			{

				PULSATING_MOTOR_ROUTINE = true;
				tc_set_compare_value (&pwm_generator_instance,
				TC_COMPARE_CAPTURE_CHANNEL_0,
				SECOND_DUTY_CYCLE);

			}
			
			else if (motor_toggle_count > 4)
			{
				pwm_motor_cleanup();
			}
		}
	}

}



 void toggle_nsleep(void){
	 static bool PULSATING_MOTOR = false;
	 if (PULSATING_MOTOR_ROUTINE){
		 if (PULSATING_MOTOR){
			 port_pin_set_output_level(MOTOR_NSLEEP_PIN,LOW);
			 PULSATING_MOTOR = false;
			 }else{
			 PULSATING_MOTOR = true;
			 port_pin_set_output_level(MOTOR_NSLEEP_PIN,HIGH);
		 }
	 }
 }
