#include "stdint.h" 

#ifndef _usbd_cdc_desc_h_
#define _usbd_cdc_desc_h_

// Configuration Descriptor
typedef struct {
    uint8_t  bLength;             // Size of this descriptor in bytes
    uint8_t  bDescriptorType;     // CONFIGURATION descriptor type (0x02)
    uint16_t wTotalLength;        // Total length of all descriptors returned for this configuration
    uint8_t  bNumInterfaces;      // Number of interfaces supported by this configuration
    uint8_t  bConfigurationValue; // Value to use as an argument to the SetConfiguration request
    uint8_t  iConfiguration;      // Index of string descriptor describing this configuration
    uint8_t  bmAttributes;        // Configuration characteristics
    uint8_t  bMaxPower;           // Maximum power consumption of the USB device from the bus in this configuration (2mA units)
} USB_ConfigurationDescriptor;

// Interface Descriptor
typedef struct {
    uint8_t bLength;            // Size of this descriptor in bytes
    uint8_t bDescriptorType;    // INTERFACE descriptor type (0x04)
    uint8_t bInterfaceNumber;   // Number of this interface
    uint8_t bAlternateSetting;  // Value used to select this alternate setting
    uint8_t bNumEndpoints;      // Number of endpoints used by this interface (excluding endpoint 0)
    uint8_t bInterfaceClass;    // Class code (assigned by the USB-IF)
    uint8_t bInterfaceSubClass; // Subclass code (assigned by the USB-IF)
    uint8_t bInterfaceProtocol; // Protocol code (assigned by the USB-IF)
    uint8_t iInterface;         // Index of string descriptor describing this interface
} USB_InterfaceDescriptor;

// Header Functional Descriptor
typedef struct {
    uint8_t  bFunctionLength;    // Size of this descriptor in bytes
    uint8_t  bDescriptorType;    // CS_INTERFACE descriptor type (0x24)
    uint8_t  bDescriptorSubtype; // Header functional descriptor subtype (0x00)
    uint16_t bcdCDC;             // CDC specification release number
} USB_CDC_HeaderFuncDescriptor;

// Call Management Functional Descriptor
typedef struct {
    uint8_t bFunctionLength;    // Size of this descriptor in bytes
    uint8_t bDescriptorType;    // CS_INTERFACE descriptor type (0x24)
    uint8_t bDescriptorSubtype; // Call Management functional descriptor subtype (0x01)
    uint8_t bmCapabilities;     // Capabilities of the call management
    uint8_t bDataInterface;     // Interface number of the data class interface
} USB_CDC_CallMgmtFuncDescriptor;

// Abstract Control Management Functional Descriptor
typedef struct {
    uint8_t bFunctionLength;    // Size of this descriptor in bytes
    uint8_t bDescriptorType;    // CS_INTERFACE descriptor type (0x24)
    uint8_t bDescriptorSubtype; // Abstract Control Management functional descriptor subtype (0x02)
    uint8_t bmCapabilities;     // Capabilities of the ACM
} USB_CDC_ACMFuncDescriptor;

// Union Functional Descriptor
typedef struct {
    uint8_t bFunctionLength;      // Size of this descriptor in bytes
    uint8_t bDescriptorType;      // CS_INTERFACE descriptor type (0x24)
    uint8_t bDescriptorSubtype;   // Union functional descriptor subtype (0x06)
    uint8_t bMasterInterface;     // The interface number of the Communication or Data Class interface, designated as the master or controlling interface for the union
    uint8_t bSlaveInterface0;     // Interface number of the first slave or associated interface in the union
    // Additional slave interfaces can be added if needed
} USB_CDC_UnionFuncDescriptor;

// Endpoint Descriptor
typedef struct {
    uint8_t bLength;          // Size of this descriptor in bytes
    uint8_t bDescriptorType;  // ENDPOINT descriptor type (0x05)
    uint8_t bEndpointAddress; // The address of the endpoint on the USB device described by this descriptor
    uint8_t bmAttributes;     // Attributes which specify the transfer type (e.g., bulk, interrupt)
    uint16_t wMaxPacketSize;  // Maximum packet size this endpoint is capable of sending or receiving
    uint8_t bInterval;        // Interval for polling endpoint for data transfers
} USB_EndpointDescriptor;

typedef struct {
    USB_ConfigurationDescriptor configDesc;          // Configuration descriptor
    USB_InterfaceDescriptor cdcInterface;            // CDC Communication Interface descriptor
    USB_CDC_HeaderFuncDescriptor headerFuncDesc;     // Header Functional descriptor
    USB_CDC_CallMgmtFuncDescriptor callMgmtFuncDesc; // Call Management Functional descriptor
    USB_CDC_ACMFuncDescriptor acmFuncDesc;           // Abstract Control Management Functional descriptor
    USB_CDC_UnionFuncDescriptor unionFuncDesc;       // Union Functional descriptor
    USB_EndpointDescriptor notificationEndpoint;     // Notification Endpoint descriptor
    USB_InterfaceDescriptor dataInterface;           // CDC Data Interface descriptor
    USB_EndpointDescriptor dataOutEndpoint;          // Data OUT Endpoint descriptor
    USB_EndpointDescriptor dataInEndpoint;           // Data IN Endpoint descriptor
} USB_CDC_ConfigurationDescriptor;

enum {
    CDC_CONFIG_DESCRIPTOR_LENGT = sizeof(USB_CDC_ConfigurationDescriptor),
};

extern const USB_CDC_ConfigurationDescriptor cdc_config_desc;

#endif //_usbd_cdc_de_c_.h