/**
* @file pma_io.h
* @brief pma nterface
* @details -
*/

#include "stdint.h"

#ifndef _pma_io_h
#define _pma_io_h

void InitPMA(void);
void InitPacketBuffer(int endpointNum, uint16_t size);
void PutPMA(uint16_t offset, const void* src, int size);
void GetPMA(uint16_t offset, void* dst, int size);
void GetDP(uint32_t endp, void* dst, uint16_t* size);
void SetDP(uint32_t endp, void* dst, uint16_t size, uint16_t* sizeTX);
#endif
