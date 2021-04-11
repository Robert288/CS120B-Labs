/*	Author: Robert Rivera 
 *	Lab Section: 021
 *	Assignment: Lab #2  Exercise #4
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <stdlib.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's pins as inputs
        DDRB = 0x00; PORTB = 0xFF; // Configure port B's pins as inputs
        DDRC = 0x00; PORTC = 0xFF; // Configure port C's pins as inputs
        DDRD = 0xFF; PORTD = 0x00; // Configure port D's pins as outputs. Initialize to 0s

        unsigned char weight = 0x00;
        unsigned char seatA = 0x00;
        unsigned char seatB = 0x00;
        unsigned char seatC = 0x00;
        unsigned char tmpD = 0x00; // Temporary variable to hold the value of D

        while (1) {
                seatA = PINA;
                seatB = PINB;
                seatC = PINC;

                weight = seatA + seatB + seatC;

                if (weight >= 0x8C) { // If the cart's total passenger weight exceeds the maximum weight of 140 kgs, 
                        tmpD = tmpD | 0x01; // Then set PD0 to 1
		} else {
			tmpD = tmpD & 0xFE; // Else set PD0 to 0
		}

                if (abs(seatA - seatC) >= 0x50) { // If the difference between A and C exceeds 80 kgs,
                        tmpD = tmpD | 0x02; // Then set PD1 to 1
		} else {
			tmpD = tmpD & 0xFD; // Else set PD1 to 0
		}

		weight = weight & 0xFC; // Only need to represent the most significant bits

		PORTD = weight | tmpD; // PD7..PD2 represents an approximiation of the total demand weight
	}

	return 1;
}
