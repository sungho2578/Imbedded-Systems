/*  Name & Email: Sungho Ahn & sahn025@ucr.edu
 *	Partner Name: Than Phan & tphan029@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 2 Exercise 1
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
	// PA0 = door sensor (input)
	// PA1 = light sensor (input)
	// PB0 = LED (output)
	
	DDRA = 0x00; PORTA = 0xFF;	// Input
	DDRB = 0xFF; PORTB = 0x00;	// Output
	
	unsigned char temp0 = 0x00;
	unsigned char temp1 = 0x00;
    
	while (1) 
    {
		temp0 = PINA & 0x01;
		temp1 = PINA & 0x02;
		if (temp0 == 0x01 && temp1 == 0x00){
			PORTB = 0x01;
		}
		else{
			PORTB = 0x00;
		}
    }
	return 0;
}