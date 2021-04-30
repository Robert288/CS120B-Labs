/* Author: Robert Rivera (rrive020)
 * Lab Section: 021
 * Assignment: Lab #6  Exercise #1
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

enum ThreeLEDs_States {ThreeLEDs_Start, LED1, LED2, LED3} ThreeLEDs_State;
	
void TickFct_ThreeLEDs() {
	switch (ThreeLEDs_State) {
		case ThreeLEDs_Start:
			ThreeLEDs_State = LED1;
			break;
			
		case LED1:
			ThreeLEDs_State = LED2;
			break;
			
		case LED2:
			ThreeLEDs_State = LED3;
			break;
			
		case LED3:
			ThreeLEDs_State = LED1;
			break;
	}
	
	switch (ThreeLEDs_State) {
		case LED1:
			PORTB = 0x01;
			break;
		
		case LED2:
			PORTB = 0x02;
			break;
			
		case LED3:
			PORTB = 0x04;
			break;
			
		default:
			break;
	}
}

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;

	TimerSet(100); // Setting it to 1000 did not match the pace of the example demo video.
					       // Used 100 to match with the demo video
	TimerOn();
	ThreeLEDs_State = ThreeLEDs_Start;

	while (1) {
		TickFct_ThreeLEDs();
		while (!TimerFlag);
		TimerFlag = 0;
	}

	return 1;
}
