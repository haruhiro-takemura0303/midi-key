
#include "usbd_cdc_desc.h"


// Configuration Descriptor
const USB_CDC_ConfigurationDescriptor cdc_config_desc = {
    .configDesc = {
        .bLength = sizeof(USB_ConfigurationDescriptor),
        .bDescriptorType = 0x02,            // CONFIGURATION descriptor type
        .wTotalLength = sizeof(USB_CDC_ConfigurationDescriptor),
        .bNumInterfaces = 2,                // Number of interfaces
        .bConfigurationValue = 1,           // Configuration value
        .iConfiguration = 0,                // Configuration string index
        .bmAttributes = 0x80,               // Bus powered
        .bMaxPower = 50,                    // 100mA
    },
    .cdcInterface = {
        .bLength = sizeof(USB_InterfaceDescriptor),
        .bDescriptorType = 0x04,            // INTERFACE descriptor type
        .bInterfaceNumber = 0,              // Interface number
        .bAlternateSetting = 0,             // Alternate setting
        .bNumEndpoints = 1,                 // Number of endpoints
        .bInterfaceClass = 0x02,            // Communication Interface Class
        .bInterfaceSubClass = 0x02,         // Abstract Control Model
        .bInterfaceProtocol = 0x01,         // AT commands
        .iInterface = 0,                    // Interface string index
    },
    .headerFuncDesc = {
        .bFunctionLength = sizeof(USB_CDC_HeaderFuncDescriptor),
        .bDescriptorType = 0x24,            // CS_INTERFACE descriptor type
        .bDescriptorSubtype = 0x00,         // Header functional descriptor
        .bcdCDC = 0x0110,                   // CDC specification version 1.10
    },
    .callMgmtFuncDesc = {
        .bFunctionLength = sizeof(USB_CDC_CallMgmtFuncDescriptor),
        .bDescriptorType = 0x24,            // CS_INTERFACE descriptor type
        .bDescriptorSubtype = 0x01,         // Call Management functional descriptor
        .bmCapabilities = 0x00,             // Device does not handle call management itself
        .bDataInterface = 1,                // Data interface number
    },
    .acmFuncDesc = {
        .bFunctionLength = sizeof(USB_CDC_ACMFuncDescriptor),
        .bDescriptorType = 0x24,            // CS_INTERFACE descriptor type
        .bDescriptorSubtype = 0x02,         // Abstract Control Management functional descriptor
        .bmCapabilities = 0x02,             // Device supports the request combination of Set_Line_Coding, Set_Control_Line
    },
    .unionFuncDesc = {
        .bFunctionLength = sizeof(USB_CDC_UnionFuncDescriptor),
        .bDescriptorType = 0x24,            // CS_INTERFACE descriptor type
        .bDescriptorSubtype = 0x06,         // Union functional descriptor
        .bMasterInterface = 0,              // Master interface number
        .bSlaveInterface0 = 1,              // First slave interface number
    },
    .notificationEndpoint = {
        .bLength = sizeof(USB_EndpointDescriptor),
        .bDescriptorType = 0x05,            // ENDPOINT descriptor type
        .bEndpointAddress = 0x81,           // IN endpoint 1
        .bmAttributes = 0x03,               // Interrupt
        .wMaxPacketSize = 0x0008,           // Maximum packet size
        .bInterval = 0xFF,                  // Polling interval
    },
    .dataInterface = {
        .bLength = sizeof(USB_InterfaceDescriptor),
        .bDescriptorType = 0x04,            // INTERFACE descriptor type
        .bInterfaceNumber = 1,              // Interface number
        .bAlternateSetting = 0,             // Alternate setting
        .bNumEndpoints = 2,                 // Number of endpoints
        .bInterfaceClass = 0x0A,            // Data Interface Class
        .bInterfaceSubClass = 0x00,         // No subclass
        .bInterfaceProtocol = 0x00,         // No protocol
        .iInterface = 0,                    // Interface string index
    },
    .dataOutEndpoint = {
        .bLength = sizeof(USB_EndpointDescriptor),
        .bDescriptorType = 0x05,            // ENDPOINT descriptor type
        .bEndpointAddress = 0x02,           // OUT endpoint 2
        .bmAttributes = 0x02,               // Bulk
        .wMaxPacketSize = 0x0040,           // Maximum packet size
        .bInterval = 0x00,                  // Polling interval
    },
    .dataInEndpoint = {
        .bLength = sizeof(USB_EndpointDescriptor),
        .bDescriptorType = 0x05,            // ENDPOINT descriptor type
        .bEndpointAddress = 0x82,           // IN endpoint 2
        .bmAttributes = 0x02,               // Bulk
        .wMaxPacketSize = 0x0040,           // Maximum packet size
        .bInterval = 0x00,                  // Polling interval
    },
};
