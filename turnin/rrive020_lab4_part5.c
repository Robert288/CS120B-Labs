/*	Author: Robert Rivera 
 *	Lab Section: 021
 *	Assignment: Lab #4  Exercise #5
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum Keypad_States {Keypad_Start, DoorLocked, Sequence, DoorUnlocked, Restart, Sequence1} Keypad_State;

unsigned char sequence[7] = {0x04, 0x00, 0x01, 0x00, 0x02, 0x00, 0x01};
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
			if ((PINA & 0x07) == sequence[i]) { 
				Keypad_State = Sequence;
			} else if (i + 1 == 6) {
                                i++;
                                if ((PINA & 0x07) == sequence[i]) {
                                        Keypad_State = DoorUnlocked;
                                } else {
                                        Keypad_State = DoorLocked;
                                }
			} else if (i < 6) {
				i++;
				if ((PINA & 0x07) == sequence[i]) { 
					Keypad_State = Sequence;
				} else {
					Keypad_State = DoorLocked;
				}	
			}
			break;	
		
		case DoorUnlocked:
			if ((PINA & 0x87) == 0x80) {
				Keypad_State = DoorLocked;
			} else if ((PINA & 0x87) == 0x00) {
				Keypad_State = Restart;
			} else {
				Keypad_State = DoorUnlocked;
			}		
			break;

		case Restart:
			if ((PINA & 0x87) == 0x80) {
				Keypad_State = DoorLocked;
			} else {
				i = 0x00;
	                        if ((PINA & 0x87) == sequence[i]) {
                                	Keypad_State = Sequence1;
                        	} else {
                                	Keypad_State = Restart;
                        	}
			}
			break;

		case Sequence1:
                        if ((PINA & 0x87) == sequence[i]) {
                                Keypad_State = Sequence1;
                        } else if (i + 1 == 6) {
                                i++;
                                if ((PINA & 0x87) == sequence[i]) {
                                        Keypad_State = DoorLocked;
                                } else {
                                        Keypad_State = Restart;
                                }
                        } else if (i < 6) {
                                i++;
                                if ((PINA & 0x87) == sequence[i]) {
                                        Keypad_State = Sequence1;
                                } else {
                                        Keypad_State = Restart;
                                }
                        }
                        break;
	}

	switch(Keypad_State) {
		case DoorLocked:
			PORTB = 0x00;
			break;
				
		case DoorUnlocked:
			PORTB = 0x01;
			break;
		
		case Sequence:
		case Restart:
		case Sequence1:
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
