/**
* @file task.h
* @brief VDI-task Interace
* @details 
*/

#include "stm32f3xx.h"

#ifndef _task_h_
#define _task_h_

/**
* Task Interface
*   CreateTask  : NVIC IRQ Enable Set Priority
*   StartTask   : Function XXX_IRQHandler
*/
void CreateTask(int id, int priority);
void WakeUpTask(int id);
void SleepTask(int id);

/**
* Task Definition
*   TASK_ID_XXX         : NVIC Macro XXX_IRQn
*   TASK_PRIORITY_XXX   : NVIC PRIORITY 
*   XXXTask             : Function XXX_IRQHandler
*/

/* Keyboard */
#define TASK_ID_KEYBOARD        EXTI0_IRQn			// Task ID
#define TASK_PRIORITY_KEYBOARD  3U			        // Task Priority
//#define KeyboardTask            EXTI0_IRQHandler	// Task Handler

#endif // _task_h_


