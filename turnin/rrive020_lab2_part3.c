/*	Author: Robert Rivera 
 *	Lab Section: 021
 *	Assignment: Lab #2  Exercise #3
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's pins as outputs. Initialize to 0s

	unsigned char cntavail; // Keeping track of number of available parking spaces
	unsigned char parkingSpace1 = 0x00; // PA0
	unsigned char parkingSpace2 = 0x00; // PA1
	unsigned char parkingSpace3 = 0x00; // PA2
	unsigned char parkingSpace4 = 0x00; // PA3

	while (1) {
		cntavail = 0x04; // Resets to prevent the number of parking availabilty to go below 0
		
		parkingSpace1 = PINA & 0x01;
		parkingSpace2 = PINA & 0x02;
		parkingSpace3 = PINA & 0x04;
		parkingSpace4 = PINA & 0x08;

		if (parkingSpace1 == 0x01) {
			cntavail--;
		}
		
		if (parkingSpace2 == 0x02) {
			cntavail--;
		}

		if (parkingSpace3 == 0x04) {
			cntavail--;
		}

		if (parkingSpace4 == 0x08) {
			cntavail--;
		}
		
		if (cntavail == 0x00) { // If parking lot is full, then set PC7 to 1
			cntavail = 0x80;
		}

		PORTC = cntavail;		
	}

	return 1;
}
