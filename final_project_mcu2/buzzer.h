/*
 ================================================================================================
 Module		 : buzzer

 Name        : buzzer.h

 Author      : Hassan Sabry Ahmed Shahin

 Description : header file for motor driver

 Date        : may 12, 2023
 ================================================================================================
 */


#ifndef BUZZER_H_
#define BUZZER_H_


#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/


/* BUZZER HW Ports and Pins Ids */

#define BUZZER_PORT_ID                 		  PORTC_ID
#define BUZZER_PIN_ID            		      PIN2_ID


/*******************************************************************************
 *                                Functions                                  *
 *******************************************************************************/


/*
 * function to intialize buzzer
 * Setup the direction for the buzzer pin as output pin through the GPIO driver.
 * Turn off the buzzer through the GPIO
 * */
void Buzzer_init();

/*Function to enable the Buzzer through the GPIO.*/
void Buzzer_on(void);

/*Function to disable the Buzzer through the GPIO.*/
void Buzzer_off(void);

#endif /* BUZZER_H_ */
