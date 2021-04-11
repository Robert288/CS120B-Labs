/*	Author: Robert Rivera 
 *	Lab Section: 021
 *	Assignment: Lab #3  Exercise #1
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
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's pins as outputs. Initialize to 0s

	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	unsigned char tmpB = 0x00; // Temporary variable to hold the value of B
	unsigned char currBit = 0x00;
	unsigned char count; // Keep track of the number of 1s
	unsigned char i;

        while (1) {
                tmpA = PINA;
                tmpB = PINB;

                count = 0x00;

                for (i = 0; i < 8; ++i) {
			currBit = (tmpA >> i) & 0x01;
			if (currBit != 0) {
				count++;
			}

			currBit = (tmpB >> i) & 0x01;
			if (currBit != 0) {
				count++;
			}
		}

		PORTC = count;
	}

	return 1;
}
