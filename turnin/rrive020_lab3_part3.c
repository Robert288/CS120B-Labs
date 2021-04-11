/*	Author: Robert Rivera 
 *	Lab Section: 021
 *	Assignment: Lab #3  Exercise #3
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's pins as input
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's pins as outputs. Initialize to 0s

	unsigned char fuelLevel = 0x00;
	unsigned char fuelLight = 0x00;
	unsigned char tmpA = 0x00; // Temporary varaiable to hold the value of A

        while (1) {
		fuelLevel = PINA & 0x0F; // PA3..PA0
		tmpA = PINA & 0x70; // PA6..PA4

		if (fuelLevel == 0x00) {
			fuelLight = 0x40; // PC6 to 1
		} else if (fuelLevel == 0x01 || fuelLevel == 0x02) {
			fuelLight = 0x60; // PC6 PC5 to 1	
		} else if (fuelLevel == 0x03 || fuelLevel == 0x04) {
			fuelLight = 0x70; // PC6..PC4 to 1
		} else if (fuelLevel == 0x05 || fuelLevel == 0x06) {
			fuelLight = 0x38; // PC5..PC3 to 1
		} else if (fuelLevel == 0x07 || fuelLevel == 0x08 || fuelLevel == 0x09) {
			fuelLight = 0x3C; // PC5..PC2 to 1
		} else if (fuelLevel == 0x0A || fuelLevel == 0x0B || fuelLevel == 0x0C) {
			fuelLight = 0x3E; // PC5..PC1 to 1
		} else {
			fuelLight = 0x3F; // PC5..PC0 to 1
		}

		if ((tmpA >> 4) == 0x03) {
			PORTC = fuelLight | 0x80;  	
		} else {
			PORTC = fuelLight;
		}
	}

	return 1;
}
