/* Author: Robert Rivera (rrive020)
 * Lab Section: 021
 * Assignment: Lab #8  Exercise #1
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code is my own original work.
 *
 *  Demo Link: https://youtu.be/yOzMxDI_jeA
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

// 0.954 hz is lowest frequency possible with this function,
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } // stops timer/counter
		else { TCCR3B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 Hz is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		
		// prevents OCR3A from underflowing, using prescaler 64
		// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR3A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR3A = (short) (8000000 / (128 * frequency)) - 1; }
		
		TCNT3 = 0; // resets counter
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
		// COM3A0: Toggle PB3 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
		// WGM32: When counter (TCNT3) matches OCR3A, reset counter
		// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum Sound_States {Sound_Start, Wait, C4_Sound, D4_Sound, E4_Sound} Sound_State;
	
unsigned char C_4 = 0x00;
unsigned char D_4 = 0x00;
unsigned char E_4 = 0x00;

void TickFct_Sound() {
	switch (Sound_State) {
		case Sound_Start:
			Sound_State = Wait;
			break;
			
		case Wait:
			if (C_4 && !D_4 && !E_4) {
				Sound_State = C4_Sound;
			} else if (!C_4 && D_4 && !E_4) {
				Sound_State = D4_Sound;
			} else if (!C_4 && !D_4 && E_4) {
				Sound_State = E4_Sound;
			} else {
				Sound_State = Wait;
			}
			break;
			
		case C4_Sound:
			if (C_4 && !D_4 && !E_4) {
				Sound_State = C4_Sound;
			} else {
				Sound_State = Wait;
			}
			break;
			
		case D4_Sound:
			if (!C_4 && D_4 && !E_4) {
				Sound_State = D4_Sound;
			} else {
				Sound_State = Wait;
			}
			break;
			
		case E4_Sound:
			if (!C_4 && !D_4 && E_4) {
				Sound_State = E4_Sound;
			} else {
				Sound_State = Wait;
			}
			break;
	}
	
	switch (Sound_State) {
		case Wait:
			set_PWM(0);
			break;
			
		case C4_Sound:
			set_PWM(261.63);
			break;
			
		case D4_Sound:
			set_PWM(293.66);
			break;
			
		case E4_Sound:
			set_PWM(329.63);
			break;
			
		default:
			break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x40; PORTB = 0x00;
	
	PWM_on();
	Sound_State = Sound_Start;

  while (1) {
		C_4 = ~PINA & 0x01;
		D_4 = ~PINA & 0x02;
		E_4 = ~PINA & 0x04;
		
		TickFct_Sound();
  }
	
	return 1;
}
