/*  Name & Email: Sungho Ahn & sahn025@ucr.edu
 *	Partner Name: Than Phan & tphan029@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 4 Exercise 5
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>

enum States {Start, Lock, PressNum, PressX, PressY, Unlock} state;
char counter = 0;
void tick()
{
	switch (state) {
		case Start:
		state = Lock;
		break;
		case Lock:
		if (PINA & 0x04) {
			state = PressNum;
			counter += 1;
		}
		else {
			state = Lock;
			counter = 0;
		}
		break;
		case PressNum:
		if (PINA & 0x01) {
			state = PressX;
			counter += 1;
		}
		else {
			state = Lock;
		}
		break;
		case PressX:
		if ((counter >= 3) && (PINA & 0x01) && (PORTB & 0x01)) {
			state = Lock;
		}
		else if ((counter >= 3) && (PINA & 0x01) && !(PORTB & 0x01)) {
			state = Unlock;
		}
		else if ((counter < 3) && (PINA & 0x02)) {
			state = PressY;
			++counter;
		}
		else {
			state = Lock;
		}
		break;
		case PressY:
		if ((PINA & 0x01) && !(PORTB & 0x01)) {
			state = Unlock;
			++counter;
		}
		else if ((PINA & 0x01) && (PORTB & 0x01)) {
			state = Lock;
			counter = 0;
		}
		else {
			state = Lock;
		}
		break;
		case Unlock:
		counter = 0;
		if (PINA & 0x04) {
			state = PressNum;
		}
		else if (PINA & 0x80) {
			state = Lock;
			counter = 0;
		}
		else {
			state = Unlock;
		}
		break;
		default:
		break;
		
	}
	switch (state) {
		case  Start:
		break;
		case Lock:
		PORTB = 0x00;
		break;
		case PressNum:
		break;
		case PressX:
		break;
		case PressY:
		break;
		case Unlock:
		PORTB = 0x01;
		break;
		default:
		break;
	}
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0x00;
	state = Start;
	/* Replace with your application code */
	while (1)
	{
		tick();
	}
}
