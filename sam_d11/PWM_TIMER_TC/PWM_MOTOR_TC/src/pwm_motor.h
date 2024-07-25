 /*
 * pwm_motor.h
 *
 * Created: 7/22/2024 3:45:54 PM
 *  Author: jatan
 */ 


#ifndef PWM_MOTOR_H_
#define PWM_MOTOR_H_

#include "asf.h"
#include "conf_example.h"
#include "system_setup.h"
#include "math.h"
#include "system_logic.h"
struct tc_module pwm_generator_instance;								// instance for PWM Motor Control (TC0)



/************************************************************************/
/* PWM DEFINITIONS														 */
/************************************************************************/

#define PWM_GENERATOR													TC2
#define PWM_PIN_OUT  													PIN_PA10E_TC2_WO0;
#define PWM_MUX_OUT														MUX_PA10E_TC2_WO0;
#define PWM_PERIOD_VALUE  											    85
#define INITIAL_DUTY_CYCLE  											((int)lround (PWM_PERIOD_VALUE / 4.0))
#define FIRST_DUTY_CYCLE  											    ((int)lround (PWM_PERIOD_VALUE / 2.5))
#define SECOND_DUTY_CYCLE  											    ((int)lround (PWM_PERIOD_VALUE / 1.10))
#define MAX_TOGGLE_COUNT  											    3



/************************************************************************/
/* MOTOR CONTROL Variables                                          */
/************************************************************************/
extern uint8_t motor_toggle_count;
extern bool motor_running;
extern bool pulsating_motor_routine;


/************************************************************************/
/* Motor control function prototypes                                        */
/************************************************************************/
void configure_pwm_generator(void);
void cycle_pwm_motor (void);
void motor_disable(void);
void toggle_nsleep(void);
void motor_enable(void);


#endif /* PWM_MOTOR_H_ */