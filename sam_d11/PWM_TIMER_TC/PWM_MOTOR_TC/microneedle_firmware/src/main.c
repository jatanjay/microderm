
/************************************************************************/
/* PWM_MOTOR_CONTROL :: 3 - SPEED, with system 10ms TC				    */
/* auth: jatan pandya												    */
/* date: 6/28/2024														*/
/************************************************************************/

#include "asf.h"

#include "system_setup.h"

#include "pwm_motor.h"

#include "led_driver.h"

#include "pwm_led.h"

#include "button.h"

#include "system_state.h"

#include "system_logic.h"

#include "adc_sample.h"


int main(void) {
  startup_sys_configs();
  while (true) {
    system_state();						// Get latest system_state
    system_logic();						// Invoke System Logic
  }
}





