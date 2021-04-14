/*	Author: Robert Rivera 
 *	Lab Section: 021
 *	Assignment: Lab #4  Exercise #1
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum LED_States {LED_Start, LED1, Button_Pressed1, LED2, Button_Pressed2} LED_State;

unsigned char button = 0x00;
unsigned char tmpB = 0x00;

void TickFct_LED() {
	switch (LED_State) {
		case LED_Start:
			LED_State = LED1;
			break;

		case LED1:
			if (button == 0x01) {
				LED_State = Button_Pressed1;
			} else {
				LED_State = LED1;
			}
			break;

		case Button_Pressed1:
			if (button == 0x00) {
				LED_State = LED2;
			} else {
				LED_State = Button_Pressed1;
			}
			break;

		case LED2:
			if (button == 0x01) {
				LED_State = Button_Pressed2;
			} else {
				LED_State = LED2;
			}
			break;

		case Button_Pressed2:
			if (button == 0x00) {
				LED_State = LED1;
			} else {
				LED_State = Button_Pressed2;
			}
			break;

		default:
			LED_State = LED_Start;
			break;
	}

	switch (LED_State) {
		case LED1:
			tmpB = 0x01;
			break;

		case Button_Pressed1:
			tmpB = 0x02;
			break;

		case LED2:
			break;

		case Button_Pressed2:
			tmpB = 0x01;
			break;

		default:
			break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's pins as input 
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's pins as output. Initialize to 0s

	LED_State = LED_Start; // Indicates initial call
	
        while (1) {
		button = PINA & 0x01;
		TickFct_LED();
		PORTB = tmpB;
	}

	return 1;
}
