#include "usbd_core.h"
#include "stdbool.h"
#include "string.h"
#include <stdio.h>
#include "pma.h" 

uint8_t gTestBuff[4] = {1, 2, 3, 4};
uint8_t gCDCRxBuff[64] = {0};
static bool isCDCInit = false;

void USBD_CDCInit(void)
{
	// EP3 command
	PCD_SET_EPTYPE(USB, 3, USB_EP_INTERRUPT);
	// EP1 EP2 Data
	PCD_SET_EPTYPE(USB, 1, USB_EP_BULK);
	PCD_SET_EPTYPE(USB, 2, USB_EP_BULK);
	
	PCD_SET_EP_TXRX_STATUS(USB, 1, USB_EP_RX_NAK, USB_EP_TX_NAK);
	PCD_SET_EP_TXRX_STATUS(USB, 2, USB_EP_RX_VALID, USB_EP_TX_NAK);
	PCD_SET_EP_TXRX_STATUS(USB, 3, USB_EP_RX_NAK, USB_EP_TX_NAK);
	isCDCInit = true;
}

void USBD_CDCNotificationInProc(void)
{
	uint16_t theCount = 0;
	SetDP(1, gTestBuff, 4, &theCount);
	PCD_SET_EP_TX_STATUS(USB, 1, USB_EP_TX_VALID);
}

void USBD_CDCDataInProc(void)
{
	uint16_t theCount = 0;
	GetDP(0, gCDCRxBuff, &theCount);
	PCD_SET_EP_TX_STATUS(USB, 1, USB_EP_RX_VALID);
}

void USBD_CDCDataOutProc(void)
{
	USBD_CDCNotificationInProc();
}

void USBD_CDCDataSend(const char str[])
{
	if(isCDCInit){
		int sizeTx;
		char buffer[0x64];
    sprintf(buffer, "%s\r", str);
		SetDP(1, buffer, strlen(buffer), &sizeTx);
		PCD_SET_EP_TXRX_STATUS(USB, 1, USB_EP_RX_VALID, USB_EP_TX_VALID);
	}
}