/*    Name & Email: Sungho Ahn & sahn025@ucr.edu
 *    Partner Name: Than Phan & tphan029@ucr.edu
 *    Lab Section: 023
 *    Assignment: Lab 3 Exercise 4
 *
 *
 *    I acknowledge all content contained herein, excluding template or example
 *    code, is my own original work.
 */

#include <avr/io.h>

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b)
{
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k)
{
	return ((x & (0x01 << k)) != 0);
}
int main(void)
{
    /* Replace with your application code */
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;

    while (1) 
    {
		for (unsigned char index = 0; index < 4; ++index)
		{
			PORTB = SetBit(PORTB, 3 - index, GetBit(PINA, 7 - index));
			PORTC = SetBit(PORTC, 7 - index, GetBit(PINA, 3 - index));
		}
    }
}

