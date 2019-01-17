/*  Name & Email: Sungho Ahn & sahn025@ucr.edu
 *	Partner Name: Than Phan & tphan029@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 2 Exercise 4
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <stdlib.h>

int main(void)
{
    /* Replace with your application code */
	DDRA = DDRB = DDRC = 0x00;		// Input
	PORTA = PORTB = PORTC = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;		// Output
	
	unsigned char totalWeight = 0x00;
	unsigned char maxWeight = 140;
   
	while (1) 
    {
		totalWeight = PINA + PINB + PINC;
		PORTD = totalWeight << 2;
		if (totalWeight > maxWeight){
			PORTD = PORTD | 0x01;
		}
		if (abs(PINA - PINC) > 80){
			PORTD = PORTD | 0x02;	
		}
	}
}
