/* Author: Robert Rivera (rrive020)
 * Lab Section: 021
 * Assignment: Lab #10  Exercise #1
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code is my own original work.
 *
 *  Demo Link:
 */

#include <avr/io.h>
#include "keypad.h"
#include "scheduler.h"
#include "timer.h"

#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

task tasks[1];

const unsigned char tasksNum = 1;
const unsigned long tasksPeriodGCD = 10;
const unsigned long periodKeypad = 10;

unsigned char x = 0x00;

enum Keypad_States {Keypad_Start, Keypad};
int TickFct_Keypad(int state) {
	switch (state) {
		case Keypad_Start:
			state = Keypad;
			break;
		
		case Keypad:
			x = GetKeypadKey();
			
			state = Keypad;
			break;
		
		default:
			state = Keypad_Start;
	}
	
	switch (state) {
		case Keypad:
			switch (x) {
				case '\0': PORTB = 0x1F; break;
				case '1': PORTB = 0x81; break;
				case '2': PORTB = 0x82; break;
				case '3': PORTB = 0x83; break;
				case '4': PORTB = 0x84; break;
				case '5': PORTB = 0x85; break;
				case '6': PORTB = 0x86; break;
				case '7': PORTB = 0x87; break;
				case '8': PORTB = 0x88; break;
				case '9': PORTB = 0x89; break;
				case 'A': PORTB = 0x8A; break;
				case 'B': PORTB = 0x8B; break;
				case 'C': PORTB = 0x8C; break;
				case 'D': PORTB = 0x8D; break;
				case '*': PORTB = 0x8E; break; 
				case '0': PORTB = 0x80; break;
				case '#': PORTB = 0x8F; break;
				default: PORTB = 0x1B; break; // Should never occur. Middle LED off.
			}
			break;
		
		default:
			break;
	}
	
	return state;
}

int main(void) {
	unsigned char i;
	
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	
	tasks[0].state = Keypad_Start; 
	tasks[0].period = periodKeypad;
	tasks[0].elapsedTime = tasks[0].period;
	tasks[0].TickFct = &TickFct_Keypad;
	
	PORTB = 0x00;
	
	TimerSet(tasksPeriodGCD);
	TimerOn();
	
    while (1) {
		for (i = 0; i < tasksNum; ++i) {
			if (tasks[i].elapsedTime >= tasks[i].period) {
				tasks[i].state = tasks[i].TickFct(tasks[i].state);
				tasks[i].elapsedTime = 0;
			}
			tasks[i].elapsedTime += tasksPeriodGCD;
		}
		
		while (!TimerFlag);
		TimerFlag = 0;
	}
	
	return 1;
}
