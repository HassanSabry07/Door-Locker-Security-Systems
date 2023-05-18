/*
 ================================================================================================
 Module		 : HMI

 Name        : mcu1.c

 Author      : Hassan Sabry Ahmed Shahin

 Description : source file for HMI

 Date        : may 12, 2023
 ================================================================================================
 */

#include "mcu1.h"

int main(void){
	/*enable I-bit*/
	SREG |= (1 << SREG_I);

	/*UART configuration*/
	UART_ConfigType configPtr = {Data_8_Bit,No_Parity, One_Bit,BAUD_RATE_1};
	UART_init(&configPtr);

	/*
	 * Ftimer=Fcpu/prescaler=8MHZ/1024=128usec
	 * to produce interrupt every 1 sec ->1/127usec=7812
	 * */
	Timer_configuration timer_config = { Timer1, CTC, 0, 7813, Prescaler_1024,timerCallBack };
	Timer_init(&timer_config);

	/*initialize LCD*/
	LCD_init();
	/*initialize PASSWORD and Main options*/
	initializePassword(); /* initialize first-time password */
	appMainOptions(); /* Display application options */

	uint8 receivedByte = 0, key = 0;

	while (1) {
		/*geting number of password from keypad*/
		key = KEYPAD_getPressedKey();
		/*if user choose to open door*/
		if (key == '+')
		{
			/*clear screen*/
			LCD_clearScreen();
			/*display on LCD*/
			LCD_displayString("Enter pass");
			/*getting passwrod from user*/
			getPassword(g_inputPassword);

			/* inform ECU to start sending */
			UART_sendByte(READY_TO_SEND);
			sendPasswordViaUART(g_inputPassword);
			/* inform Control ECU the option that user choose */
			UART_sendByte('+');

			/*receive response from ECU if password correct or not*/
			receivedByte = UART_recieveByte();
			/*if password is correct*/
			if (receivedByte == UNLOCKING_DOOR)
			{
				/*call open door function*/
				DoorOpeningTask(); /* start displaying door status on LCD */

			}
			/*if password is wrong*/
			else if (receivedByte == WRONG_PASSWORD)
			{
				/*clear screen*/
				LCD_clearScreen();
				/*display on LCD*/
				LCD_displayString("Wrong pass");
				/*message delay*/
				_delay_ms(DISPLAY_MESSAGE_DELAY);
			}
			/*display main option*/
			appMainOptions();

		}
		/*if user choose to change password*/
		else if (key == '-')
		{
			/*clear screen*/
			LCD_clearScreen();
			/*display on LCD*/
			LCD_displayString("Plz enter pass:");
			/*get ppassword from user*/
			getPassword(g_inputPassword);
			/* inform Control ECU to start sending */
			UART_sendByte(READY_TO_SEND);
			/*sending password via UART*/
			sendPasswordViaUART(g_inputPassword);

			/* inform ECU the option that user chose */
			UART_sendByte(CHANGE_PASSWORD_OPTION);

			/*if UART recieve to change paassword*/
			receivedByte = UART_recieveByte();
			if (receivedByte == CHANGING_PASSWORD)
			{
				/*initialize password*/
				initializePassword();
				/*clear screen*/
				LCD_clearScreen();
			}
			/*if UART recieve its wrong password*/
			else if (receivedByte == WRONG_PASSWORD)
			{
				/*clear screen*/
				LCD_clearScreen();
				/*display on LCD*/
				LCD_displayString("Wrong pass");
				/*message delay*/
				_delay_ms(DISPLAY_MESSAGE_DELAY);
			}
			/*display main option*/
			appMainOptions();
		}
	}
}




/*A function to get password from user*/
void getPassword(uint8 * arrayName){
/*move to next line*/
	LCD_moveCursor(1, 0);
	uint8 i=0;
	uint8 key;
/*looping till getting password from user*/
	while(i!=PASSWORD_LENGTH){
		/*get number from keypad*/
		key = KEYPAD_getPressedKey();
		if (key >= 0 && key <= 9) {
			/*display * for the number*/
			LCD_displayCharacter('*');
			/*increament to next number*/
			*(arrayName + i) = key;
			i++;
		}
		/*keypad delay*/
		_delay_ms(KEYPAD_INPUT_DELAY);
	}
	key=0;

	/*wait till user choose 'on' button*/
	while(KEYPAD_getPressedKey() != 13);
}

/*A function to display app main options*/
void appMainOptions(void){
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "+: open door");
	LCD_displayStringRowColumn(1, 0, "-: change pass");
}
/*A function to initialize password*/
void initializePassword(void){

	while(g_password_match_status == PASSWORD_MISMATCHED){
		/*clear screen*/
		LCD_clearScreen();
		/*display on LCD*/
		LCD_displayString("Plz enter pass:");
		/*move to next line*/
		LCD_moveCursor(1, 0);
		/*getting password from user*/
		getPassword(g_inputPassword); /* get the password from user */

		 /* wait for a response from ECU*/
		UART_sendByte(READY_TO_SEND);
		while (UART_recieveByte() != READY_TO_RECEIVE);
		/*sending password via UART*/
		sendPasswordViaUART(g_inputPassword);

		/*clear screen*/
		LCD_clearScreen();
		/*display on LCD*/
		LCD_displayStringRowColumn(0, 0, "Re-enter pass");
		/*move to next line*/
		LCD_moveCursor(1, 0);
		/*getting password from user*/
		getPassword(g_inputPassword);
		 /* wait for a response from ECU*/
		UART_sendByte(READY_TO_SEND);
		while (UART_recieveByte() != READY_TO_RECEIVE);
		/* get confirm password from user */
		sendPasswordViaUART(g_inputPassword);


		/* wait for a response from ECU about passwords matching or not */
		while (UART_recieveByte() != READY_TO_SEND);
		g_password_match_status = UART_recieveByte();

		/*if passwords are not matching*/
		if (g_password_match_status == PASSWORD_MISMATCHED){
			/*clear screen*/
			LCD_clearScreen();
			/*Display on LCD*/
			LCD_displayString("Pass mismatch");
			/*message delay*/
			_delay_ms(DISPLAY_MESSAGE_DELAY);
		}
	}
	/* reset password match*/
	g_password_match_status = PASSWORD_MISMATCHED;
}


/*A call back function*/
void timerCallBack(void){
	g_seconds++;
}

/*A function to open door*/
void DoorOpeningTask(void){
	/* hold the system for 15 seconds & display to user that door is unlocking */
	g_seconds = 0;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Door is unlocking");
	while (g_seconds < DOOR_UNLOCKING_PERIOD);

	/* let the door be open for 3 seconds */
	g_seconds = 0;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Door is now open");
	while (g_seconds < DOOR_LEFT_OPEN_PERIOD);

	/* hold the system for 15 seconds & display to user that door is locking */
	g_seconds = 0;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Door is locking");
	while (g_seconds < DOOR_UNLOCKING_PERIOD);
}
/*A function to send password via UART*/
void sendPasswordViaUART(uint8 * passwordArray){
	uint8 count;
	/*looping till password is sending*/
	for (count=0;count<PASSWORD_LENGTH;count++){
		UART_sendByte(passwordArray[count]);
		/*delay till password is send*/
		_delay_ms(100);
	}
}
