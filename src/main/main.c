/**
* @file main.c
* @brief -
* @details -
*/


#include "stm32f3xx.h"
#include "sysclk.h"
#include "usbd_core.h"
#include "usbd_cdc.h""
#include "pma.h"

int main (void)
{
    InitSysclkHSI48();
    InitUsbDevice();
    for(;;){
       USBD_CDCDataSend("test\r\n");
    }
}
