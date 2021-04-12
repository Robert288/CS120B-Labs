/*	Author: Robert Rivera 
 *	Lab Section: 021
 *	Assignment: Lab #3  Exercise #5
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
	DDRB = 0xFE; PORTB = 0x01; // Configure PB7..PB1 as output (initalize to 0s) and PB0 as input 
	DDRD = 0x00; PORTD = 0xFF; // Configure port D's pins as input

	unsigned char weightB = 0x00;
	unsigned char weightD = 0x00;
	unsigned char airbagStatus; 
	

        while (1) {
		weightB = PINB & 0x01; // PB0
		weightD = PIND;

		airbagStatus = 0x00;

		if ( (weightB == 0 && weightD >= 0x46) || (weightB == 1 && weightD >= 0x45) ) {
			airbagStatus = 0x02;
		} else if ( (weightB == 0 && weightD > 0x05) || (weightB == 1 && weightD > 4) ) {
			airbagStatus = 0x04;
		}
		
		PORTB = airbagStatus;
	}

	return 1;
}
