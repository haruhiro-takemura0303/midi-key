/**
* @file usbd_core.c
* @brief usb device Ctrler
* @details 
*/

#include "stm32f3xx.h"

#ifndef _usbd_core_h_
#define _usbd_core_h_

/* Define ------------------------------------------------------------*/
// Endpoint Number
#define EP_0 0U
#define EP_1 1U
#define EP_2 2U
#define EP_3 3U
#define EP_4 4U
#define EP_5 5U
#define EP_6 6U
#define EP_7 7U
#define EP_8 8U

// Data Buffer Size
#define SIZEOF_DATA_BUFF_EP0 64U
#define SIZEOF_DATA_BUFF_EP1 64U
#define SIZEOF_DATA_BUFF_EP2 64U
#define SIZEOF_DATA_BUFF_EP3 64U


/* Private macros ------------------------------------------------------------*/
/** @defgroup PCD_Private_Macros PCD Private Macros
  * @{
  */

/********************  Bit definition for USB_COUNTn_RX register  *************/
#define USB_CNTRX_NBLK_MSK                    (0x1FU << 10)
#define USB_CNTRX_BLSIZE                      (0x1U << 15)

/* SetENDPOINT */
#define PCD_SET_ENDPOINT(USBx, bEpNum, wRegValue) \
  (*(__IO uint16_t *)(&(USBx)->EP0R + ((bEpNum) * 2U)) = (uint16_t)(wRegValue))

/* GetENDPOINT */
#define PCD_GET_ENDPOINT(USBx, bEpNum)             (*(__IO uint16_t *)(&(USBx)->EP0R + ((bEpNum) * 2U)))


/**
  * @brief  sets the type in the endpoint register(bits EP_TYPE[1:0])
  * @param  USBx USB peripheral instance register address.
  * @param  bEpNum Endpoint Number.
  * @param  wType Endpoint Type.
  * @retval None
  */
#define PCD_SET_EPTYPE(USBx, bEpNum, wType) \
  (PCD_SET_ENDPOINT((USBx), (bEpNum), \
                    ((PCD_GET_ENDPOINT((USBx), (bEpNum)) & USB_EP_T_MASK) | (wType) | USB_EP_CTR_TX | USB_EP_CTR_RX)))


/**
  * @brief  gets the type in the endpoint register(bits EP_TYPE[1:0])
  * @param  USBx USB peripheral instance register address.
  * @param  bEpNum Endpoint Number.
  * @retval Endpoint Type
  */
#define PCD_GET_EPTYPE(USBx, bEpNum) (PCD_GET_ENDPOINT((USBx), (bEpNum)) & USB_EP_T_FIELD)

/**
  * @brief free buffer used from the application realizing it to the line
  *         toggles bit SW_BUF in the double buffered endpoint register
  * @param USBx USB device.
  * @param   bEpNum, bDir
  * @retval None
  */
#define PCD_FREE_USER_BUFFER(USBx, bEpNum, bDir) \
  do { \
    if ((bDir) == 0U) \
    { \
      /* OUT double buffered endpoint */ \
      PCD_TX_DTOG((USBx), (bEpNum)); \
    } \
    else if ((bDir) == 1U) \
    { \
      /* IN double buffered endpoint */ \
      PCD_RX_DTOG((USBx), (bEpNum)); \
    } \
  } while(0)

/**
  * @brief  sets the status for tx transfer (bits STAT_TX[1:0]).
  * @param  USBx USB peripheral instance register address.
  * @param  bEpNum Endpoint Number.
  * @param  wState new state
  * @retval None
  */
#define PCD_SET_EP_TX_STATUS(USBx, bEpNum, wState) \
  do { \
    uint16_t _wRegVal; \
    \
    _wRegVal = PCD_GET_ENDPOINT((USBx), (bEpNum)) & USB_EPTX_DTOGMASK; \
    /* toggle first bit ? */ \
    if ((USB_EPTX_DTOG1 & (wState))!= 0U) \
    { \
      _wRegVal ^= USB_EPTX_DTOG1; \
    } \
    /* toggle second bit ?  */ \
    if ((USB_EPTX_DTOG2 & (wState))!= 0U) \
    { \
      _wRegVal ^= USB_EPTX_DTOG2; \
    } \
    PCD_SET_ENDPOINT((USBx), (bEpNum), (_wRegVal | USB_EP_CTR_RX | USB_EP_CTR_TX)); \
  } while(0) /* PCD_SET_EP_TX_STATUS */

/**
  * @brief  sets the status for rx transfer (bits STAT_TX[1:0])
  * @param  USBx USB peripheral instance register address.
  * @param  bEpNum Endpoint Number.
  * @param  wState new state
  * @retval None
  */
#define PCD_SET_EP_RX_STATUS(USBx, bEpNum,wState) \
  do { \
    uint16_t _wRegVal; \
    \
    _wRegVal = PCD_GET_ENDPOINT((USBx), (bEpNum)) & USB_EPRX_DTOGMASK; \
    /* toggle first bit ? */ \
    if ((USB_EPRX_DTOG1 & (wState))!= 0U) \
    { \
      _wRegVal ^= USB_EPRX_DTOG1; \
    } \
    /* toggle second bit ? */ \
    if ((USB_EPRX_DTOG2 & (wState))!= 0U) \
    { \
      _wRegVal ^= USB_EPRX_DTOG2; \
    } \
    PCD_SET_ENDPOINT((USBx), (bEpNum), (_wRegVal | USB_EP_CTR_RX | USB_EP_CTR_TX)); \
  } while(0) /* PCD_SET_EP_RX_STATUS */

/**
  * @brief  sets the status for rx & tx (bits STAT_TX[1:0] & STAT_RX[1:0])
  * @param  USBx USB peripheral instance register address.
  * @param  bEpNum Endpoint Number.
  * @param  wStaterx new state.
  * @param  wStatetx new state.
  * @retval None
  */
#define PCD_SET_EP_TXRX_STATUS(USBx, bEpNum, wStaterx, wStatetx) \
  do { \
    uint16_t _wRegVal; \
    \
    _wRegVal = PCD_GET_ENDPOINT((USBx), (bEpNum)) & (USB_EPRX_DTOGMASK | USB_EPTX_STAT); \
    /* toggle first bit ? */ \
    if ((USB_EPRX_DTOG1 & (wStaterx))!= 0U) \
    { \
      _wRegVal ^= USB_EPRX_DTOG1; \
    } \
    /* toggle second bit ? */ \
    if ((USB_EPRX_DTOG2 & (wStaterx))!= 0U) \
    { \
      _wRegVal ^= USB_EPRX_DTOG2; \
    } \
    /* toggle first bit ? */ \
    if ((USB_EPTX_DTOG1 & (wStatetx))!= 0U) \
    { \
      _wRegVal ^= USB_EPTX_DTOG1; \
    } \
    /* toggle second bit ?  */ \
    if ((USB_EPTX_DTOG2 & (wStatetx))!= 0U) \
    { \
      _wRegVal ^= USB_EPTX_DTOG2; \
    } \
    \
    PCD_SET_ENDPOINT((USBx), (bEpNum), (_wRegVal | USB_EP_CTR_RX | USB_EP_CTR_TX)); \
  } while(0) /* PCD_SET_EP_TXRX_STATUS */

/**
  * @brief  gets the status for tx/rx transfer (bits STAT_TX[1:0]
  *         /STAT_RX[1:0])
  * @param  USBx USB peripheral instance register address.
  * @param  bEpNum Endpoint Number.
  * @retval status
  */
#define PCD_GET_EP_TX_STATUS(USBx, bEpNum)     ((uint16_t)PCD_GET_ENDPOINT((USBx), (bEpNum)) & USB_EPTX_STAT)
#define PCD_GET_EP_RX_STATUS(USBx, bEpNum)     ((uint16_t)PCD_GET_ENDPOINT((USBx), (bEpNum)) & USB_EPRX_STAT)


/**
  * @brief  set & clear EP_KIND bit.
  * @param  USBx USB peripheral instance register address.
  * @param  bEpNum Endpoint Number.
  * @retval None
  */
#define PCD_SET_EP_KIND(USBx, bEpNum) \
  do { \
    uint16_t _wRegVal; \
    \
    _wRegVal = PCD_GET_ENDPOINT((USBx), (bEpNum)) & USB_EPREG_MASK; \
    \
    PCD_SET_ENDPOINT((USBx), (bEpNum), (_wRegVal | USB_EP_CTR_RX | USB_EP_CTR_TX | USB_EP_KIND)); \
  } while(0) /* PCD_SET_EP_KIND */

#define PCD_CLEAR_EP_KIND(USBx, bEpNum) \
  do { \
    uint16_t _wRegVal; \
    \
    _wRegVal = PCD_GET_ENDPOINT((USBx), (bEpNum)) & USB_EPKIND_MASK; \
    \
    PCD_SET_ENDPOINT((USBx), (bEpNum), (_wRegVal | USB_EP_CTR_RX | USB_EP_CTR_TX)); \
  } while(0) /* PCD_CLEAR_EP_KIND */

/**
  * @brief  Sets/clears directly STATUS_OUT bit in the endpoint register.
  * @param  USBx USB peripheral instance register address.
  * @param  bEpNum Endpoint Number.
  * @retval None
  */
#define PCD_SET_OUT_STATUS(USBx, bEpNum)       PCD_SET_EP_KIND((USBx), (bEpNum))
#define PCD_CLEAR_OUT_STATUS(USBx, bEpNum)     PCD_CLEAR_EP_KIND((USBx), (bEpNum))

/**
  * @brief  Sets/clears directly EP_KIND bit in the endpoint register.
  * @param  USBx USB peripheral instance register address.
  * @param  bEpNum Endpoint Number.
  * @retval None
  */
#define PCD_SET_BULK_EP_DBUF(USBx, bEpNum)     PCD_SET_EP_KIND((USBx), (bEpNum))
#define PCD_CLEAR_BULK_EP_DBUF(USBx, bEpNum)   PCD_CLEAR_EP_KIND((USBx), (bEpNum))

/**
  * @brief  Clears bit CTR_RX / CTR_TX in the endpoint register.
  * @param  USBx USB peripheral instance register address.
  * @param  bEpNum Endpoint Number.
  * @retval None
  */
#define PCD_CLEAR_RX_EP_CTR(USBx, bEpNum) \
  do { \
    uint16_t _wRegVal; \
    \
    _wRegVal = PCD_GET_ENDPOINT((USBx), (bEpNum)) & (0x7FFFU & USB_EPREG_MASK); \
    \
    PCD_SET_ENDPOINT((USBx), (bEpNum), (_wRegVal | USB_EP_CTR_TX)); \
  } while(0) /* PCD_CLEAR_RX_EP_CTR */

#define PCD_CLEAR_TX_EP_CTR(USBx, bEpNum) \
  do { \
    uint16_t _wRegVal; \
    \
    _wRegVal = PCD_GET_ENDPOINT((USBx), (bEpNum)) & (0xFF7FU & USB_EPREG_MASK); \
    \
    PCD_SET_ENDPOINT((USBx), (bEpNum), (_wRegVal | USB_EP_CTR_RX)); \
  } while(0) /* PCD_CLEAR_TX_EP_CTR */

/**
  * @brief  Toggles DTOG_RX / DTOG_TX bit in the endpoint register.
  * @param  USBx USB peripheral instance register address.
  * @param  bEpNum Endpoint Number.
  * @retval None
  */
#define PCD_RX_DTOG(USBx, bEpNum) \
  do { \
    uint16_t _wEPVal; \
    \
    _wEPVal = PCD_GET_ENDPOINT((USBx), (bEpNum)) & USB_EPREG_MASK; \
    \
    PCD_SET_ENDPOINT((USBx), (bEpNum), (_wEPVal | USB_EP_CTR_RX | USB_EP_CTR_TX | USB_EP_DTOG_RX)); \
  } while(0) /* PCD_RX_DTOG */

#define PCD_TX_DTOG(USBx, bEpNum) \
  do { \
    uint16_t _wEPVal; \
    \
    _wEPVal = PCD_GET_ENDPOINT((USBx), (bEpNum)) & USB_EPREG_MASK; \
    \
    PCD_SET_ENDPOINT((USBx), (bEpNum), (_wEPVal | USB_EP_CTR_RX | USB_EP_CTR_TX | USB_EP_DTOG_TX)); \
  } while(0) /* PCD_TX_DTOG */
	
/**
  * @brief  Clears DTOG_RX / DTOG_TX bit in the endpoint register.
  * @param  USBx USB peripheral instance register address.
  * @param  bEpNum Endpoint Number.
  * @retval None
  */
#define PCD_CLEAR_RX_DTOG(USBx, bEpNum) \
  do { \
    uint16_t _wRegVal; \
    \
    _wRegVal = PCD_GET_ENDPOINT((USBx), (bEpNum)); \
    \
    if ((_wRegVal & USB_EP_DTOG_RX) != 0U)\
    { \
      PCD_RX_DTOG((USBx), (bEpNum)); \
    } \
  } while(0) /* PCD_CLEAR_RX_DTOG */

#define PCD_CLEAR_TX_DTOG(USBx, bEpNum) \
  do { \
    uint16_t _wRegVal; \
    \
    _wRegVal = PCD_GET_ENDPOINT((USBx), (bEpNum)); \
    \
    if ((_wRegVal & USB_EP_DTOG_TX) != 0U)\
    { \
      PCD_TX_DTOG((USBx), (bEpNum)); \
    } \
  } while(0) /* PCD_CLEAR_TX_DTOG */

/**
  * @brief  Sets address in an endpoint register.
  * @param  USBx USB peripheral instance register address.
  * @param  bEpNum Endpoint Number.
  * @param  bAddr Address.
  * @retval None
  */
#define PCD_SET_EP_ADDRESS(USBx, bEpNum, bAddr) \
  do { \
    uint16_t _wRegVal; \
    \
    _wRegVal = (PCD_GET_ENDPOINT((USBx), (bEpNum)) & USB_EPREG_MASK) | (bAddr); \
    \
    PCD_SET_ENDPOINT((USBx), (bEpNum), (_wRegVal | USB_EP_CTR_RX | USB_EP_CTR_TX)); \
  } while(0) /* PCD_SET_EP_ADDRESS */

/**
  * @brief  Gets address in an endpoint register.
  * @param  USBx USB peripheral instance register address.
  * @param  bEpNum Endpoint Number.
  * @retval None
  */
#define PCD_GET_EP_ADDRESS(USBx, bEpNum) ((uint8_t)(PCD_GET_ENDPOINT((USBx), (bEpNum)) & USB_EPADDR_FIELD))

#define PCD_EP_TX_CNT(USBx, bEpNum) \
  ((uint16_t *)((((uint32_t)(USBx)->BTABLE + \
                  ((uint32_t)(bEpNum) * 8U) + 2U) * PMA_ACCESS) + ((uint32_t)(USBx) + 0x400U)))

#define PCD_EP_RX_CNT(USBx, bEpNum) \
  ((uint16_t *)((((uint32_t)(USBx)->BTABLE + \
                  ((uint32_t)(bEpNum) * 8U) + 6U) * PMA_ACCESS) + ((uint32_t)(USBx) + 0x400U)))


/**
  * @brief  sets address of the tx/rx buffer.
  * @param  USBx USB peripheral instance register address.
  * @param  bEpNum Endpoint Number.
  * @param  wAddr address to be set (must be word aligned).
  * @retval None
  */
#define PCD_SET_EP_TX_ADDRESS(USBx, bEpNum, wAddr) \
  do { \
    __IO uint16_t *_wRegVal; \
    uint32_t _wRegBase = (uint32_t)USBx; \
    \
    _wRegBase += (uint32_t)(USBx)->BTABLE; \
    _wRegVal = (__IO uint16_t *)(_wRegBase + 0x400U + (((uint32_t)(bEpNum) * 8U) * PMA_ACCESS)); \
    *_wRegVal = ((wAddr) >> 1) << 1; \
  } while(0) /* PCD_SET_EP_TX_ADDRESS */

#define PCD_SET_EP_RX_ADDRESS(USBx, bEpNum, wAddr) \
  do { \
    __IO uint16_t *_wRegVal; \
    uint32_t _wRegBase = (uint32_t)USBx; \
    \
    _wRegBase += (uint32_t)(USBx)->BTABLE; \
    _wRegVal = (__IO uint16_t *)(_wRegBase + 0x400U + ((((uint32_t)(bEpNum) * 8U) + 4U) * PMA_ACCESS)); \
    *_wRegVal = ((wAddr) >> 1) << 1; \
  } while(0) /* PCD_SET_EP_RX_ADDRESS */


void InitUsbDevice(void);

#endif //_usbd_core_h_




