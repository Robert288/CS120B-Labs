/* Author: Robert Rivera (rrive020)
 * Lab Section: 021
 * Assignment: Lab #5  Exercise #3
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code is my own original work.
 *
 *  Demo Link: https://youtu.be/Ly4P0CBygA8
 */


#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum Lights_States {Lights_Start, Beginning, Wait, Middle, Wait1, End, Wait2} Lights_State;

void TickFct_Lights() {
	switch (Lights_State) {
		case Lights_Start:
			Lights_State = Beginning;
			break;

		case Beginning:
			if (~PINA & 0x01) {
				Lights_State = Wait;
			} else {
				Lights_State = Beginning;
			}
			break;

		case Wait:
			if (!(~PINA & 0x01)) {
				Lights_State = Middle;
			} else {
				Lights_State = Wait;
			}
			break;

		case Middle:
			if (~PINA & 0x01) {
				Lights_State = Wait1;
			} else {
				Lights_State = Middle;
			}
			break;

		case Wait1:
			if (!(~PINA & 0x01)) {
				Lights_State = End;
			} else {
				Lights_State = Wait1;
			}
			break;

		case End:
			if (~PINA & 0x01) {
				Lights_State = Wait2;
			} else {
				Lights_State = End;
			}
			break;

		case Wait2:
			if (!(~PINA & 0x01)) { 
				Lights_State = Beginning;
			} else {
				Lights_State = Wait2;
			}
			break; 
	}

	switch (Lights_State) {
		case Beginning:
			PORTB = 0x21;
			break;

		case Wait:
			PORTB = 0x12;
			break;

		case Middle:
			break;

		case Wait1:
			PORTB = 0x0C;

		case End:
			break;

		case Wait2:
			PORTB = 0x21;
			break;

		default:
			break;				
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	Lights_State = Lights_Start;

	while (1) {
		TickFct_Lights();
	}

	return 1;
}
