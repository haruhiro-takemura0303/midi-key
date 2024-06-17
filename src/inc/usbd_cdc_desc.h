#include "stdint.h" 

#ifndef _usbd_cdc_desc_h_
#define _usbd_cdc_desc_h_


#define LOBYTE(x)  ((uint8_t)((x) & 0x00FFU))
#define HIBYTE(x)  ((uint8_t)(((x) & 0xFF00U) >> 8U))

#define  USB_DESC_TYPE_DEVICE                           0x01U
#define  USB_DESC_TYPE_CONFIGURATION                    0x02U
#define  USB_DESC_TYPE_STRING                           0x03U
#define  USB_DESC_TYPE_INTERFACE                        0x04U
#define  USB_DESC_TYPE_ENDPOINT                         0x05U
#define  USB_DESC_TYPE_DEVICE_QUALIFIER                 0x06U
#define  USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION        0x07U
#define  USB_DESC_TYPE_BOS                              0x0FU

#define CDC_IN_EP                                   0x81U  /* EP1 for data IN */
#define CDC_OUT_EP                                  0x01U  /* EP1 for data OUT */
#define CDC_CMD_EP                                  0x82U  /* EP2 for CDC commands */
#define CDC_CMD_PACKET_SIZE                         8U  /* Control Endpoint Packet size */
#define USB_CDC_CONFIG_DESC_SIZE 67U
#define CDC_FS_BINTERVAL                          0x10U
#define CDC_DATA_FS_MAX_PACKET_SIZE                 64U  /* Endpoint IN & OUT Packet size */

typedef enum {
    CDC_SEND_ENCAPSULATED_COMMAND = 0x00,
    CDC_GET_ENCAPSULATED_RESPONSE = 0x01,
    CDC_SET_COMM_FEATURE = 0x02,
    CDC_GET_COMM_FEATURE = 0x03,
    CDC_CLEAR_COMM_FEATURE = 0x04,
    CDC_SET_LINE_CODING = 0x20,
    CDC_GET_LINE_CODING = 0x21,
    CDC_SET_CONTROL_LINE_STATE = 0x22,
    CDC_SEND_BREAK = 0x23
} CDC_ClassRequests;


typedef struct {
    uint32_t dwDTERate;   // Baud rate
    uint8_t bCharFormat;  // Stop bits (0: 1 Stop bit, 1: 1.5 Stop bits, 2: 2 Stop bits)
    uint8_t bParityType;  // Parity (0: None, 1: Odd, 2: Even, 3: Mark, 4: Space)
    uint8_t bDataBits;    // Data bits (5, 6, 7, 8 or 16)
} CDC_Line_Coding_t;

enum {
	CDC_LINE_CODING_LENGTH = sizeof(CDC_Line_Coding_t),
};

extern CDC_Line_Coding_t cdc_line_coding;

extern uint8_t cdc_config_desc[USB_CDC_CONFIG_DESC_SIZE];


#endif //_usbd_cdc_de_c_.h
