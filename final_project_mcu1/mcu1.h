/*
 ================================================================================================
 Module		 : HMI

 Name        : mcu1.h

 Author      : Hassan Sabry Ahmed Shahin

 Description : header file for HMI

 Date        : may 12, 2023
 ================================================================================================
 */

#ifndef MCU1_H_
#define MCU1_H_


#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "std_types.h"
#include "timer.h"
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

/********************************************************/
/* Definitions & Configurations */
/********************************************************/
#define PASSWORD_LENGTH			5
#define KEYPAD_INPUT_DELAY		500
#define DOOR_UNLOCKING_PERIOD	5
#define DOOR_LEFT_OPEN_PERIOD	3
#define DISPLAY_MESSAGE_DELAY	3000
/* following definitions used to communicate with Control ECU */
#define PASSWORD_MATCHED		1
#define PASSWORD_MISMATCHED		0
#define READY_TO_SEND			0x10
#define READY_TO_RECEIVE		0x11
#define CHANGE_PASSWORD_OPTION	0x15
#define UNLOCKING_DOOR			0x20
#define WRONG_PASSWORD			0x25
#define CHANGING_PASSWORD		0X30


/********************************************************/
/* Global variables */
/********************************************************/
uint8 g_inputPassword[PASSWORD_LENGTH];
uint8 g_password_match_status = 0;
uint16 g_seconds = 0;


/********************************************************/
/* Functions prototypes */
/********************************************************/
/*
 * Description: a function to initialize the password in first-run OR to change the password
 * */
void initializePassword(void);

/*
 * Description: A function to get the password from user and store it in a given array
 * */
void getPassword(uint8 * arrayName);

/*
 * Description: A function to display application options on LCD
 * */
void appMainOptions(void);

/*
 * Description: the call-back function called by the timer every 1 second
 * */
void timerCallBack(void);

/*
 * Description: A function that displays on LCD that door is opening or closing for a certain period of time
 * */
void DoorOpeningTask(void);

/*
 * Description: A function to send the password via UART by looping on sendByte function
 * */
void sendPasswordViaUART(uint8 * passwordArray);


#endif /* MCU1_H_ */
