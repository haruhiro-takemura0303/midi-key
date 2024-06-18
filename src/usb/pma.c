/**
* @file pma_io.c
* @brief pma nterface
* @details -
*/

#include "stdint.h"
#include "stdbool.h"
#include "stm32f3xx.h"
#include "usbd_core.h"
#include "pma.h"

static uint16_t tblEndpointBuffSize[] = {
	SIZEOF_DATA_BUFF_EP0,
	SIZEOF_DATA_BUFF_EP1,
	SIZEOF_DATA_BUFF_EP2,
	SIZEOF_DATA_BUFF_EP3
};

volatile static  bool isInitPMA = false;
volatile static uint16_t PMAHeadAddr = 0;
volatile Packet_Buffer_t* gPacketBufferPtr;

inline Packet_Buffer_t* GetPacketBufferPtr(int epNum)
{
	return (Packet_Buffer_t*)(&gPacketBufferPtr[epNum]);
}
/**
* @brief AllocPMA
* @param int size
* @param -
* @return - uint16_t pma_addr
* @sa -
* @details pma head adr manager. return usb local addr.
*/
uint16_t AllocPMA(int size)
{
	uint16_t ret = PMAHeadAddr;
	
	// Increment PMA Head
	if(size % 2 == 0){
		PMAHeadAddr += size;
	} else {
		PMAHeadAddr += size + 1;
	}
	
	return ret;
}

/**
* @brief ConvPMAOfst2Addr 
* @param uint16_t ofst
* @param -
* @return uint32_t* addr
* @sa -
* @details Convert USB Local Addr to Memory addr that AHB Accesse
*/
uint32_t* ConvPMAOfst2Addr(uint16_t ofst)
{
	return (uint32_t*)(USB_PMAADDR + (2 * ofst));
}

/**
* @brief InitPacketBufferTbl
* @param void
* @param -
* @return void
* @sa -
* @details Init and located Packet Buffer Table. 
*/
void InitPacketBufferTbl(void)
{
	uint16_t tableOfst = AllocPMA(4 * SIZEOF_BUFFET_TBL);
	USB->BTABLE = tableOfst;
	gPacketBufferPtr = (Packet_Buffer_t *)(ConvPMAOfst2Addr(tableOfst)); 
}

/**
* @brief InitPacketBuffer
* @param int endpointNm
* @param uint16_t size
* @return void
* @sa -
* @details Init and Locate Packet Buffer Per Endpoint
*/
void InitPacketBuffer(int endpointNum, uint16_t size)
{	
	uint16_t theRxBuffSize = 0;
	uint16_t numBlock = 0;
	
	/* Tx Endpoint*/
	// Size
	gPacketBufferPtr[endpointNum].CountTX =  0;
	// USB Local Addr
	gPacketBufferPtr[endpointNum].AddrTX = AllocPMA(size);
	
	/* Rx Endpoint */
	if( size > 62 ) {
		numBlock = (uint16_t)((size-1) / 32);
		gPacketBufferPtr[endpointNum].CountRX = (uint16_t)(numBlock << 10);
		gPacketBufferPtr[endpointNum].CountRX |= 1 << 15;
		theRxBuffSize = 32 * (numBlock + 1);
	}else{
		gPacketBufferPtr[endpointNum].CountRX = (uint16_t)(size << 10);
		theRxBuffSize = size;
	}
	// Size
	gPacketBufferPtr[endpointNum].AddrRX = AllocPMA(theRxBuffSize);
}

/**
* @brief PutPMA 
* @param uint16_t ofst
* @param const void* src 
* @param uint16_t size
* @return void
* @sa -
* @details Located to PMA from App. 
*/
void PutPMA(uint16_t ofst, const void* src, uint16_t size) 
{
	uint16_t* pma_addr = (uint16_t *)ConvPMAOfst2Addr(ofst);
  const uint16_t* src16 = (const uint16_t*) src;

  // Copy data
  for (int i = 0; i < size / 2; ++i){
		*pma_addr = *src16;
		pma_addr += 2;
		src16++;
   }

   // Odd Size Care
   if (size % 2 != 0) {
		uint8_t* pma_byte_addr = (uint8_t*)&pma_addr;
		const uint8_t* src_byte = (const uint8_t*) src;
    	*pma_byte_addr = *src_byte;
    }
}

/**
* @brief GetPMA
* @param uint16_t ofst
* @param void* dst
* @param uint16_t size
* @return -
* @sa -
* @details Get Data from PMA 
*/
void GetPMA(uint16_t ofst, void* dst, uint16_t size)
{
	const uint16_t* pma_addr = (const uint16_t*)ConvPMAOfst2Addr(ofst);
	uint16_t* dst16 = (uint16_t*) dst;

  // Copy Data
  for (int i = 0; i < size / 2; ++i) {
		*dst16 = *pma_addr;
		pma_addr += 2;
		dst16++;
	}

  // Odd Size Care
  if (size % 2 != 0) {
		uint8_t* dst_byte = (uint8_t*)dst16;
		const uint8_t* pma_byte_addr = (const uint8_t*)pma_addr;
		*dst_byte = *pma_byte_addr;
	}
}

void GetDP(uint32_t endp, void* dst, uint16_t* sizeRX)
{
	*sizeRX = gPacketBufferPtr[endp].CountRX & USB_COUNT_RX_MSK;
	GetPMA(gPacketBufferPtr[endp].AddrRX, dst, gPacketBufferPtr[endp].CountRX & USB_COUNT_RX_MSK);
}

void SetDP(uint32_t endp, void* dst, uint16_t size, uint16_t* sizeTX)
{
	uint16_t theSize = size;
	// Check DP Range
	if(size > tblEndpointBuffSize[endp]){
		theSize = tblEndpointBuffSize[endp];
	}
	PutPMA(gPacketBufferPtr[endp].AddrTX, dst, theSize);
	gPacketBufferPtr[endp].CountTX = theSize;
	*sizeTX = theSize;
}

/**
* @brief InitPMA
* @param void
* @param -
* @return void
* @sa -
* @details Init PMA.
*/
void InitPMA(void)
{
	if(!isInitPMA){
		/* Init Packet Buffer Tbl */
		InitPacketBufferTbl();
		/* Init Packet Buffet */
		InitPacketBuffer(0, 64);
		InitPacketBuffer(1, 64);
		InitPacketBuffer(2, 64);
		InitPacketBuffer(3, 10);
		isInitPMA = true;
	}
}
