/*
 ================================================================================================
 Module		 : MOTOR

 Name        : motor.c

 Author      : Hassan Sabry Ahmed Shahin

 Description : source file for motor driver

 Date        : Apr 4, 2023
 ================================================================================================
 */


#include"motor.h"
#include"gpio.h"

 void DcMotor_Init(void){

	 /* PB1 output Pin */
	 GPIO_setupPinDirection(MOTOR_PORT_ID,MOTOR_FORWARD_PIN_ID,PIN_OUTPUT);

	 /* PB2 output Pin */
	 GPIO_setupPinDirection(MOTOR_PORT_ID,MOTOR_REVERSE_PIN_ID,PIN_OUTPUT);

	 /*intialize value=0 to stop the motor*/
	 GPIO_writePin(MOTOR_PORT_ID,MOTOR_FORWARD_PIN_ID,LOGIC_LOW);
	 GPIO_writePin(MOTOR_PORT_ID,MOTOR_REVERSE_PIN_ID,LOGIC_LOW);

 }

 void DcMotor_Rotate(DcMotor_State state){

	 /*implement motor code */
	 switch (state){
	 case CW:
		 GPIO_writePin(MOTOR_PORT_ID,MOTOR_FORWARD_PIN_ID,LOGIC_HIGH);
		 GPIO_writePin(MOTOR_PORT_ID,MOTOR_REVERSE_PIN_ID,LOGIC_LOW);
		 break;

	 case ACW:
		 GPIO_writePin(MOTOR_PORT_ID,MOTOR_FORWARD_PIN_ID,LOGIC_LOW);
		 GPIO_writePin(MOTOR_PORT_ID,MOTOR_REVERSE_PIN_ID,LOGIC_HIGH);
		 break;

	 case Stop:
		 GPIO_writePin(MOTOR_PORT_ID,MOTOR_FORWARD_PIN_ID,LOGIC_LOW);
		 GPIO_writePin(MOTOR_PORT_ID,MOTOR_REVERSE_PIN_ID,LOGIC_LOW);
		 break;
	 }

 }

