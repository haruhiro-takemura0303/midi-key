/**
* @file usbd_core.c
* @brief usb device Ctrler
* @details 
*/

#include "pma.h"
#include "usbd_core.h"
#include "usbd_setup.h"
#include "sysclk.h"

/* ENDPOINT */
#define USB_EPnR(n) *(volatile uint16_t *)(&USB->EP0R + 2 * (n))
#define SET_ENDPOINT(bEpNum, wRegValue)	(*(__IO uint16_t *)(&USB->EP0R + ((bEpNum) * 2U)) = (uint16_t)(wRegValue))
#define GET_ENDPOINT(bEpNum)	(*(__IO uint16_t *)(&USB->EP0R + ((bEpNum) * 2U)))

static void StartUSBDevice(void)
{
	uint32_t winterruptmask;

  /* Clear pending interrupts */
  USB->ISTR = 0U;

  /* Set winterruptmask variable */
  winterruptmask = USB_CNTR_CTRM  |
                   USB_CNTR_RESETM;

  /* Set interrupt mask */
  USB->CNTR = (uint16_t)winterruptmask;
}

/**
* @brief InitUSBDevice
* @param void
* @param -
* @return void
* @sa 
* @details Initiialize Usb 
*/
void InitUsbDevice(void)
{
	/* Init GPIO*/
	// Enable Clock
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	while(!(RCC->AHBENR & RCC_AHBENR_GPIOAEN_Msk));
	
	// PA11     ------> USB_DM
	GPIOA->MODER |= GPIO_MODER_MODER11_1;
	GPIOA->PUPDR = GPIOA->PUPDR & ~GPIO_PUPDR_PUPDR11_Msk;
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR11_0 |  GPIO_OSPEEDER_OSPEEDR11_1); 
	GPIOA->AFR[1] = (GPIOA->AFR[1] & ~GPIO_AFRH_AFRH3_Msk) | ( 0x0E <<  GPIO_AFRH_AFRH3_Pos);
	
	// PA12     ------> USB_DP
	GPIOA->MODER |= GPIO_MODER_MODER12_1;
	GPIOA->PUPDR = GPIOA->PUPDR & ~GPIO_PUPDR_PUPDR12_Msk;
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR12_0 |  GPIO_OSPEEDER_OSPEEDR12_1); 
	GPIOA->AFR[1] = (GPIOA->AFR[1] & ~GPIO_AFRH_AFRH4_Msk) | ( 0x0E <<  GPIO_AFRH_AFRH4_Pos);
	
	/* Init Clock */
	RCC->CFGR |= RCC_CFGR_USBPRE_DIV1;
	RCC->APB1ENR |= RCC_APB1ENR_USBEN;
	while(!(RCC->APB1ENR & RCC_APB1ENR_USBEN_Msk));
	
	/* Init IRQ*/
	NVIC_SetPriority(USB_LP_CAN_RX0_IRQn, 0); // USB Low Priority IRQ
	NVIC_EnableIRQ(USB_LP_CAN_RX0_IRQn);

	/* Init USB IP */
	USB->CNTR = (uint16_t)USB_CNTR_FRES;	// FRES = 1
	USB->CNTR = 0;												// Clear USB Reset
	USB->ISTR = 0U;											// Clear Pending IRQ
	
	/* Start USB */
	StartUSBDevice();
}


int testNum = 0;
void USB_LP_CAN_RX0_IRQHandler(void)
{
	uint32_t flag = USB->ISTR;
	
	/* Reset IRQ */
	if(flag & USB_ISTR_RESET){
		testNum++;
		USB->ISTR &= ~USB_ISTR_RESET;
		// Init PMA
		InitPMA();
		// Set STATE RX : Valid TX : NACK
		PCD_SET_EP_TXRX_STATUS(USB, 0, USB_EP_RX_VALID, USB_EP_TX_NAK);
		// Set Endopint as Ctrl
		PCD_SET_EPTYPE(USB, 0, USB_EP_CONTROL);
		// Enable Device Addr
		USB->DADDR |= USB_DADDR_EF;
	}
	
	while( (flag = USB->ISTR) & USB_ISTR_CTR ) {
		const uint32_t epNum = flag & USB_ISTR_EP_ID;
		const uint32_t epReg = GET_ENDPOINT(epNum);
		
		switch(epNum){
		case EP_0:
			// Set State NACK		
			PCD_SET_EP_TXRX_STATUS(USB, 0, USB_EP_RX_NAK, USB_EP_TX_NAK);
			
			/* ----- Ctrl Out  -----  */
			if((flag & USB_ISTR_DIR) != 0U){
				// Clear Ctrl Rx
				PCD_CLEAR_RX_EP_CTR(USB, 0);
				/* ----- Setup Stage -----  */
				if((epReg & USB_EP_SETUP) != 0U){
					//First  Data Stage
					switch (USBDCtrlSetupStageProc()){
					case DATA_IN_STAGE:
						USBDCtrlDataInStageProc();
						break;
					case DATA_OUT_STAGE:
						USBDCtrlDataOutStageProc();
						break;
					case NO_DATA_STAGE:
						USBCtrlPutZLP();
						PCD_SET_EP_TXRX_STATUS(USB, 0, USB_EP_RX_VALID, USB_EP_TX_VALID);
						break;
					case SETUP_STALL:
						PCD_SET_EP_TXRX_STATUS(USB, 0, USB_EP_RX_VALID, USB_EP_TX_STALL);
						break;
					default:
						break;
					}
				} 
				else {
				/*------No Setup Stage---------*/
					if(!(GetPacketBufferPtr(0)->CountRX & USB_COUNT_RX_MSK)){
						/* ---- Status Out ------- */
						PCD_CLEAR_EP_KIND(USB, 0);
						PCD_SET_EP_TXRX_STATUS(USB, 0, USB_EP_RX_VALID, USB_EP_TX_NAK);
					}
					else{
						USBDCtrlDataOutStageProc();
					}
				}
				
			} 
			else {
			/* ------ Ctrl In ------ */
				// Clear Ctrl Tx
				PCD_CLEAR_TX_EP_CTR(USB, 0);
				if(!(GetPacketBufferPtr(0)->CountTX)){
						/* ------ Status In ------ */
						USBCtrlStatusInStageProc();
						PCD_SET_EP_TXRX_STATUS(USB, 0, USB_EP_RX_VALID, USB_EP_TX_NAK);
				} else {
					// Update EP State
					USBDCtrlDataInStageProc();
				}
			}
			break;
		case EP_1:
		case EP_2:
		case EP_3:
		default:
			break;
		}
	}
}
