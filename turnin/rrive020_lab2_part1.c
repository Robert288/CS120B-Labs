/*	Author: Robert Rivera 
 *	Lab Section: 021
 *	Assignment: Lab #2  Exercise #1
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
	DDRB = 0xFF; PORTB = 0x00; // Configure port B's pins as outputs. Initialize to 0s

	unsigned char garageDoorSensor = 0x00; // PA0
	unsigned char lightSensor = 0x00; // PA1
	unsigned char led = 0x00; // PB0

	while (1) {
		garageDoorSensor = PINA & 0x01;
		lightSensor = PINA & 0x02;

		if (garageDoorSensor == 0x01 && lightSensor == 0x00) {
			led = 0x01;
		} else {
			led = 0x00;
		}

		PORTB = led;	
	}

	return 1;
}
