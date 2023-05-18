/*
 ================================================================================================
 Module		 : MOTOR

 Name        : motor.h

 Author      : Hassan Sabry Ahmed Shahin

 Description : header file for motor driver

 Date        : Apr 4, 2023
 ================================================================================================
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/


/* Motor HW Ports and Pins Ids */

#define MOTOR_PORT_ID                 		  PORTD_ID
#define MOTOR_FORWARD_PIN_ID                  PIN2_ID
#define MOTOR_REVERSE_PIN_ID                  PIN3_ID

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum{

	Stop,CW,ACW

 }DcMotor_State;

 /*******************************************************************************
  *                                Functions Prototypes                                  *
  *******************************************************************************/

 void DcMotor_Init(void);
 void DcMotor_Rotate(DcMotor_State state);

#endif /* MOTOR_H_ */
