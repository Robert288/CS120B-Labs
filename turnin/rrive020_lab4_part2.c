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

enum Button_States {Button_Start, Initial, Increment, Decrement, Reset, Wait, Wait1, Wait2} Button_State;

unsigned char incrementButton = 0x00;
unsigned char decrementButton = 0x00;
unsigned char tmpC = 0x00;

void TickFct_Button() {
	switch (Button_State) {
		case Button_Start:
			Button_State = Initial;
			break;

		case Initial:
			if (incrementButton && decrementButton) {
				Button_State = Reset;
			} else if (incrementButton && !decrementButton) {
				Button_State = Increment;
			} else if (!incrementButton && decrementButton) {
				Button_State = Decrement;
			} else {
				Button_State = Initial;
			}
			break;

		case Increment:
		case Wait1:
			if (incrementButton && decrementButton) {
				Button_State = Reset;
			} else if (!incrementButton && !decrementButton) {
				Button_State = Initial;
			} else if (!incrementButton && decrementButton) {
				Button_State = Decrement;
			} else {
				Button_State = Wait1;
			}
			break;

		case Decrement:
		case Wait2:
			if (incrementButton && decrementButton) {
                                Button_State = Reset;
                        } else if (!incrementButton && !decrementButton) {
                                Button_State = Initial;
                        } else if (incrementButton && !decrementButton) {
                                Button_State = Increment;
                        } else {
                                Button_State = Wait2;
                        }		
			break;

		/* case Reset:
		case Wait:
                        if (!incrementButton && !decrementButton) {
                                Button_State = Initial;
                        } else {
                                Button_State = Wait;
                        }
			break; */
		
		case Reset: 
                        if (!incrementButton && !decrementButton) {
                                Button_State = Initial;
                        } else {
                                Button_State = Wait;
                        }
			break;
			
		case Wait: 
                        if (!incrementButton && !decrementButton) {
                                Button_State = Initial;
                        } else if (incrementButton && !decrementButton) {
                                Button_State = Increment;
                        } else if (!incrementButton && decrementButton) {
				Button_State = Decrement;
			} else {
                                Button_State = Wait;
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

		case Wait:
			break;

		case Wait1:
			break;

		case Wait2:
			break;

		default:
			break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's pins as input 
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's pins as output. Initialize to 0s

	Button_State = Button_Start; // Indicates initial call
	tmpC = 0x07;

        while (1) {
		incrementButton = PINA & 0x01;
		decrementButton = PINA & 0x02;
		decrementButton = (decrementButton >> 1);
		TickFct_Button();
		PORTC = tmpC;
	}

	return 1;
}
