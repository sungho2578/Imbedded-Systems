/*  Name & Email: Sungho Ahn & sahn025@ucr.edu
 *	Partner Name: Than Phan & tphan029@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 4 Exercise 2
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
enum States{start, wait, inc, dec, reset} state;

unsigned char button0 = 0x00;
unsigned char button1 = 0x00;
unsigned char cnt = 0x00;

void Tick(){	
	
	button0 = PINA & 0x01;
	button1 = PINA & 0x02;
	
	switch(state){
		case start:
			state = wait;
			break;
			
		case wait:
			if (button0 && !button1){
				state = inc;
			}
			else if (!button0 && button1){
				state = dec;
			}
			else if (button0 && button1){
				state = reset;
			}
			else
				state = wait;
			break;
		
		case inc:
			if (button0 && !button1){
				state = inc;
			}
			else if (!button0 && button1){
				state = dec;
			}
			else if (button0 && button1){
				state = reset;
			}
			else
				state = wait;
			break;
				
		case dec:
			if (!button0 && button1){
				state = dec;
			}
			else if (button0 && !button1){
				state = inc;
			}
			else if (button0 && button1){
				state = reset;
			}
			else
				state = wait;
			break;
				
		case reset:
			if (button0 && !button1){
				state = inc;
			}
			else if (!button0 && button1){
				state = dec;
			}
			else if (button0 && button1){
				state = reset;
			}
			else
				state = wait;
			break;
			
		default:
			state = start;
			break;
	}
			
	switch(state){
		case start:
			break;
			
		case wait:
			break;
			
		case inc:
			if (cnt < 9){
				cnt += 1;
			}
			PORTC = cnt;
			break;
			
		case dec:
			if (cnt > 0){
				cnt -= 1;
			}
			PORTC = cnt;
			break;
			
		case reset:
			cnt = 0;
			PORTC = cnt;
			break;
	}
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	cnt = 7;
	PORTC = cnt;
	state = start;
	
    while (1) 
    {	
		Tick();
    }
}

