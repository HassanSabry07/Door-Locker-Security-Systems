 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Tarek updated by Hassan Sabry
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/*******************************************************************************
 *                      Types Declaration                                  *
 *******************************************************************************/

typedef enum{

	Data_5_Bit,Data_6_Bit,Data_7_Bit,Data_8_Bit
}UART_BitData;


typedef enum{
No_Parity,Reserved,Even_Parity,Odd,_Parity
}UART_Parity;


typedef enum{
One_Bit,Two_Bit
}UART_StopBit;

typedef enum{
	BAUD_RATE_1=9600 ,BAUD_RATE_2=14400 ,BAUD_RATE_3=19200 ,BAUD_RATE_4=38400
}UART_BaudRate;

typedef struct{
 UART_BitData bit_data;
 UART_Parity parity;
 UART_StopBit stop_bit;
 UART_BaudRate baud_rate;
}UART_ConfigType;



/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/


/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType * Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
