/*  Name & Email: Sungho Ahn & sahn025@ucr.edu
 *	Partner Name: Than Phan & tphan029@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 11 Exercise 4
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
//#include <bit.h>
#include <timer.h>
#include <stdio.h>
#include <keypad.h>
#include "io.c"

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
unsigned char setKey=0x00;
enum SM1_states{go} SM1_state;
unsigned char cursor_pos=0x01;

//--------End Shared Variables------------------------------------------------

//--------User defined FSMs---------------------------------------------------
//Enumeration of states.
int SMTick1(int state)
{
	switch(state)
	{
		default:
		setKey = GetKeypadKey();
		if(setKey != '\0')
		{
			LCD_WriteData(setKey);
			if(cursor_pos > 15)
			{
				cursor_pos = 1;
				LCD_Cursor(cursor_pos);
			}
			else
			{
				cursor_pos++;
				LCD_Cursor(cursor_pos);
			}
		}
		break;
	}
	return 0;
}

// --------END User defined FSMs-----------------------------------------------

// Implement scheduler code from PES.
int main()
{
	// Set Data Direction Registers
	// Buttons PORTA[0-7], set AVR PORTA to pull down logic
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	// . . . etc
	
	//Declare an array of tasks
	static task task1;
	task *tasks[] = {&task1};
	unsigned long int task_period = 300;
	unsigned long int task_tick = 30;
	//const unsigned short numTasks = sizeof(task)/sizeof(task*);
	
	// Task 1
	task1.state = -1;//Task initial state.
	task1.period = task_period;//Task Period.
	task1.elapsedTime = task_tick;//Task current elapsed time.
	task1.TickFct = &SMTick1;//Function pointer for the tick.

	// Set the timer and turn it on
	TimerSet(task_period);
	TimerOn();
	LCD_init();
	
	const char* intro = "Congratulations!";
	LCD_DisplayString(1, intro);
	LCD_Cursor(cursor_pos);

	while(1)
	{
		// Scheduler code
		tasks[0]->TickFct(0);
		
		while(!TimerFlag){}
		TimerFlag=0;
	}
	
	// Error: Program should not exit!
	return 0;
}