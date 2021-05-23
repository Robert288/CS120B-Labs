/* Author: Robert Rivera (rrive020)
 * Lab Section: 021
 * Assignment: Lab #10  Exercise #2
 * Exercise Description: [optional - include for your own benefit]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code is my own original work.
 *
 *  Demo Link: https://youtu.be/n4P10mHTSIQ
 */

#include <avr/io.h>
#include "keypad.h"
#include "scheduler.h"
#include "timer.h"

#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

task tasks[2];

const unsigned char tasksNum = 2;
const unsigned long tasksPeriodGCD = 10;
const unsigned long periodKeypad = 10;
const unsigned long periodSequence = 10;

unsigned char x = 0x00;
unsigned char j;
unsigned char keypad = 0x00;
unsigned char insideButton = 0x00;
unsigned char sequence[12] = {0x0F, 0x1F, 0x01, 0x1F, 0x02, 0x1F, 0x03, 0x1F, 0x04, 0x1F, 0x05, 0x1F};

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
			break;
	}
	
	switch (state) {
		case Keypad:
			switch (x) {
				case '\0': keypad = 0x1F; break;
				case '1': keypad = 0x01; break;
				case '2': keypad = 0x02; break;
				case '3': keypad = 0x03; break;
				case '4': keypad = 0x04; break;
				case '5': keypad = 0x05; break;
				case '6': keypad = 0x06; break;
				case '7': keypad = 0x07; break;
				case '8': keypad = 0x08; break;
				case '9': keypad = 0x09; break;
				case 'A': keypad = 0x0A; break;
				case 'B': keypad = 0x0B; break;
				case 'C': keypad = 0x0C; break;
				case 'D': keypad = 0x0D; break;
				case '*': keypad = 0x0E; break; 
				case '0': keypad = 0x00; break;
				case '#': keypad = 0x0F; break;
				default: keypad = 0x1B; break; // Should never occur. Middle LED off.
			}
			break;
		
		default:
			break;
	}
	
	return state;
}

enum Sequence_States {Sequence_Start, DoorLocked, PoundPressed, Sequence, DoorUnlocked};
int TickFct_Sequence(int state) {
	insideButton = ~PINB & (0x01 << 7);
	
	switch (state) {
		case Sequence_Start:
			state = DoorLocked;
			break;
		
		case DoorLocked:
			j = 0x00;
			if (keypad == sequence[j]) {
				state = PoundPressed;
			} else {
				state = DoorLocked;
			}
			break;
			
		case PoundPressed:
			j = 0x00;
			if (keypad == sequence[j]) {
				state = PoundPressed;
			} else if (keypad == sequence[j + 1]) {
				j++;
				state = Sequence;
			} else {
				state = DoorLocked;
			}
			break;
			
		case Sequence:
			if (keypad == sequence[j]) {
				state = Sequence;
			} else if (j + 1 == 11) {
				j++;
				if (keypad == sequence[j]) {
					state = DoorUnlocked;
				} else if (keypad == 0x0F) {
					state = PoundPressed;
				} else {
					state = DoorLocked;
				}
			} else if (j < 11) {
				j++;
				if (keypad == sequence[j]) {
					state = Sequence;
				} else if (keypad == 0x0F) {
					state = PoundPressed;
				} else {
					state = DoorLocked;
				}
			}
			break;
			
		case DoorUnlocked:
			if (insideButton) {
				state = DoorLocked;
			} else {
				state = DoorUnlocked;
			}
			break;
		
		default:
			state = Sequence_Start;
			break;
	}
	
	switch (state) {
		case DoorLocked:
		case PoundPressed:
			PORTB = 0x00;
			break;			
					
		case DoorUnlocked:
			PORTB = 0x01;
			break;

		case Sequence:
		default:
			break;
	}
	
	return state;
}

int main(void) {
	unsigned char i;
	
	DDRB = 0x7F; PORTB = 0x80;
	DDRC = 0xF0; PORTC = 0x0F;
	
	tasks[0].state = Keypad_Start; 
	tasks[0].period = periodKeypad;
	tasks[0].elapsedTime = tasks[0].period;
	tasks[0].TickFct = &TickFct_Keypad;
	
	tasks[1].state = Sequence_Start;
	tasks[1].period = periodSequence;
	tasks[1].elapsedTime = tasks[1].period;
	tasks[1].TickFct = &TickFct_Sequence;
	
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
