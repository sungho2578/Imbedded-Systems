/*  Name & Email: Sungho Ahn & sahn025@ucr.edu
 *	Partner Name: Than Phan & tphan029@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 10 Exercise 2
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B = 0x0B;// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A = 125;    // Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register
	TIMSK1 = 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1=0;

	_avr_timer_cntcurr = _avr_timer_M;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds

	//Enable global interrupts
	SREG |= 0x80; // 0x80: 1000000
}

void TimerOff() {
	TCCR1B = 0x00; // bit3bit1bit0=000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect) {
	// CPU automatically calls when TCNT1 == OCR1 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; // Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { // results in a more efficient compare
		TimerISR(); // Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}


enum ThreeLED_States {ThreeLED_init, ThreeLED_run} threeLED_state;
enum BlinkLED_States {BlinkLED_init, BlinkLED_run} blinkLED_state;
enum Combine_States {Combine_init, Combine_run} combine_state;

unsigned char threeLED;
unsigned char blinkLED;

void ThreeLED_tick()
{
	switch (threeLED_state)
	{
		case ThreeLED_init:
		threeLED = 0;
		threeLED_state = ThreeLED_run;
		break;
		case ThreeLED_run:
		threeLED_state = ThreeLED_run;
		break;
		default:
		break;
	}
	switch (threeLED_state)
	{
		case ThreeLED_init:
		break;
		case ThreeLED_run:
		if (threeLED == 0x00)
		threeLED = 0x01;
		else if (threeLED == 0x01)
		threeLED = threeLED << 1;
		else if (threeLED == 0x02)
		threeLED = threeLED << 1;
		else if (threeLED == 0x04)
		threeLED = 0x01;
		break;
		default:
		break;
	}
}

void BlinkLED_tick()
{
	switch (blinkLED_state)
	{
		case BlinkLED_init:
		blinkLED = 0x00;
		blinkLED_state = BlinkLED_run;
		break;
		case BlinkLED_run:
		blinkLED_state = BlinkLED_run;
		break;
		default:
		break;
	}
	switch (blinkLED_state)
	{
		case BlinkLED_init:
		break;
		case BlinkLED_run:
		if (blinkLED == 0x00)
		blinkLED = 0x08;
		else if (blinkLED == 0x08)
		blinkLED = 0x00;
		break;
		default:
		break;
	}
}

void Combine_tick()
{
	switch (combine_state)
	{
		case Combine_init:
		PORTB = 0x00;
		combine_state = Combine_run;
		break;
		case Combine_run:
		combine_state = Combine_run;
		break;
		default:
		break;
	}
	switch (combine_state)
	{
		case Combine_init:
		break;
		case Combine_run:
		PORTB = blinkLED | threeLED;
		break;
		default:
		break;
	}
}
int main(void)
{
	unsigned long blinkLED_elapTime = 0;
	unsigned long threeLED_elapTime = 0;
	const unsigned long timerPeriod = 100;
	
	PORTB = 0xFF; PORTB = 0x00;
	combine_state = Combine_init;
	threeLED_state = ThreeLED_init;
	blinkLED_state = BlinkLED_init;
	TimerSet(100);
	TimerOn();
	
	while (1)
	{
		if (blinkLED_elapTime >= 1000){
			BlinkLED_tick();
			blinkLED_elapTime = 0;
		}
		
		if (threeLED_elapTime >= 300){
			ThreeLED_tick();
			threeLED_elapTime = 0;
		}

		Combine_tick();
		while(!TimerFlag);
		TimerFlag = 0;
		
		blinkLED_elapTime += timerPeriod;
		threeLED_elapTime += timerPeriod;
	}
}