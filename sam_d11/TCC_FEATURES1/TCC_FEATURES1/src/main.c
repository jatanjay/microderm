#include <asf.h>

#include "string.h"
//
#include "conf_example.h"

struct tcc_module tcc_instance;
enum status_code stat = STATUS_OK;
struct tcc_config config_tcc;

#define TCC_PERIOD_VALUE 0x230
#define DEBOUNCE_DELAY 50 

uint32_t debounce_timestamp = 0;

bool is_button_pressed(void);

bool button_pressed = false;
bool button_released = false;


static void configure_tcc(void);
void check_button_press(void);
void toggle_pwm_duty_cycle(void);
void turn_led_on(void);
#define MAX_TOGGLE_COUNT 3

uint8_t toggle_count = 0;
bool pwm_running = false;


#define start_duty_cycle ((int)lround(TCC_PERIOD_VALUE / 6.0))
#define first_duty_cycle ((int)lround(TCC_PERIOD_VALUE / 3.5))
#define second_duty_cycle ((int)lround(TCC_PERIOD_VALUE / 1.5))



static void configure_tcc(void) {

	tcc_get_config_defaults(&config_tcc, CONF_PWM_MODULE);
	config_tcc.counter.period                               = TCC_PERIOD_VALUE;
	config_tcc.compare.match[TCC_MATCH_CAPTURE_CHANNEL_0]	= start_duty_cycle;

	config_tcc.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;
	config_tcc.pins.enable_wave_out_pin[TCC_MATCH_CAPTURE_CHANNEL_0]    = true;
	config_tcc.pins.wave_out_pin[TCC_MATCH_CAPTURE_CHANNEL_0]           = PIN_PA04F_TCC0_WO0;
	config_tcc.pins.wave_out_pin_mux[TCC_MATCH_CAPTURE_CHANNEL_0]       = MUX_PA04F_TCC0_WO0;
	stat = tcc_init(&tcc_instance, CONF_PWM_MODULE, &config_tcc);


}

bool is_button_pressed(void) {
	
	static int press_delay_count = 5;
	
	if(!port_pin_get_input_level(SW0_PIN))
	{
		button_pressed = true;
		press_delay_count--;		
	}
	else
	{
		button_pressed = false;
		press_delay_count = 5;
	}
	
	if(press_delay_count <= 0)
	{
		button_released = false;
		return true;
		press_delay_count = 0;
	}
	else
	{
		button_released = true;
		return false;	
	}

}




void check_button_press(void) {
	
	static bool motor_status_changed = false;
	
	if (is_button_pressed() & !motor_status_changed) {
	
		toggle_count++;
		motor_status_changed = true;
		
		if (!pwm_running) {
			pwm_running = true;			
			tcc_enable(&tcc_instance);
		} else {
			toggle_pwm_duty_cycle();
		}
	}
	
	if(button_released){
		motor_status_changed = false;
	}
}

void toggle_pwm_duty_cycle(void) {
	if (pwm_running) {
		if (toggle_count == 2) {
			LED_On(LED_0_PIN);
			tcc_set_compare_value(&tcc_instance, TCC_MATCH_CAPTURE_CHANNEL_0, first_duty_cycle);
			} else if (toggle_count == 3) {
			tcc_set_compare_value(&tcc_instance, TCC_MATCH_CAPTURE_CHANNEL_0, second_duty_cycle);
			LED_Off(LED_0_PIN);
			} else if (toggle_count > 3) {
			toggle_count = 0;
			tcc_set_compare_value(&tcc_instance, TCC_MATCH_CAPTURE_CHANNEL_0, start_duty_cycle);
			pwm_running = false;
			tcc_disable(&tcc_instance);
			//tcc_restart_counter(&tcc_instance);
		}
	}
}
int main(void) {
	system_init();
    system_interrupt_enable_global();
	configure_tcc();
	while (1) {
		check_button_press();
	}
}