/*
 ================================================================================================
 Module		 : ECU
 Name        : mcu2.c

 Author      : Hassan Sabry Ahmed Shahin

 Description : source file for ECU

 Date        : may 12, 2023
 ================================================================================================
 */


#include "mcu2.h"

int main(void){
	SREG |=(1<<7); // enable I-BIT


	/*UART configuration*/
	UART_ConfigType configPtr = {Data_8_Bit,No_Parity, One_Bit,BAUD_RATE_1};
	UART_init(&configPtr);

	/*
	 * Ftimer=Fcpu/prescaler=8MHZ/1024=128usec
	 * to produce interrupt every 1 sec ->1/127usec=7812
	 * */
	Timer_configuration timer_config = { Timer1, CTC, 0, 7813, Prescaler_1024, timerCallBack };
	Timer_init(&timer_config);

	/* initialize I2C */
	TWI_ConfigType config = {0x02, TWI_CONTROL_ECU_ADDRESS};
	TWI_init(&config);
	/*initialize DC motor and buzzer*/
	DcMotor_Init();
	Buzzer_init();
	/*initialize password*/
	initializePassword();

	uint8 receivedByte=0;
	while(1){
		/*wait for response from HMI*/
		if (UART_recieveByte() == READY_TO_SEND)
		{
			/*receive Password via UART*/
			receivePasswordViaUART(g_receivedPassword);
			receivedByte = UART_recieveByte();
			/*if user choose to change password*/
			if ( receivedByte == '+')
			{
				/*compare two passwords if they are matching*/
				if (compare_passwords(g_storedPassword, g_receivedPassword) == PASSWORD_MATCHED)
				{
					/* inform HMI to display that door is unlocking */
					UART_sendByte(UNLOCKING_DOOR);
					/*call open door function*/
					DoorOpeningTask();
				}
				/*compare two passwords if they are mismatch*/
				else
				{
					/*send to HMI the password is wrong*/
					UART_sendByte(WRONG_PASSWORD);
					/* count number of wrong attempts, and turn on a buzzer of it exceeds the limit */
					g_wrongPasswordCounter++;
					if (g_wrongPasswordCounter == NUMBER_OF_WRONG_PASSWORD_ATTEMPTS)
					{
						/*turnning on buzzer*/
						Buzzer_on();
						/*buzzer delay*/
						g_seconds=0;
						while(g_seconds < ALARM_ON_DELAY); /* turn on alarm for a certain period */
						/*turnning off buzzer*/
						Buzzer_off();
						g_wrongPasswordCounter=0; /* reset the counter */
					}
				}


			}
			/*if user choose to change the password*/
			else if (receivedByte == CHANGE_PASSWORD_OPTION)
			{
				/*compare_passwords if they are match*/
				if (compare_passwords(g_storedPassword, g_receivedPassword) == PASSWORD_MATCHED)
				{
					 /* inform HMI to change password */
					UART_sendByte(CHANGING_PASSWORD);
					/*initialize password*/
					initializePassword();
				}
				/*if the user enter wrong password*/
				else
				{
					 /* inform HMI its a wrong password */
					UART_sendByte(WRONG_PASSWORD);
				}
			}
		}
	}
}


/********************************************************/
/* Functions implementations */
/********************************************************/
/*A function to compare two passwords*/
uint8 compare_passwords(uint8 a_password1[PASSWORD_LENGTH],uint8 a_password2[PASSWORD_LENGTH]) {
	/*getting password from EEPROM*/
	updateStoredPassword();
	uint8 i;
	/*looping till comparing two passwords*/
	for (i = 0; i < PASSWORD_LENGTH; i++) {
		if (a_password1[i] != a_password2[i]) {
			/*if passwords are mismatch return false */
			return PASSWORD_MISMATCHED;
		}
	}
	/*if passwords are mismatch return true */
	return PASSWORD_MATCHED;
}
/*A function to open door*/
void DoorOpeningTask(void){
	/* run the DC motor clockwise for 15 seconds */
	g_seconds = 0;
	DcMotor_Rotate(CW);
	while (g_seconds < DOOR_UNLOCKING_PERIOD);

	/* let the door be open for 3 seconds */
	g_seconds = 0;
	DcMotor_Rotate(Stop);
	while (g_seconds < DOOR_LEFT_OPEN_PERIOD);

	/* hold the system for 15 seconds & display to user that door is locking */
	g_seconds = 0;
	DcMotor_Rotate(ACW);
	while (g_seconds < DOOR_UNLOCKING_PERIOD);

	DcMotor_Rotate(Stop);
}
/*A call back function*/
void timerCallBack(void){
	g_seconds++;
}
/*A function to initialize Password*/
void initializePassword(void){

	/* do not return from this function till ECU sends a PASSWORD_MATCH signal */
	uint8 confirmationPassword[PASSWORD_LENGTH];
	uint8 check=0;
	while(!check){
		/* wait till HMI gets ready*/
		while (UART_recieveByte() != READY_TO_SEND);
		/* inform HMI that ECU ready to receive the password */
		UART_sendByte(READY_TO_RECEIVE);
		/*receive Password via UART*/
		receivePasswordViaUART(g_receivedPassword);

		/* wait till HMI gets ready*/
		while (UART_recieveByte() != READY_TO_SEND);
		/* inform HMI to send the confirmation password */
		UART_sendByte(READY_TO_RECEIVE);
		/*receive Password via UART*/
		receivePasswordViaUART(confirmationPassword);

		/*compare between two passwords*/
		if (compare_passwords(g_receivedPassword, confirmationPassword) == PASSWORD_MATCHED){
			/*wait for response from ECU*/
			UART_sendByte(READY_TO_SEND);
			/*if passwords are match*/
			UART_sendByte(PASSWORD_MATCHED);
			/*store password in EEPROM*/
			storePassword();
			/*end this function*/
			check=1;
		}
		else{
			/*wait for response from ECU*/
			UART_sendByte(READY_TO_SEND);
			/*if passwords are mismatch*/
			UART_sendByte(PASSWORD_MISMATCHED);
		}
	}
}

/*A function to receive Password via UART*/
void receivePasswordViaUART(uint8 * passwordArray){
	uint8 count;
	/*looping till receiving password*/
	for (count=0;count<PASSWORD_LENGTH;count++){
		*(passwordArray+count) = UART_recieveByte();
		/*delay till it receive password*/
		_delay_ms(100);
	}
}
/*A function to update Password*/
void updateStoredPassword(void){
	uint8 i;
	/*looping till reading password from EEPROM*/
	for (i=0;i<PASSWORD_LENGTH;i++){
		EEPROM_readByte(EEPROM_STORE_ADDREESS+i, g_storedPassword+i);
	}
}
/*A function to store Password*/
void storePassword(void){
	uint8 i;
	for (i = 0; i < PASSWORD_LENGTH; i++) {
		/*looping till writing password from EEPROM*/
		EEPROM_writeByte(EEPROM_STORE_ADDREESS + i, g_receivedPassword[i]);
		_delay_ms(100);
	}
}
