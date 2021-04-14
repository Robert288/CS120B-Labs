/*	Author: Robert Rivera 
 *	Lab Section: 021
 *	Assignment: Lab #4  Exercise #2
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum Button_States {Button_Start, Initial, Increment, Decrement, Reset, Wait1, Wait2, Wait4} Button_State;

unsigned char incrementButton = 0x00;
unsigned char decrementButton = 0x00;
unsigned char tmpC = 0x00;

void TickFct_Button() {
	switch (Button_State) {
		case Button_Start:
			Button_State = Initial;
			break;

		case Initial:
			if (incrementButton && !decrementButton) {
				Button_State = Increment;
			} else if (!incrementButton && decrementButton) {
				Button_State = Decrement;
			} else if (incrementButton && decrementButton) {
				Button_State = Reset;
			} else if (!incrementButton && !decrementButton) {
				Button_State = Initial;
			}
			break;

		case Increment:
			if (incrementButton == 0 && decrementButton == 0) {
				Button_State = Wait4;
			} else if (incrementButton == 1 && decrementButton == 0) {
				Button_State = Wait1;
			} else if (incrementButton == 1 && decrementButton == 1) {
				Button_State = Reset;
			} 
			break;

		case Decrement:
			if (incrementButton == 0 && decrementButton == 0) {
				Button_State = Wait4;
			} else if (incrementButton == 0 && decrementButton == 1) {
				Button_State = Wait2;
			} else if (incrementButton == 1 && decrementButton == 1) {
				Button_State = Reset;
			} 
			break;

		case Reset:
			if (incrementButton == 0 && decrementButton == 0) {
				Button_State = Wait4;
			} else {
				Button_State = Reset;
			}
			break;

		case Wait1:
			if (incrementButton == 1 && decrementButton == 0) {
				Button_State = Wait1;
			} else if (incrementButton == 1 && decrementButton == 1) {
				Button_State = Reset;
			} else if (incrementButton == 0 && decrementButton == 0) {
				Button_State = Wait4;
			}
			break;

		case Wait2:
			if (incrementButton == 0 && decrementButton == 1) {
				Button_State = Wait2;
			} else if (incrementButton == 1 && decrementButton == 1) {
				Button_State = Reset;
			} else if (incrementButton == 0 && decrementButton == 0) {
				Button_State = Wait4;
			}
			break;

		case Wait4:
			if (incrementButton == 1 && decrementButton == 0) {
				Button_State = Increment;
			} else if (incrementButton == 0 && decrementButton == 1) {
				Button_State = Decrement;
			} else if (incrementButton == 1 && decrementButton == 1) {
				Button_State = Reset;
			} else {
				Button_State = Wait4;
			}
			break;
	}

	switch (Button_State) {
		case Initial:
			break;

		case Increment:
			if (tmpC < 9) {
				tmpC = tmpC + 1;
			}
			break;

		case Decrement:
			if (tmpC > 0) {
				tmpC = tmpC - 1;
			}
			break;

		case Reset:
			tmpC = 0;
			break;

		case Wait1:
			break;
		case Wait2:
			break;
		case Wait4:
			break;
		default:
			break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's pins as input 
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's pins as output. Initialize to 0s

	PORTC = 0x07;
	Button_State = Button_Start; // Indicates initial call
	
        while (1) {
		incrementButton = PINA & 0x01;
		decrementButton = PINA & 0x02;
		decrementButton = (decrementButton >> 1);
		TickFct_Button();
		PORTC = tmpC;
	}

	return 1;
}
