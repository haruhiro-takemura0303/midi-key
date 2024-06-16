#include "stdint.h"
#include "usbd_desc.h"


// For USB CDC
const USBDeviceDescriptor dev_desc = {
        .bLength = DEVICE_DESCRIPTOR_LENGTH,
        .bDescriptorType = USB_DESC_DEVICE,
        .bcdUSB = 0x200, // USB2.0
        .bDeviceClass = 2, // Communication
        .bDeviceSubClass = 0, // tekitou
        .bDeviceProtocol = 0,
        .bMaxPacketSize0 = 64,
        .idVendor = 0xdead,
        .idProduct = 0xbeef,
        .bcdDevice = 0x100,
        .iManufacturer = 2,
        .iProduct = 3,
        .iSerialNumber = 1,
        .bNumConfigurations = 1
};

const USBStringDescriptor string_desc[10] = {
        { // Default
                .bLength = 3 * 2 + 2,
                .bDescriptorType = USB_DESC_STRING,
                .wLANGID = {'E', 'N', 'G'}},
        { // Manufacture
                .bLength = 4 * 2 + 2,
                .bDescriptorType = USB_DESC_STRING,
                .wLANGID = {'T', 'E', 'S', 'T'}
        },
        { // Product
                .bLength = 10 * 2 + 2,
                .bDescriptorType = USB_DESC_STRING,
                .wLANGID = {'S', 'h', 'i', 't', ' ', 'C', 'o', 'r', 'p', '.'}
        },
        { // Serial
                .bLength = 15 * 2 + 2,
                .bDescriptorType = USB_DESC_STRING,
                .wLANGID = {'U', 'n', 'c', 'o', 'm', 'm', 'u', 'n', 'i', 'c', 'a', 't', 'i', 'o', 'n'}
        },
        {}, //dumm
};

