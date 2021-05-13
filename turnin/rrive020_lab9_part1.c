/* Author: Robert Rivera (rrive020)
 * Lab Section: 021
 * Assignment: Lab #9  Exercise #1
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code is my own original work.
 *
 *  Demo Link: https://youtu.be/jamwoY_wzU4
 */

#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;

enum ThreeLEDs_States {ThreeLEDs_Start, LED1, LED2, LED3} ThreeLEDs_State;
enum BlinkingLED_States {BlinkingLED_Start, On, Off} BlinkingLED_State;

void TickFct_ThreeLEDsSM() {
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
			threeLEDs = 0x01;
			break;
		 case LED2:
			threeLEDs = 0x02;
			break;
		
		case LED3:
			threeLEDs = 0x04;
			break;
		
		default:
			break;
	}
}

void TickFct_BlinkingLEDSM() {
	switch (BlinkingLED_State) {
		case BlinkingLED_Start:
			BlinkingLED_State = On;
			break;
		
		case On:
			BlinkingLED_State = Off;
			break;
		
		case Off:
			BlinkingLED_State = On;
			break;
	}
	
	switch (BlinkingLED_State) {
		case On:
			blinkingLED = 0x08;
			break;
		
		case Off:
			blinkingLED = 0x00;
			break;
			
		default:
			break;
	}
}

void TickFct_CombineLEDsSM() {
	PORTB = threeLEDs | blinkingLED;
}

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	
	TimerSet(1000);
	TimerOn();
	
	ThreeLEDs_State = ThreeLEDs_Start;
	BlinkingLED_State = BlinkingLED_Start;
	
  while (1) {
		TickFct_ThreeLEDsSM();
		TickFct_BlinkingLEDSM();
		TickFct_CombineLEDsSM();
		while (!TimerFlag);
		TimerFlag = 0;
  }
}
