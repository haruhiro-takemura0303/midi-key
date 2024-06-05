/**
* @file usbd_core.c
* @brief usb device Ctrler
* @details 
*/

#ifndef _usbd_core_h_
#define _usbd_core_h_

#define SIZEOF_DATA_BUFF_EP0 64U
#define SIZEOF_DATA_BUFF_EP1 64U
#define SIZEOF_DATA_BUFF_EP2 64U
#define SIZEOF_DATA_BUFF_EP3 64U

enum {
	EP_0 = 0,
	EP_1,
	EP_2,
	EP_3,
};

void InitUsbDevice(void);

#endif //_usbd_core_h_




