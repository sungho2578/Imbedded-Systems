/*  Name & Email: Sungho Ahn & sahn025@ucr.edu
 *	Partner Name: Than Phan & tphan029@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 4 Exercise 3
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>
enum States{start, locked, wait, unlock} state;

unsigned char ButtonX;
unsigned char ButtonY;
unsigned char ButtonP;
unsigned char ButtonLock;
unsigned temB;

void Tick(){
	ButtonX = PINA & 0x01;
	ButtonY = PINA & 0x02;
	ButtonP = PINA & 0x04;
	ButtonLock = PINA & 0x80;

	switch(state){
		case start:
			state = locked;
			break;
		case locked:
			if (!ButtonX && !ButtonY && ButtonP && !ButtonLock){
				state = wait;
			}
			else 
				state = locked;
			break;
		case wait:
			if (!ButtonX && !ButtonY && ButtonP && !ButtonLock){
				state = wait;
			}
			else if (!ButtonX && ButtonY && !ButtonP && !ButtonLock){
				state = unlock;
			}
			else
				state = locked;
			break;
		case unlock:
			if (ButtonLock){
				state = locked;
			}
			else
				state = unlock;
			break;
	}

	switch(state){
		case start:
			break;	
		case locked:
			temB = 0x00;
			PORTB = temB;
			break;
		case wait:
			break;
		case unlock:
			temB = 0x01;
			PORTB = temB;
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