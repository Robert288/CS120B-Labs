/*	Author: Robert Rivera 
 *	Lab Section: 021
 *	Assignment: Lab #3  Exercise #4
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
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's pins as output. Initialize to 0s
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's pins as output. Initialize to 0s

	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A

        while (1) {
		tmpA = PINA;

		PORTB = (tmpA >> 4) & 0x0F; // PA7..PA4 to PB3..PB0
		PORTC = (tmpA << 4) & 0xF0; // PA3..PA0 to PB7..PB4
	}

	return 1;
}
