/*  Name & Email: Sungho Ahn & sahn025@ucr.edu
 *	Partner Name: Than Phan & tphan029@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 2 Exercise 3
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0xFF;	// Input
	DDRC = 0xFF; PORTC = 0x00;	// Output
	
	unsigned char cntavail = 0x00;
   
	while (1) 
    {
		cntavail = 0x04;
		if((PINA & 0x01) == 1) cntavail--;
		if((PINA & 0x02) == 2) cntavail--;
		if((PINA & 0x04) == 4) cntavail--;
		if((PINA & 0x08) == 8) cntavail--;	
		PORTC = cntavail;
		
		if (cntavail == 0x00){
				PORTC = PORTC | 0x80;
		}
	}
}
