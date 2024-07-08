/**
* @file keybord_task.h
* @brief 
* @details 
*/

#include "keyboard_task.h"
#include "keyboard.h"
#include "task.h"

/** Define ***************************/
#define EVENT_MASK_SCANLINE0  1U << 0U
#define EVENT_MASK_SCANLINE1  1U << 1U
#define EVENT_MASK_SCANLINE2  1U << 2U
#define EVENT_MASK_SCANLINE3  1U << 3U

/** variable ***************************/
static uint32_t flgptn = 0;

/** Function  ***************************/
static void InitKeyboardTask(void)
{
    CreateTask(TASK_ID_KEYBOARD, TASK_PRIORITY_KEYBOARD);
}

void ClearEventFlag(uint32_t msk)
{   
    /* Disble IRQ */
    flgptn = flgptn & ~msk;
}

void SetEventFlagScanLine(int scanNum)
{   
    switch (scanNum){
    case SCAN0:
        flgptn |= EVENT_MASK_SCANLINE0;
        break;
    case SCAN1:
        flgptn |= EVENT_MASK_SCANLINE1;
        break;
    case SCAN2:
        flgptn |= EVENT_MASK_SCANLINE2;
        break;
    case SCAN3:
        flgptn |= EVENT_MASK_SCANLINE3;
        break;
    default:
        break;
    }

    WakeUpTask(TASK_ID_KEYBOARD); 
}

/** VDI Task ***************************/
//void KeyBoardTask(void)
void EXTI0_IRQHandler(void)
{
    for(;;){
        if((flgptn & EVENT_MASK_SCANLINE0) !=0 ){
            ClearEventFlag(EVENT_MASK_SCANLINE0);
            KeyboardProc(SCAN0);
        }        
        if((flgptn & EVENT_MASK_SCANLINE1) !=0 ){
            ClearEventFlag(EVENT_MASK_SCANLINE1);
            KeyboardProc(SCAN1);
        }
        if((flgptn & EVENT_MASK_SCANLINE2) !=0 ){
            ClearEventFlag(EVENT_MASK_SCANLINE2);
            KeyboardProc(SCAN2);
        }        
        if((flgptn & EVENT_MASK_SCANLINE3) !=0 ){
            ClearEventFlag(EVENT_MASK_SCANLINE3);
            KeyboardProc(SCAN3);
        }

        if(flgptn == 0){
            SleepTask(TASK_ID_KEYBOARD);
            break;
        }
    }
}

void InitKeyboard(void)
{
	InitKeyboardManager();
	InitKeyboardTask();
}