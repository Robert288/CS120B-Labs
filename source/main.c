/*	Author: Robert Rivera 
 *	Lab Section: 021
 *	Assignment: Lab #4  Exercise #4
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum Keypad_States {Keypad_Start, DoorLocked, Sequence} Keypad_State;

unsigned char sequence[] = {0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01};
unsigned char i;


void TickFct_Keypad() {
	switch (Keypad_State) {
		case Keypad_Start:
			Keypad_State = DoorLocked;
			break;

		case DoorLocked:
			i = 0x00;
			if ((PINA & 0x07) == sequence[i]) {
				Keypad_State = Sequence;
			} else {
				Keypad_State = DoorLocked;
			}
			break;

		case Sequence:
			i++;
			if ((PINA & 0x07) == seqeunce[i]) {
				Keypad_State = DoorUnlocked;
			} else if (
				
			break;	
	}

	switch(Keypad_State) {
		case DoorLocked:
			PORTB = 0x00;
			break;
		
		case Sequence:
			PORTB = 0x01;
			break;

		default:
			break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	while (1) {
		TickFct_Keypad();		
	}

	return 1;
}
