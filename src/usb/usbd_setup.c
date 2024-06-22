#include "stdbool.h"
#include "stm32f3xx.h"
#include "usbd_setup.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "pma.h"

typedef struct{
	eUSB_Data_Stage_Type_t type;
	eUSB_Data_Stage_State_t state;
	uint8_t* pData;
	uint16_t padd1;
	uint16_t txNum;
	uint16_t count;  
} USB_Data_Stage_Manager_t;


USB_Setup_DP_t gSetupDP = {0};
USB_Data_Stage_Manager_t gDataStageManager = {0};
uint8_t gRxBuff[SIZEOF_DATA_BUFF_EP0] = {0};
uint8_t* pAckData;

static bool SetDescriptorPtr(void);
static void USBDCtrlPutData(void);

void USBCtrlPutZLP(void)
{	
	uint16_t theCount = 0;
	SetDP(0, pAckData, 0, &theCount);
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

static bool SetDescriptorPtr(void)
{
	bool ret = true;
	const uint8_t type = (gSetupDP.wValue >> 8) & 0xFF;
	const uint8_t num = gSetupDP.wValue & 0xFF;
	
	switch(type){
	case USB_DESC_DEVICE:
		gDataStageManager.pData = (uint8_t *)&dev_desc;
		gDataStageManager.count = DEVICE_DESCRIPTOR_LENGTH;
		break;
	case USB_DESC_CONFIGURATION:
		gDataStageManager.pData = (uint8_t *)&cdc_config_desc;
		gDataStageManager.count = USB_CDC_CONFIG_DESC_SIZE;
		break;
	case USB_DESC_STRING:
		gDataStageManager.pData = (uint8_t *)&string_desc[num];
		gDataStageManager.count = string_desc[num].bLength;
		break;
	case USB_DESC_INTERFACE:
	case USB_DESC_ENDPOINT:
		ret = false;
		break;
	case USB_DESC_DEVICE_QUALIFIER:
	case USB_DESC_OTHER_SPEED_CONFIGURSTION:
		ret = false;
		break;
	}
	return ret;
}


int USBDCtrlSetupStageClassProc(void)
{
	int ret = 0;
	
	switch (gSetupDP.bRequest) {
  case CDC_SET_LINE_CODING:
		gDataStageManager.pData = gRxBuff;
		gDataStageManager.count = gSetupDP.wLength;
		ret = DATA_OUT_STAGE;
		break;
  case CDC_GET_LINE_CODING:
		gDataStageManager.pData = (uint8_t *)&cdc_line_coding;
		gDataStageManager.count = CDC_LINE_CODING_LENGTH;
		ret = DATA_IN_STAGE;
		break;
  case CDC_SET_CONTROL_LINE_STATE:
		ret = NO_DATA_STAGE;
		break;
  case CDC_SEND_BREAK:
		ret = NO_DATA_STAGE;
		break;
   default:
		 ret = SETUP_STALL;
		 break;
    }
	return ret;
}

int USBDCtrlSetupStageStandardProc(void)
{
	// Get Setup DP
	int ret = 0;

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
			if(SetDescriptorPtr()){
				ret = DATA_IN_STAGE;
			} else {
				ret = SETUP_STALL;
				gDataStageManager.count = 0;
			}
		}
		break;
	case SET_DESCRIPTOR:
		ret = DATA_OUT_STAGE;
		break;
	case GET_CONFIGURATION:
		ret = DATA_IN_STAGE;
		break;
	case SET_CONFIGURATION:
		ret = NO_DATA_STAGE;
		/* Init CDC */
		USBD_CDCInit();
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

#define USBD_BM_REQUEST_TYPE_Msk 0x60U
#define USBD_BM_REQUEST_TYPE_Pos 5U

int USBDCtrlSetupStageProc(void)
{
	int ret = 0;
	uint16_t  size = 0;
	
	GetDP(0, &gSetupDP, &size);
	
	switch((gSetupDP.bmRequestType & USBD_BM_REQUEST_TYPE_Msk) >> USBD_BM_REQUEST_TYPE_Pos){
	case REQUEST_TYPE_STANDARD:
		ret = USBDCtrlSetupStageStandardProc();
		break;
  case REQUEST_TYPE_CLASS:
		ret = USBDCtrlSetupStageClassProc();
		break;
  case REQUEST_TYPE_VENDOR:
		ret = SETUP_STALL;
		break;
	}
	
	return ret;
}

void USBDCtrlDataInStageProc(void)
{	
	// Update TX Num
	gDataStageManager.count = gSetupDP.wLength < gDataStageManager.count ? gSetupDP.wLength : gDataStageManager.count;
	
	// Update State
	if(gDataStageManager.count == 0){
		gDataStageManager.state = DATA_STAGE_FINISH;
	} else if (gDataStageManager.count <= gSetupDP.wLength){
		gDataStageManager.state = DATA_STAGE_LAST;
	} else {
		gDataStageManager.state = DATA_STAGE_CONTINUE;
	}

	// Tx Data
	switch(gDataStageManager.state){
	case DATA_STAGE_CONTINUE:
		PCD_SET_EP_RX_STATUS(USB, 0, USB_EP_RX_STALL);
		USBDCtrlPutData();
		PCD_SET_EP_TX_STATUS(USB, 0, USB_EP_TX_VALID);
		break;
	case DATA_STAGE_LAST:
		PCD_SET_EP_RX_STATUS(USB, 0, USB_EP_RX_NAK);
		USBDCtrlPutData();
		PCD_SET_OUT_STATUS(USB, 0);
		PCD_SET_EP_TXRX_STATUS(USB, 0, USB_EP_RX_VALID, USB_EP_TX_VALID);
		break;
	case DATA_STAGE_FINISH:
		USBCtrlPutZLP();
		PCD_SET_EP_TXRX_STATUS(USB, 0, USB_EP_RX_VALID, USB_EP_TX_NAK);
		break;
	}
	
}

void USBDCtrlGetData(void)
{
		uint16_t size = 0;
		GetDP(0, gDataStageManager.pData, &size);
		gDataStageManager.count -= size;
}

void USBDCtrlDataOutStageClassProc(void)
{

	switch (gSetupDP.bRequest) {
  case CDC_SET_CONTROL_LINE_STATE:
		break;
	case CDC_SET_LINE_CODING:
		break;
  case CDC_GET_LINE_CODING:
  default:
		 break;
   }
}

void USBDCtrlDataOutStageStandardProc(void)
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

void USBDCtrlDataOutStageProc(void)
{
	USBDCtrlGetData();
	
	// Update State
	if(gDataStageManager.count == 0){
		gDataStageManager.state = DATA_STAGE_FINISH;
	}
	else {
		gDataStageManager.state = DATA_STAGE_CONTINUE;
	}
	
	switch((gSetupDP.bmRequestType & USBD_BM_REQUEST_TYPE_Msk) >> USBD_BM_REQUEST_TYPE_Pos){
	case REQUEST_TYPE_STANDARD:
		USBDCtrlDataOutStageStandardProc();
		break;
  case REQUEST_TYPE_CLASS:
		USBDCtrlDataOutStageClassProc();
		break;
  case REQUEST_TYPE_VENDOR:
		break;
	}
	
	// Rx Data
	switch(gDataStageManager.state){
	case DATA_STAGE_CONTINUE:
		PCD_SET_EP_RX_STATUS(USB, 0, USB_EP_TX_STALL);
		USBDCtrlPutData();
		PCD_SET_EP_TX_STATUS(USB, 0, USB_EP_RX_VALID);
		break;
	case DATA_STAGE_LAST:
		break;
	case DATA_STAGE_FINISH:
		USBCtrlPutZLP();
		PCD_SET_EP_TXRX_STATUS(USB, 0, USB_EP_RX_VALID, USB_EP_TX_VALID);
		break;
	}
}

void USBCtrlStatusInStageProc(void)
{
	switch(gSetupDP.bRequest){
	case SET_ADDRESS:
		USB->DADDR |= gSetupDP.wValue;
		break;
	default:
		break;
	}
}
