/*  Name & Email: Sungho Ahn & sahn025@ucr.edu
 *	Partner Name: Than Phan & tphan029@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 11 Exercise 3
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <bit.h>
#include <timer.h>
#include <stdio.h>
#include "io.c"
#include <keypad.h>


//--------Task scheduler data structure---------------------------------------
// Struct for Tasks represent a running process in our simple real-time operating system.
typedef struct _task {
	/*Tasks should have members that include: state, period,
		a measurement of elapsed time, and a function pointer.*/
	signed char state; //Task's current state
	unsigned long int period; //Task period
	unsigned long int elapsedTime; //Time elapsed since last task tick
	int (*TickFct)(int); //Task tick function
} task;

//--------End Task scheduler data structure-----------------------------------

//--------Shared Variables----------------------------------------------------
unsigned char x = 0x1F;
unsigned char get_value;
unsigned char temp = 0;
unsigned char temp2 = 0;

//--------End Shared Variables------------------------------------------------

//--------User defined FSMs---------------------------------------------------
//Enumeration of states.
enum states {init, press, off} state;
	
int tick_f(int state)
{
	//State machine transitions
	switch (state)
	{
		case init:
			if (x == 0x1F)
			{
				state = init;
			}
			else if (x != 0x1F)
			{
				state = press;
			}
			break;
		
		case press:
			if (x != 0x1F)
			{
				state = press;
			}
			else if (x == 0x1F)
			{
				state = off;
			}
			break;
		
		case off:
			state = init;
			break;
		
		default:
			state = init;
			x = 0x1F;
			break;
	}
	
	//State machine actions
	switch (state)
	{
		case init:
			x = GetKeypadKey();
			break;
		
		case press:
			x = GetKeypadKey();
			temp = GetKeypadKey();
			break;
		
		case off:
			temp2 = temp;
			break;
		
		default:
			break;
	}
	
	return state;
}

// --------END User defined FSMs-----------------------------------------------

int main(void)
{
	// Set Data Direction Registers
	// Buttons PORTA[0-7], set AVR PORTA to pull down logic
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	// . . . etc
	
	//Period for the tasks
	unsigned long int GCD = 50;
	
	//GCD periods for scheduler
	unsigned long int SMTick1_period = 50;
	
	//Declare an array of tasks
	static task task1;
	task *tasks[] = { &task1 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	//Task 1
	task1.state = -1;//Task initial state.
	task1.period = SMTick1_period;//Task Period.
	task1.elapsedTime = SMTick1_period;//Task current elapsed time.
	task1.TickFct = &tick_f;//Function pointer for the tick.
	
	// Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();
	
	
	DDRD = 0xFF; PORTD = 0x00;
	DDRA = 0xFF; PORTA = 0x00;
	LCD_init();
	LCD_ClearScreen();
	LCD_Cursor(1);
	unsigned char total = 0;
	unsigned short i; // Scheduler for-loop iterator

	while(1)
	{ 	
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime ==
			tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		
		switch (temp)
		{
			LCD_ClearScreen();
			case '\0': PORTB = 0x1F; break;
			case '1': PORTB = 0x01; total = 1; LCD_Cursor(1); LCD_WriteData(total + '0'); break;
			case '2': PORTB = 0x02; total = 2; LCD_Cursor(1); LCD_WriteData(total + '0'); break;
			case '3': PORTB = 0x03; total = 3; LCD_Cursor(1); LCD_WriteData(total + '0'); break;
			case '4': PORTB = 0x04; total = 4; LCD_Cursor(1); LCD_WriteData(total + '0'); break;
			case '5': PORTB = 0x05; total = 5; LCD_Cursor(1); LCD_WriteData(total + '0'); break;
			case '6': PORTB = 0x06; total = 6; LCD_Cursor(1); LCD_WriteData(total + '0'); break;
			case '7': PORTB = 0x07; total = 7; LCD_Cursor(1); LCD_WriteData(total + '0'); break;
			case '8': PORTB = 0x08; total = 8; LCD_Cursor(1); LCD_WriteData(total + '0'); break;
			case '9': PORTB = 0x09; total = 9; LCD_Cursor(1); LCD_WriteData(total + '0'); break;
			case '0': PORTB = 0x00; total = 0; LCD_Cursor(1); LCD_WriteData(total + '0'); break;
			case 'A': PORTB = 0x0A; total = 65; LCD_Cursor(1); LCD_WriteData(total); break;
			case 'B': PORTB = 0x0B; total = 66; LCD_Cursor(1); LCD_WriteData(total); break;
			case 'C': PORTB = 0x0C; total = 67; LCD_Cursor(1); LCD_WriteData(total); break;
			case 'D': PORTB = 0x0D; total = 68; LCD_Cursor(1); LCD_WriteData(total); break;
			case '*': PORTB = 0x0E; total = 42; LCD_Cursor(1); LCD_WriteData(total); break;
			case '#': PORTB = 0x0F; total = 35; LCD_Cursor(1); LCD_WriteData(total); break;
			default: PORTB = 0x1B; break;

			while (!TimerFlag);
			TimerFlag = 0;
		}
	}
	
	// Error: Program should not exit!
	return 0;
}