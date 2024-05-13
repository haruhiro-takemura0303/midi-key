/**
* @file     sysclk.h 
* @brief    system clock settings
* @details  -
*/

#include "stdint.h"

#ifndef _sysclk_h
#define _sysclk_h

void InitSysclkHSI48(void);
void WaiteXmsec(uint32_t ms);

#endif // _sysclk_h

