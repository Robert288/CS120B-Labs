/* Author: Robert Rivera (rrive020)
 * Lab Section: 021
 * Assignment: Lab #6  Exercise #2
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code is my own original work.
 *
 *  Demo Link:
 */

#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum ThreeLEDs_States {ThreeLEDs_Start, LED1, LED2, LED3, Middle, Pressed, Restart} ThreeLEDs_State;
	
unsigned char button = 0x00;
	
void TickFct_ThreeLEDs() {
	switch (ThreeLEDs_State) {
		case ThreeLEDs_Start:
			ThreeLEDs_State = LED1;
			break;
			
		case LED1:
			if (button) {
				ThreeLEDs_State = Pressed;
			} else {
				ThreeLEDs_State = LED2;
			}
			break;
			
		case LED2:
			if (button) {
				ThreeLEDs_State = Pressed;
			} else {
				ThreeLEDs_State = LED3;
			}
			break;
			
		case LED3:
			if (button) {
				ThreeLEDs_State = Pressed;
			} else {
				ThreeLEDs_State = Middle;
			}
			break;
			
		case Middle:
			if (button) {
				ThreeLEDs_State = Pressed;
			} else {
				ThreeLEDs_State = LED1;
			}
			break;
			
		case Pressed:
			if (button) {
				ThreeLEDs_State = Pressed;
			} else {
				ThreeLEDs_State = Restart;
			}
			break;
			
		case Restart:
			if (!button) {
				ThreeLEDs_State = Restart;
			} else {
				ThreeLEDs_State = LED1;
			}
			break;
	}
	
	switch (ThreeLEDs_State) {
		case LED1:
			PORTB = 0x01;
			break;
		
		case LED2:
		case Middle:
			PORTB = 0x02;
			break;
			
		case LED3:
			PORTB = 0x04;
			break;
			
		case Pressed:
		case Restart:
			break;
			
		default:
			break;
	}
}

int main(void) {
	DDRA - 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	TimerSet(30);
	TimerOn();
	ThreeLEDs_State = ThreeLEDs_Start;

	while (1) {
		button = ~PINA & 0x01;
		TickFct_ThreeLEDs();
		while (!TimerFlag);
		TimerFlag = 0;
	}

	return 1;
}
