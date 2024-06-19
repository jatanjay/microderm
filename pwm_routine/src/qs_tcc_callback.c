#include <asf.h>

#include <conf_quick_start_callback.h>

static void configure_tcc(void);
static void configure_tcc_callbacks(void);
static void tcc_callback_to_change_duty_cycle(
struct tcc_module *
const module_inst);
void configure_extint_channel(void);
void configure_extint_callbacks(void);
void extint_detection_callback(void);

struct tcc_module tcc_instance;


#define PWM_PERIOD 0x210 // DEC 528
#define CC_X ((int)round(PWM_PERIOD / 7.0))


static bool pwm_started = false;

static
const uint32_t delay_values[] = {
	1,
	3,
	5,
	22
};
static
const uint8_t num_delay_values = sizeof(delay_values) / sizeof(delay_values[0]);

void configure_tcc_callback_delay(uint32_t delay);

static uint32_t pwm_callback_delay = 0;

void configure_tcc_callback_delay(uint32_t delay) {
	pwm_callback_delay = delay;
}

static uint8_t current_delay_index = 0;


static void tcc_callback_to_change_duty_cycle(
struct tcc_module *
const module_inst) {

	if (!pwm_started) {
		return;
	}

	tcc_set_compare_value(module_inst,
	(enum tcc_match_capture_channel)
	(TCC_MATCH_CAPTURE_CHANNEL_0 + CONF_PWM_CHANNEL),
	pwm_callback_delay);
}
static void configure_tcc(void) {

	struct tcc_config config_tcc;
	tcc_get_config_defaults( & config_tcc, CONF_PWM_MODULE);
	config_tcc.counter.period = PWM_PERIOD;
	config_tcc.compare.wave_generation = TCC_WAVE_GENERATION_SINGLE_SLOPE_PWM;
	config_tcc.compare.match[CONF_PWM_CHANNEL] = PWM_PERIOD;
	config_tcc.pins.enable_wave_out_pin[CONF_PWM_OUTPUT] = true;
	config_tcc.pins.wave_out_pin[CONF_PWM_OUTPUT] = CONF_PWM_OUT_PIN;
	config_tcc.pins.wave_out_pin_mux[CONF_PWM_OUTPUT] = CONF_PWM_OUT_MUX;
	tcc_init( & tcc_instance, CONF_PWM_MODULE, & config_tcc);
	tcc_enable( & tcc_instance);

}

static void configure_tcc_callbacks(void) {
	tcc_register_callback( &
	tcc_instance,
	tcc_callback_to_change_duty_cycle,
	(enum tcc_callback)(TCC_CALLBACK_CHANNEL_0 + CONF_PWM_CHANNEL));
	tcc_enable_callback( & tcc_instance,
	(enum tcc_callback)(TCC_CALLBACK_CHANNEL_0 + CONF_PWM_CHANNEL));

}

void configure_extint_channel(void) {
	struct extint_chan_conf config_extint_chan;
	extint_chan_get_config_defaults( & config_extint_chan);
	config_extint_chan.gpio_pin = BUTTON_0_EIC_PIN;
	config_extint_chan.gpio_pin_mux = BUTTON_0_EIC_MUX;
	config_extint_chan.gpio_pin_pull = EXTINT_PULL_UP;
	config_extint_chan.detection_criteria = EXTINT_DETECT_BOTH;
	extint_chan_set_config(BUTTON_0_EIC_LINE, & config_extint_chan);
}
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
	port_pin_set_output_level(LED_0_PIN, pin_state);
	static uint32_t i = 0;
	i = (i + CC_X) & 0xFFFF;
	if (pin_state) {
		pwm_callback_delay = i;
	}
}

int main(void) {
	system_init();
	configure_tcc();
	configure_tcc_callbacks();
	configure_extint_channel();
	configure_extint_callbacks();
	system_interrupt_enable_global();
	while (true) {}
}