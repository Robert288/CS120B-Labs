/* Author: Robert Rivera (rrive020)
 * Lab Section: 021
 * Assignment: Lab #7  Exercise #4
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code is my own original work.
 *
 *  Demo Link:
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion
	// ADSC: setting this bit starts the first conversion
	// ADATE: setting this bit enables auto-triggering. Since we are
	//		  in Free Running Mode, a new conversion will trigger whenever
	//		  the previous conversion completes.
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	unsigned short x = 0x00;
	
	ADC_init();

  while (1) {
		x = ADC;
		
		if (x >= 367) {
			PORTB = 0xFF;
		} else if (x >= (367/2)) {
			PORTB = 0X7F;
		} else if (x >= (367/3)) {
			PORTB = 0x3F;	
		} else if (x >= (367/4)) {
			PORTB = 0x1F;	
		} else if (x >= (367/5)) {
			PORTB = 0x0F;
		} else if (x >= (367/6)) {
			PORTB = 0x07;	
		} else if (x >= (367/7)) {
			PORTB = 0x03;
		} else {
			PORTB = 0x01;
		}
  }
	
	return 1;
}
