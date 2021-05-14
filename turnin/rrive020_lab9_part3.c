/* Author: Robert Rivera (rrive020)
 * Lab Section: 021
 * Assignment: Lab #9  Exercise #3
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

unsigned char threeLEDs = 0x00;
unsigned char blinkingLED = 0x00;
unsigned char sound = 0x00;
unsigned char button = 0x00;

enum ThreeLEDs_States {ThreeLEDs_Start, LED1, LED2, LED3} ThreeLEDs_State;
enum BlinkingLED_States {BlinkingLED_Start, On, Off} BlinkingLED_State;
enum Sound_States {Sound_Start, Sound_Off, Sound_On, Sound_Continue} Sound_State;

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

void TickFct_SoundSM() {
	button = ~PINA & 0x04;
	
	switch (Sound_State) {
		case Sound_Start:
			Sound_State = Sound_Off;
			break;
		
		case Sound_Off:
			if (button) {
				Sound_State = Sound_On;
			} else {
				Sound_State = Sound_Off;
			}
			break;
		
		case Sound_On:
			if (button) {
				Sound_State = Sound_Continue;
			} else {
				Sound_State = Sound_Off;
			}
			break;
		
		case Sound_Continue:
			if (button) {
				Sound_State = Sound_On;
			} else {
				Sound_State = Sound_Off;
			}
			break;
		
		default:
			break;
	}
	
	switch (Sound_State) {
		case Sound_Off:
		case Sound_Continue:
			sound = 0x00;
			break;
			
		case Sound_On:
			sound = 0x10;
			break;
			
		default:
			break;
	}
}

void TickFct_CombineLEDsSM() {
	PORTB = threeLEDs | blinkingLED | sound;
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	unsigned long ThreeLEDs_elapsedTime = 300;
	unsigned long BlinkingLED_elapsedTime = 1000;
	unsigned long Sound_elapsedTime = 2;
	const unsigned long timerPeriod = 1;
	
	PORTB = 0x00;
		
	TimerSet(timerPeriod);
	TimerOn();
	
	ThreeLEDs_State = ThreeLEDs_Start;
	BlinkingLED_State = BlinkingLED_Start;
	Sound_State = Sound_Start;
	
	while (1) {
		if (ThreeLEDs_elapsedTime >= 300) {
			TickFct_ThreeLEDsSM(); // Execute one tick of the ThreeLEDs synchSM
			ThreeLEDs_elapsedTime = 0;
		}
		
		if (BlinkingLED_elapsedTime >= 1000) {
			TickFct_BlinkingLEDSM(); // Execute one tick of the BlinkingLEDS synchSM
			BlinkingLED_elapsedTime = 0;
		}
		
		if (Sound_elapsedTime >= 2) {
			TickFct_SoundSM(); // Execute one tick of the Sound synchSM
			Sound_elapsedTime = 0;
		}
		
		TickFct_CombineLEDsSM();
		
		while (!TimerFlag); // Wait for timer period
		TimerFlag = 0; // Lower flag raised by timer
		ThreeLEDs_elapsedTime += timerPeriod;
		BlinkingLED_elapsedTime += timerPeriod;
		Sound_elapsedTime += timerPeriod;
	}
	
	return 1;
}
