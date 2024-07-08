/**
* @file keyboard.h
* @brief 
* @details 
*/

#include"stdint.h"
#include "scan.h"

#ifndef _keyboard_h_
#define _keyboard_h_

#define NUMOF_KEYBOARD  32U
#define NUMOF_SCANLINE_KEYBOARD 8U

void InitKeyboardManager(void);
void SetKeyboardBitmap(int scanNum, uint8_t theVal);
void KeyboardProc(int scanNum);

#endif // _keyboard_h_