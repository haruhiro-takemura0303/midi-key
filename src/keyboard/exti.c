/**
* @file     pinconfig.c 
* @brief    gpio settings
* @details  -
*/

#include "stm32f3xx.h"
#include "usbd_cdc.h"
#define SYSCFG_EXTICR0_Msk SYSCFG_EXTICR1_EXTI0_Msk|SYSCFG_EXTICR1_EXTI1_Msk|SYSCFG_EXTICR1_EXTI2_Msk|SYSCFG_EXTICR1_EXTI3_Msk

void InitEXTI(void)
{
	/* Enable EXTICR */
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PD	\
											|SYSCFG_EXTICR1_EXTI1_PC	\
											|SYSCFG_EXTICR1_EXTI2_PC	\
											|SYSCFG_EXTICR1_EXTI3_PC;
	
	
	/* Setting EXCTICR */
	// Enable IRQ
	EXTI->IMR |= EXTI_IMR_IM0|EXTI_IMR_IM1|EXTI_IMR_IM2 |EXTI_IMR_IM3;
	// Disable Event
	EXTI->EMR &= ~(EXTI_EMR_EM0|EXTI_EMR_EM1|EXTI_EMR_EM2|EXTI_EMR_EM3);
	// Disabel Rise Edge
	EXTI->RTSR &= ~(EXTI_RTSR_TR0|EXTI_RTSR_TR1|EXTI_RTSR_TR2|EXTI_RTSR_TR3);							
	// Enable Fall Edge
	EXTI->FTSR |= EXTI_FTSR_TR0|EXTI_FTSR_TR1|EXTI_FTSR_TR2|EXTI_FTSR_TR3;
	
	/* Enable NVIC */
	// EXTI0
	NVIC_SetPriority(EXTI0_IRQn, 2);
  NVIC_EnableIRQ(EXTI0_IRQn);
	// EXTI1
	NVIC_SetPriority(EXTI1_IRQn, 2);
  NVIC_EnableIRQ(EXTI1_IRQn);
	// EXTI2
	NVIC_SetPriority(EXTI2_TSC_IRQn, 2);
  NVIC_EnableIRQ(EXTI2_TSC_IRQn);
	// EXTI3
	NVIC_SetPriority(EXTI3_IRQn, 2);
  NVIC_EnableIRQ(EXTI3_IRQn);
}

int test0 = 0;
int test1 = 0;
int test2 = 0;
int test3 = 0;
void EXTI0_IRQHandler(void)
{
	if (EXTI->PR & EXTI_PR_PR0) {
    EXTI->PR |= EXTI_PR_PR0;
		USBD_CDCDataSend("s0s ");
  }
}

void EXTI1_IRQHandler(void)
{
	if (EXTI->PR & EXTI_PR_PR1) {
    EXTI->PR |= EXTI_PR_PR1;
		USBD_CDCDataSend("s1s ");
  }
}

void EXTI2_TSC_IRQHandler(void)
{
	if (EXTI->PR & EXTI_PR_PR2) {
    EXTI->PR |= EXTI_PR_PR2;
		USBD_CDCDataSend("s2s ");
  }
}

void EXTI3_IRQHandler(void)
{
	if (EXTI->PR & EXTI_PR_PR3) {
    EXTI->PR |= EXTI_PR_PR3;
		USBD_CDCDataSend("s3s ");
  }
}
