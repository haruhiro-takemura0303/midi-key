/**
* @file     pinconfig.c 
* @brief    gpio settings
* @details  -
*/

#include "stm32f3xx.h"

static void InitPinGPIO(void)
{
	/* Keyboard Scan Line PA0 - PA3 */
	// Clcok Enable
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	// Set Mode Input
	GPIOA->MODER &= ~(GPIO_MODER_MODER0_Msk
					| GPIO_MODER_MODER1_Msk
					| GPIO_MODER_MODER2_Msk
					| GPIO_MODER_MODER3_Msk);
	
	/* Keyboard Scan Line PC0 - PC7 */
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
	// Set Mode Input
	GPIOC->MODER &= ~(GPIO_MODER_MODER0_Msk
					| GPIO_MODER_MODER1_Msk
					| GPIO_MODER_MODER2_Msk
					| GPIO_MODER_MODER3_Msk
					| GPIO_MODER_MODER4_Msk
					| GPIO_MODER_MODER5_Msk
					| GPIO_MODER_MODER6_Msk
					| GPIO_MODER_MODER7_Msk);
	
	GPIOC->MODER = (GPIOC->MODER &~GPIO_MODER_MODER11_Msk) | GPIO_MODER_MODER11_0;
}

void InitPin(void)
{
	InitPinGPIO();
}