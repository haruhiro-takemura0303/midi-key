/**
* @file	usbd_setup.h 
* @brief usb device setup manager
* @details 
*/

#ifndef _usbd_setup_h_
#define _usbd_setup_h_
#include "stdint.h"

typedef enum{
	GET_STATUS = 0,
	CLEAR_FEATURE,
	REV1,
	SET_FEATURE,
	REV2,
	SET_ADDRESS,
	GET_DESCRIPTOR,
	SET_DESCRIPTOR,
	GET_CONFIGURATION,
	SET_CONFIGURATION,
	GET_INTERFACE,
	SET_INTERFACE,
	SYNCH_FRAME,
	SET_SEL,
	SET_ISOCH_DELAY,
}eUSB_Setup_Request_t;

typedef enum {
	DATA_OUT_STAGE,
	DATA_IN_STAGE,
	NO_DATA_STAGE,
	AIDLE,
}eUSB_Data_Stage_Type_t;

typedef enum {
	DATA_STAGE_CONTINUE,
	DATA_STAGE_LAST,
	DATA_STAGE_FINISH,
}eUSB_Data_Stage_State_t;

typedef struct{
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint16_t wValue;
	uint16_t wIndex;
	uint16_t wLength;
}USB_Setup_DP_t;


int USBDCtrlSetupStageProc(void);
void USBDCtrlDataInStageProc(void);
void USBDCtrlDataOutStageProc(void);
void USBDCtrlGetData(void);
void USBCtrlStatusInStageProc(void);
#endif // _usbd_setup_h_
