/**
* @file key_scan.c
* @brief detect key sacan
* @details 
*/

#include "stdint.h"
#include "stm32f3xx.h"
#include "keyboard_task.h"

/** Define ***************************/
#define GPIO_SCAN_MASK 0x0FU
#define SCAN_STABLE_COUNT 1U
#define SCAN_KEYBOARD_PIN_MASK 0xFFU

/** Type ***************************/
typedef enum{
    SCAN_PRE_NEGATE,
    SCAN_NEGATE,
    SCAN_ASSERT,
    SCAN_PRE_ASSERT,
}eScan_State_t;

typedef struct {    
    eScan_State_t state;
    uint8_t mask;
    uint8_t count;
}sScan_Manager_t;

/** Variable ***************************/
uint8_t scanBuff = 0;
sScan_Manager_t sScanManager[4U];

/** Function ***************************/
static void InitScanManager(void)
{
    /* Init Scan Manager */
    for (int i = 0; i < SCAN_STABLE_COUNT; i++){
        sScanManager[i].state = SCAN_NEGATE;
        sScanManager[i].mask = 1U << i;
        sScanManager[i].count = 0;
    }
}

int testVal;
static void Scan(void)
{
    uint8_t theVal =  (uint8_t)(GPIOA->IDR & GPIO_SCAN_MASK);
    uint8_t diff = theVal != scanBuff;

    for(int i = 0; i < NUMOF_SCANLINE; i++){
        sScan_Manager_t* pManager = &sScanManager[i];       
        // Update Count
        if((diff & pManager->mask) != 0){
            pManager->count = 0;   
        } else {
            pManager->count += 1;
        }

        // Update State
        switch (pManager->state){
        case SCAN_NEGATE:
            if(pManager->count == 0){
                pManager->state = SCAN_PRE_ASSERT;
            }
            else {
                pManager->count = 1;
            }
            break;
        
        case SCAN_ASSERT:
            if(pManager->count == 0){
                pManager->state = SCAN_PRE_NEGATE;
            }
            else {
                pManager->count = 1;
                //SetKeyboardBitmap(i, ~(GPIOC->IDR & SCAN_KEYBOARD_PIN_MASK));
                //SetEventFlagScanLine(i);
            }
            break;
        
        case SCAN_PRE_NEGATE:
            if(pManager->count == 0){
                pManager->state = SCAN_ASSERT;
            }
            else if (pManager->count >= SCAN_STABLE_COUNT){
                pManager->state = SCAN_NEGATE;
								if(i==0){
									GPIOC->BSRR = GPIO_BSRR_BR_11;
								}
            }
            break;
        
        case SCAN_PRE_ASSERT:
         if(pManager->count == 0){
						pManager->state = SCAN_NEGATE;
         }
         else if (pManager->count >= SCAN_STABLE_COUNT){
						pManager->state = SCAN_ASSERT;
            //SetKeyboardBitmap(i, ~(GPIOC->IDR & SCAN_KEYBOARD_PIN_MASK));
            //SetEventFlagScanLine(i);
					 	if(i==0){
							GPIOC->BSRR = GPIO_BSRR_BS_11;
						}
          }
            break;
        
        default:
            break;
        }

    }
    // Update Buffer
    scanBuff = theVal;
}

/** IRQ ***************************/
static void InitScanTimer(void)
{
    // Enable clock for TIM2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // Enable TIM2 clock

    // Configure prescaler and counter
    TIM2->PSC = 47; // Set prescaler (48MHz / (47 + 1) = 1MHz)
		TIM2->ARR = 10;  // Set auto-reload register (0.001msec * (39 + 1) = 0.04msec)

    // Enable interrupt
    TIM2->DIER |= TIM_DIER_UIE; // Enable update interrupt
    NVIC_EnableIRQ(TIM2_IRQn);  // Enable TIM2 interrupt in NVIC
}

static void StartScanTimer(void)
{
    TIM2->CR1 |= TIM_CR1_CEN; // Enable counter
}

void TIM2_IRQHandler(void)
{
    /* Timer Update */
    if (TIM2->SR & TIM_SR_UIF)
    {
        TIM2->SR &= ~TIM_SR_UIF; //Clear Update Flag
        Scan();
    }
}

void InitScan(void)
{
    InitScanManager();
    InitScanTimer();
}

void StartScan(void)
{
    StartScanTimer();
}
