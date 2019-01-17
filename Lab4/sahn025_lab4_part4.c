/*  Name & Email: Sungho Ahn & sahn025@ucr.edu
 *	Partner Name: Than Phan & tphan029@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 4 Exercise 4
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>

enum States {Start, Lock, PressA2Unlock, Unlock, PressA2Lock} state;
void tick()
{
	switch (state)
	{
		case Start:
		state = Lock;
		break;
		
		case Lock:
		if (PINA & 0x04)
		state = PressA2Unlock;
		else
		state = Lock;
		
		case PressA2Unlock:
		if ((!(PINA & 0x04)) && (PINA & 0x02))
		state = Unlock;
		else
		state = PressA2Unlock;
		break;
		
		case Unlock:
		if (PINA & 0x80)
		state = Lock;
		else if (PINA & 0x04)
		state = PressA2Lock;
		else
		state = Unlock;
		break;
		
		case PressA2Lock:
		if ((!(PINA & 0x04)) && (PINA & 0x02))
		state = Lock;
		else
		state = PressA2Lock;
		default:
		break;
	}
	switch (state)
	{
		case Lock:
		PORTB = 0x00;
		break;
		case PressA2Unlock:
		PORTB = 0x00;
		break;
		case Unlock:
		PORTB = 0x01;
		break;
		case PressA2Lock:
		PORTB = 0x01;
		default:
		break;
	}
}
int main(void)
{
	/* Replace with your application code */
	state = Start;
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	while (1)
	{
		tick();
	}
}
