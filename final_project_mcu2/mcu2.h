/*
 ================================================================================================
 Module		 : ECU

 Name        : mcu2.c

 Author      : Hassan Sabry Ahmed Shahin

 Description : header file for ECU

 Date        : may 12, 2023
 ================================================================================================
 */

#ifndef MCU2_H_
#define MCU2_H_

#include "buzzer.h"
#include "timer.h"
#include "motor.h"
#include "external_eeprom.h"
#include "twi.h"
#include <avr/io.h>
#include <avr/delay.h>
#include "uart.h"

/********************************************************/
/* Definitions & Configurations */
/********************************************************/
#define PASSWORD_LENGTH						5
#define DOOR_UNLOCKING_PERIOD				5
#define DOOR_LEFT_OPEN_PERIOD				3
#define NUMBER_OF_WRONG_PASSWORD_ATTEMPTS 	3
#define ALARM_ON_DELAY						3

/* following definitions used to communicate with Control ECU */
#define PASSWORD_MATCHED		1
#define PASSWORD_MISMATCHED		0
#define READY_TO_SEND			0x10
#define READY_TO_RECEIVE		0x11
#define CHANGE_PASSWORD_OPTION	0x15
#define UNLOCKING_DOOR			0x20
#define WRONG_PASSWORD			0x25
#define CHANGING_PASSWORD		0X30
#define TWI_CONTROL_ECU_ADDRESS				0x1
#define EEPROM_STORE_ADDREESS				0x00


/********************************************************/
/* Global variables */
/********************************************************/
uint8 g_receivedPassword[PASSWORD_LENGTH];
uint8 g_storedPassword[PASSWORD_LENGTH];
uint8 g_wrongPasswordCounter=0;
uint16 g_seconds = 0;


/********************************************************/
/* Functions prototypes */
/********************************************************/
/*
 * Description: a function to compare the stored pass with the received one
 * */
uint8 compare_passwords(uint8 a_password1[PASSWORD_LENGTH],uint8 a_password2[PASSWORD_LENGTH]);

/*
 * Description: a function to initialize the password in first-run OR to change the password
 * */
void initializePassword(void);

/*
 * Decription: A function that rotates on the DC motor for 15 seconds clockwise, stops it for 3 seconds, then
 * 		rotates it anti-clockwise for 15 seconds.
 * */
void DoorOpeningTask(void);

/*
 * Decription: the call-back function called by the timer every 1 second
 * */
void timerCallBack(void);

/*
 * Description: A function to receive the password via UART by looping on receiveByte function
 * */
void receivePasswordViaUART(uint8 * passwordArray);

/*
 * Description: A function to retrieve the stored password from EEPROM
 * */
void updateStoredPassword(void);

/*
 * Description: A function to store the password in EEPROM
 * */
void storePassword(void);


#endif /* MCU2_H_ */
