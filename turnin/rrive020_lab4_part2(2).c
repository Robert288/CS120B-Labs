/*	Author: Robert Rivera
 *	Lab Section: 021
 *	Assignment: Lab 4  Exercise 2
 *	
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum Button_States {Button_Start, Initial, Increment, Reset, Decrement, Wait1, Wait2} Button_State;

unsigned char incrementButton = 0x00;
unsigned char decrementButton = 0x00;
unsigned char tmpC = 0x00;

void TickFCT_Button() {
  switch(Button_State) {
    case Button_Start:
			Button_State = Initial;
			break;
      
		case Initial:
		case Reset:
			if (incrementButton && decrementButton) {
				Button_State = Reset;
			} else if (!incrementButton && decrementButton) {
				Button_State = Decrement;
			} else if (incrementButton && !decrementButton) {
				Button_State = Increment;
			} else if (!incrementButton && !decrementButton) {
				Button_State = Initial;
			}
			break;
 
		case Increment:
		case Decrement:
			if (incrementButton && decrementButton) {
				Button_State = Reset;
			} else {
				Button_State = Wait2;
			}
			break;
  
		case Wait1:
			if (incrementButton && decrementButton) {
				Button_State = Reset;
			} else if (incrementButton && !decrementButton) {
				Button_State = Increment;
			} else if (!incrementButton && decrementButton) {
				Button_State = Decrement;
			} else {
				Button_State = Wait1;
			}
			break;
      
		case Wait2:
			if (button1 && button2) {
				Button_State = Reset;
			} else if (!button1 && !button2) {
				Button_State = Wait1;
			} else {
				Button_State = Wait2;
			}
			break;
      
		default:
			break;
	}
  
  switch(Button_State) {
		case Init:
			break;
      
		case Increment:
			if (tmpC < 9) {
				tmpC = tmpC + 1;
			}
			break;
      
		case Wait1:
			break;
      
		case Reset:
			tmpC = 0;
			break;
      
		case Wait2:
			break;
      
		case Decrement:
			if(tmpC > 0) {
				tmpC = tmpC - 1;
			}
			break;
      
		default:
			break;
	}
}

int main(void) {
  DDRA = 0x00; PORTA = 0xFF;
  DDRC = 0xFF; PORTC = 0x00;
	
	tmpC = 0x07;
  Button_State = Button_Start; // Indicates initial call

  while (1) {
    incrementButton = PINA & 0x01;
    decrementButton = PINA & 0x02;
    decrementButton = (decrementButton >> 1);
    TickFCT_Button();
		PORTC = tmpC;
  }
  
  return 1;
}
