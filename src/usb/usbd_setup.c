
#include "stm32f3xx.h"
#include "usbd_setup.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "pma.h"

typedef struct{
	eUSB_Data_Stage_Type_t type;
	eUSB_Data_Stage_State_t state;
	uint8_t* pData;
	uint16_t padd1;
	uint16_t count;  
} USB_Data_Stage_Manager_t;


USB_Setup_DP_t gSetupDP = {0};
USB_Data_Stage_Manager_t gDataStageManager = {0};

static void SetDescriptorPtr(void);


int USBDCtrlSetupStageProc(void)
{
	// Get Setup DP
	int ret = 0;
	uint16_t  size = 0;
	GetDP(0, &gSetupDP, &size);
	
	switch(gSetupDP.bRequest){
	case GET_STATUS:
		ret = DATA_IN_STAGE;
		break;
	case CLEAR_FEATURE:
		break;
	case SET_FEATURE:
		ret = NO_DATA_STAGE;
		break;
	case SET_ADDRESS:
		ret = NO_DATA_STAGE;
		break;
	case GET_DESCRIPTOR:
		if(gDataStageManager.count == 0){
			SetDescriptorPtr();
		}
		ret = DATA_IN_STAGE;
		break;
	case SET_DESCRIPTOR:
		ret = DATA_OUT_STAGE;
		break;
	case GET_CONFIGURATION:
		ret = DATA_IN_STAGE;
		break;
	case SET_CONFIGURATION:
		ret = DATA_OUT_STAGE;
		break;
	case GET_INTERFACE:
		ret = DATA_IN_STAGE;
		break;
	case SET_INTERFACE:
		ret = DATA_OUT_STAGE;
		break;
	case SYNCH_FRAME:
		break;
	case SET_SEL:
		ret = DATA_OUT_STAGE;
		break;
	case SET_ISOCH_DELAY:
		break;
	case REV1:
	case REV2:
		break;
	}
	
	return ret;
}

static void SetDescriptorPtr(void)
{
	const uint8_t desc_type = (gSetupDP.wValue >> 8) & 0xFF;
	switch(desc_type){
	case USB_DESC_DEVICE:
		gDataStageManager.pData = (uint8_t *)&dev_desc;
		gDataStageManager.count = DEVICE_DESCRIPTOR_LENGTH;
		break;
	case USB_DESC_CONFIGURATION:
		break;
	case USB_DESC_STRING:
		break;
	case USB_DESC_INTERFACE:
	case USB_DESC_ENDPOINT:
		break;
	}
}

static void USBDCtrlPutData(void)
{
	uint16_t theCount = 0;
	if(gDataStageManager.pData){
		SetDP(0, gDataStageManager.pData, gDataStageManager.count, &theCount);
		gDataStageManager.count -= theCount;
		gDataStageManager.pData += theCount;
	}
}

void USBDCtrlDataInStageProc(void)
{
	// Update Count
	if(gDataStageManager.count == 0){
		gDataStageManager.state = DATA_STAGE_FINISH;
	} else if (gDataStageManager.count < SIZEOF_DATA_BUFF_EP0){
		gDataStageManager.state = DATA_STAGE_LAST;
	} else {
		gDataStageManager.state = DATA_STAGE_CONTINUE;
	}

	// Tx Data
	switch(gDataStageManager.state){
	case DATA_STAGE_CONTINUE:
		PCD_SET_EP_RX_STATUS(USB, 0, USB_EP_RX_STALL);
		USBDCtrlPutData();
		PCD_TX_DTOG(USB, 0);
		PCD_SET_EP_TX_STATUS(USB, 0, USB_EP_TX_VALID);
		break;
	case DATA_STAGE_LAST:
		PCD_SET_EP_RX_STATUS(USB, 0, USB_EP_RX_NAK);
		USBDCtrlPutData();
		PCD_TX_DTOG(USB, 0);
		PCD_SET_EP_TXRX_STATUS(USB, 0, USB_EP_RX_VALID, USB_EP_TX_VALID);
		break;
	case DATA_STAGE_FINISH:
		PCD_SET_EP_RX_STATUS(USB, 0, USB_EP_RX_NAK);
		break;
	}
}


void USBDCtrlDataOutStageProc(void)
{
	switch(gSetupDP.bRequest){
	case CLEAR_FEATURE:
		break;
	case SET_FEATURE:
		break;
	case SET_ADDRESS:
		break;
	case SET_DESCRIPTOR:
		break;
	case SET_CONFIGURATION:
		break;
	case SET_INTERFACE:
		break;
	case SYNCH_FRAME:
		break;
	case SET_SEL:
		break;
	case SET_ISOCH_DELAY:
		break;
	default:
		break;
	}
}

