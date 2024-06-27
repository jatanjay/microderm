#include <asf.h>

#include "conf_example.h"
#include "string.h"

struct tcc_module tcc_instance;
enum status_code stat = STATUS_OK;
struct tcc_config config_tcc;

#define TCC_PERIOD_VALUE 0x395
#define DEBOUNCE_DELAY 50
#define GLCK_SOURCE GCLK_GENERATOR_0
#define TCC_CLOCK_DIVIDER TCC_CLOCK_PRESCALER_DIV4
#define MAX_TOGGLE_COUNT 3
#define start_duty_cycle ((int)lround(TCC_PERIOD_VALUE / 4.0))
#define first_duty_cycle ((int)lround(TCC_PERIOD_VALUE / 2.5))
#define second_duty_cycle ((int)lround(TCC_PERIOD_VALUE / 1.10))

uint8_t toggle_count = 0;
uint32_t debounce_timestamp = 0;
bool is_button_pressed(void);
bool button_pressed = false;
bool button_released = false;
bool pwm_running = false;
static void configure_tcc(void);
void check_button_press(void);
void toggle_pwm_duty_cycle(void);
void turn_led_on(void);


/************************************************************************/
/* TC - TIMER CODE START												*/
/************************************************************************/

struct tc_module tc_instance;
bool tick_flag_10ms;

void configure_tc(void);
void configure_tc_callbacks(void);
void tc_callback_to_toggle_led(
struct tc_module *const module_inst);

void tc_callback_to_toggle_led(
struct tc_module *const module_inst)
{
	
	static int tick_count_1ms;
	static int tick_count_10ms;
	tick_count_1ms++;
	if(tick_count_1ms>10){
		tick_count_10ms++;
		tick_count_1ms = 0;
		tick_flag_10ms = true;
		port_pin_toggle_output_level(LED0_PIN);
		check_button_press();
	}
	
}

void configure_tc(void)
{
	struct tc_config config_tc;
	tc_get_config_defaults(&config_tc);
	config_tc.clock_source = GCLK_GENERATOR_0; // Change source from 0 to 1 (8mhz to 32khz)
	config_tc.counter_size = TC_COUNTER_SIZE_8BIT;
	config_tc.clock_prescaler = TC_CLOCK_PRESCALER_DIV64;
	config_tc.counter_8_bit.period = 124;
	tc_init(&tc_instance, TC1, &config_tc);
	tc_enable(&tc_instance);
}

void configure_tc_callbacks(void)
{
	tc_register_callback(&tc_instance, tc_callback_to_toggle_led, TC_CALLBACK_OVERFLOW);
	tc_enable_callback(&tc_instance, TC_CALLBACK_OVERFLOW);
}


/************************************************************************/
/* TC - TIMER CODE START												*/
/************************************************************************/




static void configure_tcc(void) {
	tcc_get_config_defaults(&config_tcc, CONF_PWM_MODULE);
	config_tcc.counter.period = TCC_PERIOD_VALUE;
	config_tcc.compare.match[TCC_MATCH_CAPTURE_CHANNEL_0] = start_duty_cycle;
	config_tcc.compare.wave_polarity[TCC_MATCH_CAPTURE_CHANNEL_0] = 1;
	config_tcc.compare.wave_generation = TCC_WAVE_GENERATION_DOUBLE_SLOPE_BOTH;
	config_tcc.counter.clock_prescaler = TCC_CLOCK_DIVIDER;
	config_tcc.pins.enable_wave_out_pin[TCC_MATCH_CAPTURE_CHANNEL_0] = true;
	config_tcc.pins.wave_out_pin[TCC_MATCH_CAPTURE_CHANNEL_0] =
	PIN_PA04F_TCC0_WO0;
	config_tcc.pins.wave_out_pin_mux[TCC_MATCH_CAPTURE_CHANNEL_0] =
	MUX_PA04F_TCC0_WO0;
	stat = tcc_init(&tcc_instance, CONF_PWM_MODULE, &config_tcc);
}

bool is_button_pressed(void) {
	static int press_delay_count = 5;
	if (!port_pin_get_input_level(SW0_PIN)) {
		button_pressed = true;
		press_delay_count--;
		} else {
		button_pressed = false;
		press_delay_count = 5;
	}
	if (press_delay_count <= 0) {
		button_released = false;
		return true;
		press_delay_count = 0;
		} else {
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
	if (button_released) {
		motor_status_changed = false;
	}
}

void toggle_pwm_duty_cycle(void) {
	if (pwm_running) {
		if (toggle_count == 2) {
			tcc_set_compare_value(&tcc_instance, TCC_MATCH_CAPTURE_CHANNEL_0,
			first_duty_cycle);
			} else if (toggle_count == 3) {
			tcc_set_compare_value(&tcc_instance, TCC_MATCH_CAPTURE_CHANNEL_0,
			second_duty_cycle);
			} else if (toggle_count > 3) {
			toggle_count = 0;
			tcc_set_compare_value(&tcc_instance, TCC_MATCH_CAPTURE_CHANNEL_0,
			start_duty_cycle);
			pwm_running = false;
			tcc_disable(&tcc_instance);
		}
	}
}
int main(void) {
	system_init();
	configure_tc();
	configure_tc_callbacks();
	system_interrupt_enable_global();
	configure_tcc();
	while (true) {
		if(tick_flag_10ms)
		{
			tick_flag_10ms = false;
		}
}
}