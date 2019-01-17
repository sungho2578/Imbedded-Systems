/*  Name & Email: Sungho Ahn & sahn025@ucr.edu
 *	Partner Name: Than Phan & tphan029@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 4 Exercise 1
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
enum States{start, led01, led10} state;

unsigned char button = 0x00;
unsigned char tmpB = 0x00;

void Tick(){	
	
	button = PINA & 0x01;
	
	switch(state){
		case start:
			state = led01;
			break;
			
		case led01:
			if (button){
				state = led10;
			}
			else if (!button){
				state = led01;
			}
			break;
			
		case led10:
			if (button){
				state = led01;
			}
			else if (!button){
				state = led10;
			}
			break;
			
		default:
			state = start;
			break;
	}
	
	switch(state){
		case start:
			break;
				
		case led01:
			tmpB = 0x01;
			PORTB = tmpB;
			break;
				
		case led10:
			tmpB = 0x02;
			PORTB = tmpB;
			break;
				
		default:
			break;
	}
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	state = start;
	
    while (1) 
    {	
		Tick();
    }
}

