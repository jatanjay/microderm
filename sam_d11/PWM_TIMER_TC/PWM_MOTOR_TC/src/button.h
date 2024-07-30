/*
 * button.h
 *
 * Created: 7/22/2024 3:37:47 PM
 *  Author: jatan
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_


#include <asf.h>
#include <system_setup.h>






























/************************************************************************/
/* BUTTON DEBOUNCE DEFNS	                                            */
/************************************************************************/

#define DELAY_PRESS_CN											400
#define DELAY_DEBOUNCE_CN										2


/************************************************************************/
/* BUTTON 1 LOGIC FLAGS	                                            */
/************************************************************************/
extern bool BUTTON_ONE_PRESS_STATUS;
extern bool BUTTON_ONE_RELEASE_STATUS;
extern bool LongPressB1Flag;
extern int press_B1_delay_count;
extern int long_press_B1_delay_count;


/************************************************************************/
/* BUTTON 2 LOGIC FLAGS	                                            */
/************************************************************************/
extern bool BUTTON_TWO_PRESS_STATUS;
extern bool BUTTON_TWO_RELEASE_STATUS;
extern bool LongPressB2Flag;
extern int press_B2_delay_count;
extern int long_press_B2_delay_count;


/************************************************************************/
/* Function Prototypes		                                            */
/************************************************************************/
 bool is_button_one_pressed(void);
 bool is_button_two_pressed(void);


#endif /* BUTTON_H_ */

