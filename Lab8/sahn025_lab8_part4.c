/*  Name & Email: Sungho Ahn & sahn025@ucr.edu
 *	Partner Name: Than Phan & tphan029@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 8 Exercise 4
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	//        in Free Running Mode, a new conversion will trigger whenever
	//        the previous conversion completes.
}

int main(void)
{
	/* Replace with your application code */
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	unsigned short s = 0x0000;
	unsigned short MAX = 0x01F0;
	ADC_init();
	
	while (1)
	{
		s = ADC;
		if (s >= MAX)
		PORTB = 0xFF;
		else if (s >= MAX / 2)
		PORTB = 0x7F;
		else if (s >= MAX / 4)
		PORTB = 0x3F;
		else if (s >= MAX / 6)
		PORTB = 0x1F;
		else if (s >= MAX / 8)
		PORTB = 0x0F;
		else if (s >= MAX / 10)
		PORTB = 0x07;
		else if (s >= MAX / 12)
		PORTB = 0x03;
		else
		PORTB = 0x01;

	}
}