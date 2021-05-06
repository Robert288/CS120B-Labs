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

enum Sound_States {Sound_Start, Off, WaitOn, On, WaitOff, Up, Down} Sound_State;
	
const double musicalNotes[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
unsigned char i;
unsigned char power = 0x00;
unsigned char up = 0x00;
unsigned char down = 0x00;
	
void TickFct_Sound() {
	switch (Sound_State) {
		case Sound_Start:
			Sound_State = Off;
			break;
		
		case Off:
			i = 0x00;
			if (power) {
				Sound_State = WaitOn;
			} else {
				Sound_State = Off;
			}
			break;
			
		case WaitOn:
			if (!power) {
				Sound_State = On;
			} else {
				Sound_State = WaitOn;
			}
			break;
			
		case On:
			if (up && (i < 7)) {
				i = i + 1;
				Sound_State = Up;
			} else if (down && (i > 0)) {
				i = i - 1;
				Sound_State = Down;
			} else if (power) {
				Sound_State = WaitOff;
			} else {
				Sound_State = On;
			}
			break;

		case WaitOff:
			if (!power) {
				Sound_State = Off;
			} else {
				Sound_State = WaitOff;
			}
			break;
			
		case Up:
			if (!up) {
				Sound_State = On;
			} else {
				Sound_State = Up;
			}
			break;
			
		case Down:
			if (!down) {
				Sound_State = On;
			} else {
				Sound_State = Down;
			}
			break;
	}
	
	switch (Sound_State) {
		case Off:
			set_PWM(0.0);
			break;
			
		case On:
			set_PWM(musicalNotes[i]);
			break;
			
		case WaitOn:
		case WaitOff:
		case Up:
		case Down:
			break;
			
		default:
			break;
	}
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	PWM_on();
	Sound_State = Sound_Start;

  while (1) {
		power = ~PINA & 0x01;
		up = ~PINA & 0x02;
		down = ~PINA & 0x04;
		
		TickFct_Sound();
  }
	
	return 1;
}
