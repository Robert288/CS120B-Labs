/*	Author: Robert Rivera 
 *	Lab Section: 021
 *	Assignment: Lab #4  Exercise #3
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum Keypad_States {Keypad_Start, DoorLocked, PoundPressed, Wait, DoorUnlocked} Keypad_State;

unsigned char buttonX = 0x00;
unsigned char buttonY = 0x00;
unsigned char buttonP = 0x00;
unsigned char buttonI = 0x00;
unsigned char doorStatus = 0x00;
unsigned char currentState;

void TickFct_Keypad() {
	switch (Keypad_State) {
		case Keypad_Start:
			Keypad_State = DoorLocked;
			break;

		case DoorLocked:
			if (!buttonX && !buttonY && buttonP) {
				Keypad_State = PoundPressed;
			} else {
				Keypad_State = DoorLocked;
			}
			break;

		case PoundPressed:
                        if (!buttonX && !buttonY && !buttonP) {
                                Keypad_State = Wait;
                        } else if (!buttonX && !buttonY && buttonP) {
                                Keypad_State = PoundPressed;
                        } else {
                                Keypad_State = DoorLocked;
                        }
			break;

		case Wait:
			if (!buttonX && buttonY && !buttonP) {
				Keypad_State = DoorUnlocked;
			} else if (!buttonX && !buttonY && !buttonP) {
				Keypad_State = Wait;
			} else {
				Keypad_State = DoorLocked;
			}
			break;

		case DoorUnlocked:
			if (buttonI && !buttonX && !buttonY && !buttonP) {
				Keypad_State = DoorLocked;
			} else {
				Keypad_State = DoorUnlocked;
			}
			break;
	}

	switch (Keypad_State) {
		case DoorLocked:
			doorStatus = 0x00;
			currentState = DoorLocked;
			break;

		case PoundPressed:
			currentState = PoundPressed;
			break;

		case Wait:
			currentState = Wait;
			break;

		case DoorUnlocked:
			doorStatus = 0x01;
			currentState = DoorUnlocked;
			break;
		
		default:
			break;	
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;

	Keypad_State = Keypad_Start;	

        while (1) {
		buttonX = PINA & 0x01; // PA0
		buttonY = PINA & (0x01 << 1); // PA1
		buttonP = PINA & (0x01 << 2); // PA2
		buttonI = PINA & (0x01 << 7); // PA7

		TickFct_Keypad();

		PORTB = doorStatus;
		PORTC = currentState;
	}

	return 1;
}
