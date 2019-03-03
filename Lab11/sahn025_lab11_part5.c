/*  Name & Email: Sungho Ahn & sahn025@ucr.edu
 *	Partner Name: Than Phan & tphan029@ucr.edu
 *	Lab Section: 23
 *	Assignment: Lab 11 Exercise 5
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
unsigned char row1 = 8;
unsigned char row2 = 29;
unsigned char cursorLocation = 2;
unsigned char playPause = 0;
unsigned char stop = 0;
//--------End Shared Variables------------------------------------------------

//--------User defined FSMs---------------------------------------------------
//Enumeration of states.

enum Move_States {M_start, M_move} M_state;
	

int SMTick1(int state)
{
	if (stop)
		return 0;
	switch (M_state)
	{
		case M_start:
			if (~PINA & 0x04)
			{
				M_state = M_move;
				playPause = 1;
			}
			else
			{
				M_state = M_start;
				playPause = 0;
			}
			row1 = 8;
			row2 = 29;
			break;
		case M_move:
			if (~PINA & 0x04)
				playPause = (playPause == 1) ? 0 : 1;
			else
				M_state = M_move;
			break;
		default:
			break;			
	}
	
	switch (M_state)
	{
		case M_start:
			break;
		case M_move:
			if (playPause)
			{
				row1 = (row1 <= 1) ? 16 : --row1;
				row2 = (row2 <= 17) ? 32 : --row2;
			}
			break;
		default:
			break;
	}
	LCD_ClearScreen();
	LCD_Cursor(cursorLocation);
	LCD_Cursor(row1);
	LCD_WriteData('#');
	LCD_Cursor(row2);
	LCD_WriteData('#');
	LCD_Cursor(cursorLocation);
	return 0;
}

enum Cursor_States {C_start, C_move} C_state;


int SMTick2()
{
	if (stop)
		return 0;
	switch(C_state)
	{
		case C_start:
			cursorLocation = 2;
			C_state = C_move;
			break;
		case C_move:
			break;
		default:
			break;
	}
	switch(C_state)
	{
		case C_start:
			break;
		case C_move:
			if (~PINA & 0x08)
			{
				cursorLocation = 2;
			}
			else if (~PINA & 0x10)
			{
				cursorLocation = 18;
			}
			break;
		default:
			break;
	}
	LCD_Cursor(cursorLocation);
	return 0;
}

enum Fail_States {F_start, F_check} F_state;

int SMTick3()
{
	switch (F_state)
	{
		case F_start:
			F_state = F_check;
			break;
		case F_check:
			break;
		default:
			break;
	}
	switch (F_state)
	{
		case F_start:
			break;
		case F_check:
			if ((cursorLocation == row1) || (cursorLocation == row2))
			{
				if (stop == 0)
					LCD_DisplayString(3, " Hurray for   Embedded Systems");
				stop = 1;
				if (~PINA & 0x04)
				{
					row1 = 8;
					row2 = 29;
					stop = 0;
				}
			}
			break;
		default:
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
	DDRA = 0x03; PORTA = 0xFC;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	// . . . etc
	
	//Declare an array of tasks
	static task task1;
	static task task2;
	static task task3;
	task *tasks[] = {&task1, &task2, &task3};
	unsigned long int task_period = 100;
	unsigned long int task_tick = 500;
	//const unsigned short numTasks = sizeof(task)/sizeof(task*);
	
	// Task 1
	task1.state = -1;//Task initial state.
	task1.period = task_period;//Task Period.
	task1.elapsedTime = 500;//Task current elapsed time.
	task1.TickFct = &SMTick1;//Function pointer for the tick.

	// task 2
	task2.state = -1;
	task2.period = task_period;
	task2.elapsedTime = 200;
	task2.TickFct = &SMTick2;

	// task 3
	task3.state = -1;
	task3.period = task_period;
	task3.elapsedTime = 0;
	task3.TickFct = &SMTick3;
	
	// Set the timer and turn it on
	TimerSet(task_period);
	TimerOn();
	LCD_init();
	
	LCD_Cursor(row1);
	LCD_WriteData('#');
	LCD_Cursor(row2);
	LCD_WriteData('#');
	LCD_Cursor(2);

	while(1)
	{
		// Scheduler code
		if (task3.elapsedTime >= 0)
		{
			tasks[2]->TickFct(0);
			task3.elapsedTime = 0;
		}
		if (task1.elapsedTime >= 500)
		{
			tasks[0]->TickFct(0);
			task1.elapsedTime = 0;
		}
		if (task2.elapsedTime >= 200)
		{
			tasks[1]->TickFct(0);
			task2.elapsedTime = 0;
		}
		while(!TimerFlag){}
		TimerFlag=0;
		task1.elapsedTime += task_period;
		task2.elapsedTime += task_period;
		task3.elapsedTime += task_period;
	}
	
	// Error: Program should not exit!
	return 0;
}
