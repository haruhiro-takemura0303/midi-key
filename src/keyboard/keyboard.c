/**
* @file keyboard_snooper.c
* @brief 
* @details 
*/

#include "keyboard.h"

/** Type ***************************/
#define KEYBOARD_STABLE_COUNT 3U
#define STANDARD_VELOCITY 0x3FU
#define NOTE_OFST 0U

/** Type ***************************/
// keyboard State
typedef enum {
    PRE_OFF,
    PRE_ON,
    OFF,
    ON,
}eKeyboard_State_t;

// Each Keyboard Manager
typedef struct {
    eKeyboard_State_t state;
    uint8_t mask;
    uint8_t count;
    uint8_t note;
    uint8_t velocity;
} Keyboard_Manager_t;

// Keyboard Manager
typedef union {
    Keyboard_Manager_t arrayAll[NUMOF_KEYBOARD];
    Keyboard_Manager_t arrayBlock[NUMOF_SCANLINE][NUMOF_SCANLINE_KEYBOARD];
}uKeyboard_Manager_t;

/** Variable ***************************/
// Keyboard Pin Bitmap
uint8_t gKeyboardPinBitmap[NUMOF_SCANLINE] = {0};
// Keyboard Pin Bitmap Buffer
static uint8_t buffKeyboardPinBitmap[NUMOF_SCANLINE] = {0};
// Keyboard manager
uKeyboard_Manager_t uKeyboardManager;

/** Function ***************************/
void InitKeyboardManager(void)
{
    for(uint8_t i = 0; i < NUMOF_KEYBOARD; i++){
        uKeyboardManager.arrayAll[i].state = OFF;
        uKeyboardManager.arrayAll[i].mask = (1U << (i % NUMOF_SCANLINE_KEYBOARD));
        uKeyboardManager.arrayAll[i].count = 0;
        uKeyboardManager.arrayAll[i].note = i + NOTE_OFST;
        uKeyboardManager.arrayAll[i].velocity = STANDARD_VELOCITY;
    }
}

void SetKeyboardBitmap(int scanNum, uint8_t theVal)
{
    gKeyboardPinBitmap[scanNum] = theVal;
}

void KeyboardProc(int scanNum)
{
    uint8_t diff = buffKeyboardPinBitmap[scanNum] != gKeyboardPinBitmap[scanNum];

    for(int i = 0; i < NUMOF_SCANLINE_KEYBOARD; i++){
        // Set The Manager Ptr
        Keyboard_Manager_t* pManager = &(uKeyboardManager.arrayBlock[scanNum][i]);       
        
        // Update Count
        if(diff & pManager->mask != 0){
            pManager->count++;   
        } else {
            pManager->count = 0;
        }

        // Update State
        switch (pManager->state){
        case OFF:
            if(pManager->count == 0){
                pManager->state = PRE_ON;
            }
            else {
                pManager->state = 0;
            }
            break;
        
        case ON:
            if(pManager->count == 0){
                pManager->state = PRE_OFF;
            }
            else {
                pManager->state = 0;
            }
            break;
        
        case PRE_OFF:
            if(pManager->count == 0){
                pManager->state = ON;
            }
            else if (pManager->count >= KEYBOARD_STABLE_COUNT){
                pManager->state = OFF;
                /* Note Off */
            }
            break;
        
        case PRE_ON:
         if(pManager->count == 0){
                pManager->state = OFF;
            }
            else if (pManager->count >= KEYBOARD_STABLE_COUNT){
                pManager->state = ON;
                /* Note On */
            }
            break;
        
        default:
            break;
        }

    }
    // Update Buffer
    buffKeyboardPinBitmap[scanNum] = gKeyboardPinBitmap[scanNum];
}


