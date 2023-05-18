/*
 ================================================================================================
 Module		 : buzzer

 Name        : buzzer.c

 Author      : Hassan Sabry Ahmed Shahin

 Description : source file for buzzer driver

 Date        : may 12, 2023
 ================================================================================================
 */

#include"gpio.h"
#include"buzzer.h"
/*
 * function to intialize buzzer
 * Setup the direction for the buzzer pin as output pin through the GPIO driver.
 * Turn off the buzzer through the GPIO
 * */
void Buzzer_init(){
	 /* PA0 output Pin */
	 GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);

	 /*intialize value=0 to stop the motor*/
	 GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}

/*Function to enable the Buzzer through the GPIO.*/
void Buzzer_on(void){
	 /*intialize value=1 to enable the buzzer*/
	 GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_HIGH);
}
/*Function to disable the Buzzer through the GPIO.*/
void Buzzer_off(void){
	 /*intialize value=0 to stop the buzzer*/
	 GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);

}
