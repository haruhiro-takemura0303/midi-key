/**
* @file     clk.c 
* @brief    system clock settings
* @details  -
*/

#include "stm32f3xx.h"

#define SYSTICK_COUNT_NUM_1MSEC 6000U
#define SYSTICK_COUNT_NUM_1USEC 6U

/**
* @brief  Inist Stsclk used HIS48
* @param  void
* @param  
* @return void
* @sa 
* @details  Using PLL Clock from HSI. 
*/
void InitSysclkHSI48(void)
{
	/* Set PLL */
	RCC->CR &= ~RCC_CR_PLLON_Msk;						// Set PLL OFF
	RCC->CFGR &= ~RCC_CFGR_PLLSRC_Msk;			// Set PLL Input as HSI/2
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PLLMUL_Msk) | RCC_CFGR_PLLMUL12;	// Set PLL x12
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PPRE1_Msk) | (0x4 << RCC_CFGR_PPRE1_Pos);
	RCC->CR |= RCC_CR_PLLON;								// Set PLL ON
	while(!(RCC->CR & RCC_CR_PLLRDY_Msk));	// Waite until PLL is stable
	
	/* Set Flash */
	FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY_Msk) | FLASH_ACR_LATENCY_1;	// Set Flash Latency as 1
	
	/* Set Clock Source */
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL;		// Set Clock Source as PLL
	while((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_PLL);	// Waite until PLL is Stable
}

/**
* @brief Waite for Xm Sec
* @param int ms
* @param -
* @return void
* @sa 
* @details SysTick CLK is HCLK/8 = 6 MHz
*/
void WaiteXmsec (uint32_t ms)
{
	SysTick->LOAD = SYSTICK_COUNT_NUM_1MSEC - 1;	// Reset Vlue For CountDown Timer(1msec)
	SysTick->VAL = 0;
	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;			// Start Count
	for(uint32_t i = 0; i < ms; i++ ){
			while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));	// Waite Count 1msec
	}
}

/**
* @brief Waite for um Sec
* @param int ms
* @param -
* @return void
* @sa 
* @details SysTick CLK is HCLK/8 = 6 MHz
*/
void Waiteumsec (uint32_t us)
{
	SysTick->LOAD = SYSTICK_COUNT_NUM_1USEC - 1;	// Reset Vlue For CountDown Timer(1msec)
	SysTick->VAL = 0;
	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;			// Start Count
	for(uint32_t i = 0; i < us; i++ ){
			while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));	// Waite Count 1msec
	}
}
