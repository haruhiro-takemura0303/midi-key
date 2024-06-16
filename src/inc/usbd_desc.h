/**
* @file usbd_desc.h
* @brief usb device descriptor
* @details 
*/

#ifndef USB_DESCRIPTORS_H
#define USB_DESCRIPTORS_H

#include <stdint.h>
#include "usbd_cdc_desc.h"

/* Type USB Descriptor */
enum {
	USB_DESC_DEVICE = 0x01,
	USB_DESC_CONFIGURATION,
	USB_DESC_STRING,
	USB_DESC_INTERFACE,
	USB_DESC_ENDPOINT,
	USB_DESC_DEVICE_QUALIFIER,
	USB_DESC_OTHER_SPEED_CONFIGURSTION,
	USB_DESC_INTERFACE_ASSOCIATION,
};

/* Device Descriptor */
typedef struct {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bcdUSB;
    uint8_t bDeviceClass;
    uint8_t bDeviceSubClass;
    uint8_t bDeviceProtocol;
    uint8_t bMaxPacketSize0;
    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;
    uint8_t iManufacturer;
    uint8_t iProduct;
    uint8_t iSerialNumber;
    uint8_t bNumConfigurations;
} USBDeviceDescriptor;

enum {
    DEVICE_DESCRIPTOR_LENGTH = 18,
};

extern const USBDeviceDescriptor dev_desc;

/* String Descriptor */
typedef struct {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t wLANGID[20];
} USBStringDescriptor;

extern const USBStringDescriptor string_desc[10]; 

#endif // USB_DESCRIPTORS_H
