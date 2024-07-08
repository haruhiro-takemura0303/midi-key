/**
* @file main.c
* @brief -
* @details -
*/


#include "stm32f3xx.h"

#include "sysclk.h"
#include "pin_config.h"

#include "usbd_core.h"
#include "usbd_cdc.h"
#include "pma.h"

#include "scan.h"
#include "keyboard_task.h"

int main (void)
{
    InitSysclkHSI48();
		InitUsbDevice();
    InitScan();
    InitPin();
		InitKeyboard();

    StartScan();
    
    for(;;){
        
    }
}
