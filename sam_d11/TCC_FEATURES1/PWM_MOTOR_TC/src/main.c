
/************************************************************************/
/* PWM_MOTOR_CONTROL :: 3 - SPEED, with system 10ms TC				    */
/* auth: jatan pandya												    */
/* date: 6/28/2024														*/
/************************************************************************/

#include <asf.h>
#include "conf_example.h"
#include "string.h"

struct tc_module
    system_timer_instance; // instance for system timer counter (TC1)
struct tc_module pwm_generator_instance; // instance for PWM Motor Control (TC0)

#define SYSTEM_TC														TC1
#define PWM_GENERATOR													TC2

#define TC_COUNTER_SIZE													TC_COUNTER_SIZE_8BIT
#define TC_CLOCK_PRESCALER 											    TC_CLOCK_PRESCALER_DIV64
#define TC_CLOCK_SOURCE  											    GCLK_GENERATOR_0

#define SYSTEM_TC_PERIOD_VALUE  										124

#define PWM_PIN_OUT  													PIN_PA10E_TC2_WO0;
#define PWM_MUX_OUT  													MUX_PA10E_TC2_WO0;

#define PWM_PERIOD_VALUE  											    85
#define INITIAL_DUTY_CYCLE  											((int)lround (PWM_PERIOD_VALUE / 4.0))
#define FIRST_DUTY_CYCLE  											    ((int)lround (PWM_PERIOD_VALUE / 2.5))
#define SECOND_DUTY_CYCLE  											    ((int)lround (PWM_PERIOD_VALUE / 1.10))
#define MAX_TOGGLE_COUNT  											    3

uint8_t toggle_count = 0;
bool BUTTON_PRESS_STATUS = false;
bool BUTTON_RELEASE_STATUS = false;
bool PWM_RUNNING = false;
bool tick_flag_10ms;

static void configure_pwm_generator (void);
void check_button_press (void);
void cycle_pwm_duty (void);
void turn_led_on (void);
bool is_button_pressed (void);

/************************************************************************/
/* TC - TIMER CODE START
 */
/************************************************************************/

void configure_system_tc (void);
void system_tc_callbacks (void);
void tc_callback_to_toggle_led (struct tc_module *const module_inst);

void tc_callback_to_toggle_led (struct tc_module *const module_inst)
{
  static int tick_count_1ms;
  static int tick_count_10ms;
  tick_count_1ms++;
  if (tick_count_1ms > 10)
    {
      tick_count_10ms++;
      tick_count_1ms = 0;
      tick_flag_10ms = true;
      port_pin_toggle_output_level (LED0_PIN);
      check_button_press ();
    }
}

void configure_system_tc (void)
{
  struct tc_config config_tc;
  tc_get_config_defaults (&config_tc);
  config_tc.clock_source = TC_CLOCK_SOURCE;
  config_tc.counter_size = TC_COUNTER_SIZE;
  config_tc.clock_prescaler = TC_CLOCK_PRESCALER;
  config_tc.counter_8_bit.period = SYSTEM_TC_PERIOD_VALUE;
  tc_init (&system_timer_instance, SYSTEM_TC, &config_tc);
  tc_enable (&system_timer_instance);
}

void system_tc_callbacks (void)
{
  tc_register_callback (&system_timer_instance, tc_callback_to_toggle_led,
                        TC_CALLBACK_OVERFLOW);
  tc_enable_callback (&system_timer_instance, TC_CALLBACK_OVERFLOW);
}

/************************************************************************/
/* TC - TIMER CODE START
 */
/************************************************************************/


static void configure_pwm_generator (void)
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
  config_tc.pwm_channel[0].pin_out = PIN_PA10E_TC2_WO0;
  config_tc.pwm_channel[0].pin_mux = MUX_PA10E_TC2_WO0;

  config_tc.pwm_channel[0].enabled = true;

  tc_init (&pwm_generator_instance, PWM_GENERATOR, &config_tc);
}

bool is_button_pressed (void)
{
  static int press_delay_count = 5;
  if (!port_pin_get_input_level (SW0_PIN))
    {
      BUTTON_PRESS_STATUS = true;
      press_delay_count--;
    }
  else
    {
      BUTTON_PRESS_STATUS = false;
      press_delay_count = 5;
    }
  if (press_delay_count <= 0)
    {
      BUTTON_RELEASE_STATUS = false;
      return true;
      press_delay_count = 0;
    }
  else
    {
      BUTTON_RELEASE_STATUS = true;
      return false;
    }
}

void check_button_press (void)
{
  static bool motor_status_changed = false;
  if (is_button_pressed () & !motor_status_changed)
    {
      toggle_count++;
      motor_status_changed = true;
      if (!PWM_RUNNING)
        {
          PWM_RUNNING = true;
          tc_enable (&pwm_generator_instance);
        }
      else
        {
          cycle_pwm_duty ();
        }
    }
  if (BUTTON_RELEASE_STATUS)
    {
      motor_status_changed = false;
    }
}

void cycle_pwm_duty (void)
{
  if (PWM_RUNNING)
    {
      if (toggle_count == 2)
        {
          tc_set_compare_value (&pwm_generator_instance,
                                TC_COMPARE_CAPTURE_CHANNEL_0, FIRST_DUTY_CYCLE);
        }
      else if (toggle_count == 3)
        {
          tc_set_compare_value (&pwm_generator_instance,
                                TC_COMPARE_CAPTURE_CHANNEL_0,
                                SECOND_DUTY_CYCLE);
        }
      else if (toggle_count > 3)
        {
          toggle_count = 0;
          tc_set_compare_value (&pwm_generator_instance,
                                TC_COMPARE_CAPTURE_CHANNEL_0,
                                INITIAL_DUTY_CYCLE);
          PWM_RUNNING = false;
          tc_disable (&pwm_generator_instance);
        }
    }
}

int main (void)
{
  system_init ();
  system_interrupt_enable_global ();

  configure_system_tc ();
  system_tc_callbacks ();
  configure_pwm_generator ();
  while (true)
    {
      if (tick_flag_10ms)
        {
          tick_flag_10ms = false;
        }
    }
}
