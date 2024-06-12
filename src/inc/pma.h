/**
* @file pma_io.h
* @brief pma nterface
* @details -
*/

#include "stdint.h"

#ifndef _pma_io_h
#define _pma_io_h

#define SIZEOF_BUFFET_TBL 8U
#define USB_COUNT_RX_MSK 0x3FFU

typedef struct  {
	uint16_t AddrTX;
	uint16_t Pad1;
	uint16_t CountTX;
	uint16_t Pad2;
	uint16_t AddrRX;
	uint16_t Pad3;
	uint16_t CountRX;
	uint16_t Pad4;
} Packet_Buffer_t;

Packet_Buffer_t* GetPacketBufferPtr(int epNum);
void InitPMA(void);
void InitPacketBuffer(int endpointNum, uint16_t size);
void PutPMA(uint16_t ofst, const void* src, uint16_t size);
void GetPMA(uint16_t ofst, void* dst, uint16_t size);
void GetDP(uint32_t endp, void* dst, uint16_t* size);
void SetDP(uint32_t endp, void* dst, uint16_t size, uint16_t* sizeTX);
#endif
