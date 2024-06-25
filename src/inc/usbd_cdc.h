

#ifndef _usbd_cdc_h
#define _usbd_cdc_h

void USBD_CDCInit(void);
void USBD_CDCNotificationInProc(void);
void USBD_CDCDataInProc(void);
void USBD_CDCDataOutProc(void);
void USBD_CDCDataSend(const char str[]);
#endif