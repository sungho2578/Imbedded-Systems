/*  Name & Email: Sungho Ahn & sahn025@ucr.edu
 *	Partner Name: Than Phan & tphan029@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 10 Exercise 5
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




enum States{start, init, inc, reInc, dec, reDec} state;
unsigned char button0, button1;
unsigned char Bvalue = 0;
unsigned char oneS = 0;
unsigned char miliS = 0;
unsigned char counterState = 0;

void tick()
{
	button0 = (~PINA) & 0x01;
	button1 = ((~PINA) >> 1) & 0x01;
	//Bvalue = PORTB;
	switch (state)
	{
		case start:
		state = init;
		break;
		case init:
		if ((button0) && (button1))
		{
			state = init;
		}
		else if (button0)
		{
			Bvalue = Bvalue < 15 ? Bvalue + 1 : Bvalue;
			state = inc;
		}
		else if (button1)
		{
			Bvalue = Bvalue > 0 ? Bvalue - 1 : Bvalue;
			state = dec;
		}
		break;
		case inc:
		if ((button0) && !(button1))
		{
			state = inc;
			oneS = 1;
		}
		else if (!(button0) && !(button1))
		{
			state = reInc;
			oneS = 0;
			miliS = 0;
			counterState = 0;
		}
		else if (!(button0) && (button1))
		{
			Bvalue = Bvalue > 0 ? Bvalue - 1 : Bvalue;
			state = dec;
			oneS = 0;
			miliS = 0;
			counterState = 0;
		}
		else if ((button0) && (button1))
		{
			state = init;
			oneS = 0;
			miliS = 0;
			counterState = 0;
		}
		break;
		case dec:
		if ((button0) && !(button1))
		{
			Bvalue = Bvalue < 15 ? Bvalue + 1 : Bvalue;
			state = inc;
			oneS = 0;
			miliS = 0;
			counterState = 0;
		}
		else if (!(button0) && !(button1))
		{
			state = reDec;
			oneS = 0;
			miliS = 0;
			counterState = 0;
		}
		else if (!(button0) && (button1))
		{
			state = dec;
			oneS = 1;
		}
		else if ((button0) && (button1))
		{
			state = init;
			oneS = 0;
			miliS = 0;
			counterState = 0;
		}
		break;
		case reInc:
		if ((button0) && !(button1))
		{
			Bvalue = Bvalue < 15 ? Bvalue + 1 : Bvalue;
			state = inc;
		}
		else if (!(button0) && !(button1))
		{
			state = reInc;
		}
		else if (!(button0) && (button1))
		{
			Bvalue = Bvalue > 0 ? Bvalue - 1 : Bvalue;
			state = dec;
		}
		else if ((button0) && (button1))
		{
			state = init;
		}
		break;
		case reDec:
		if ((button0) && !(button1))
		{
			Bvalue = Bvalue < 15 ? Bvalue + 1 : Bvalue;
			state = inc;
		}
		else if (!(button0) && !(button1))
		{
			state = reDec;
		}
		else if (!(button0) && (button1))
		{
			Bvalue = Bvalue > 0 ? Bvalue - 1 : Bvalue;
			state = dec;
		}
		else if ((button0) && (button1))
		{
			state = init;
		}
		break;
		default:
		break;
	}
	
	switch (state)
	{
		case init:
		Bvalue = 0x00;
		break;
		case inc:
		break;
		case dec:
		break;
		case reInc:
		break;
		case reDec:
		break;
		default:
		break;
	}
	PORTB = Bvalue;
}


enum OneS_states{OneS_Start, OneS_Inc, OneS_Dec} OneS_state;

void OneS_tick()
{
	switch (OneS_state)
	{
		case OneS_Start:
		if ((!oneS) || miliS)
		{
			break;
		}
		if (~PINA & 0x01)
		{
			OneS_state = OneS_Inc;
		}
		else if (~PINA & 0x02)
		{
			OneS_state = OneS_Dec;
		}
		break;
		case OneS_Inc:
		if ((!oneS) || miliS)
		{
			break;
		}
		if (~PINA & 0x01)
		{
			OneS_state = OneS_Inc;
		}
		else if (~PINA & 0x02)
		{
			OneS_state = OneS_Dec;
		}
		break;
		case OneS_Dec:
		if ((!oneS) || miliS)
		{
			break;
		}
		if (~PINA & 0x01)
		{
			OneS_state = OneS_Inc;
		}
		else if (~PINA & 0x02)
		{
			OneS_state = OneS_Dec;
		}
		break;
		default:
		break;
	}
	switch (OneS_state)
	{
		case OneS_Start:
		break;
		case OneS_Inc:
		if ((!oneS) || miliS)
		break;
		Bvalue = (Bvalue < 15) ? ++Bvalue : Bvalue;
		break;
		case OneS_Dec:
		if ((!oneS) || miliS)
		break;
		Bvalue = (Bvalue > 0) ? --Bvalue : Bvalue;
		break;
		default:
		break;
	}
	if (oneS)
	{
		PORTB = Bvalue;
		++counterState;
		if (counterState >= 3)
		{
			miliS = 1;
		}
	}
}


enum MiliS_states{MiliS_Start, MiliS_Inc, MiliS_Dec} MiliS_state;

void MiliS_tick()
{
	switch (MiliS_state)
	{
		case MiliS_Start:
		if (!miliS)
		{
			break;
		}
		if (~PINA & 0x01)
		{
			MiliS_state = MiliS_Inc;
		}
		else if (~PINA & 0x02)
		{
			MiliS_state = MiliS_Dec;
		}
		break;
		case MiliS_Inc:
		if (!miliS)
		{
			break;
		}
		if (~PINA & 0x01)
		{
			MiliS_state = MiliS_Inc;
		}
		else if (~PINA & 0x02)
		{
			MiliS_state = MiliS_Dec;
		}
		break;
		case MiliS_Dec:
		if (!miliS)
		{
			break;
		}
		if (~PINA & 0x01)
		{
			MiliS_state = MiliS_Inc;
		}
		else if (~PINA & 0x02)
		{
			MiliS_state = MiliS_Dec;
		}
		break;
		default:
		break;
	}
	switch (MiliS_state)
	{
		case MiliS_Start:
		break;
		case MiliS_Inc:
		if (!miliS)
		break;
		Bvalue = (Bvalue < 15) ? ++Bvalue : Bvalue;
		break;
		case MiliS_Dec:
		if (!miliS)
		break;
		Bvalue = (Bvalue > 0) ? --Bvalue : Bvalue;
		break;
		default:
		break;
	}
	if (miliS)
	{
		PORTB = Bvalue;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned long One_elTime = 1000;
	unsigned long Mili_elTime = 400;
	unsigned char period = 100;
	TimerSet(period);
	TimerOn();
	button1 = button0 = 0;
	Bvalue = 0;
	state = start;
	OneS_state = OneS_Start;
	while (1)
	{
		tick();
		if (One_elTime >= 1000)
		{
			OneS_tick();
			One_elTime = 0;
		}
		if (Mili_elTime >= 400)
		{
			MiliS_tick();
			Mili_elTime = 0;
		}
		while (!TimerFlag);
		TimerFlag = 0;
		One_elTime += period;
		Mili_elTime += period;
	}
}

