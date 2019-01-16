/*      Name & Email: Sungho Ahn & sahn025@ucr.edu
 *	Partner Name: Than Phan & tphan029@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 3 Exercise 5
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */


#include <avr/io.h>

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

int main(void)
{
    /* Replace with your application code */
	DDRD = 0x00; PORTD = 0x00;
	DDRB = 0xFE; PORTB = 0x00;
	unsigned short temp = 0x00;
    while (1) 
    {
		temp = 0x00;
		temp = PIND;
		temp = temp << 1;
		temp = temp | (PINB & 0x01);
		if (temp >= 70)
			PORTB = SetBit(PORTB, 1, 1);
		else
			PORTB = SetBit(PORTB, 1, 0);
		if ((temp < 70) && (temp > 5))
			PORTB = SetBit(PORTB, 2, 1);
		else
			PORTB = SetBit(PORTB, 2, 0);
    }
}

