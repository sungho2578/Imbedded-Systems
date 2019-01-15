/*  Name & Email: Sungho Ahn & sahn025@ucr.edu
 *	Partner Name: Than Phan & tphan029@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 3 Exercise 1
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>

// Bit-access function
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0x00; PORTB = 0xFF; // Configure port B's 8 pins as inputs,
	DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs,
	
	unsigned char tmpA = 0x00; // intermediate variable used for port updates
	unsigned char tmpB = 0x00; // intermediate variable used for port updates
	
	while(1)
	{
		// 1) Read Inputs and assign to variables
		unsigned char cnt = 0x00;
		tmpA = PINA;
		tmpB = PINB;
		
		// 2) Perform Computation
		for (int i = 0; i < 8; i++){
			if(GetBit(tmpA, i)){
				cnt++;
			}
		}
		
		for (int i = 0; i < 8; i++){
			if(GetBit(tmpB, i)){
				cnt++;
			}
		}	
		
		// 3) write results to port	 
		PORTC = cnt;		
	}
}