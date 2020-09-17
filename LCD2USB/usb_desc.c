#include "usb.h"

const unsigned char usb_device_descriptor[SIZE_DEVICE_DESCRIPTOR] =
{
	SIZE_DEVICE_DESCRIPTOR, // Size of the Descriptor in Bytes
	USB_DESC_TYPE_DEVICE, // Device Descriptor (0x01)
	0x10, 
	0x01, // USB 1.1 = 0x0110; USB 1.0 = 0x0100
	0x00, // Class Code
	0x00, // Subclass Code
	0x00, // Protocol Code
	0x08, // Maximum Packet Size for Zero Endpoint
	0x03, // VID_L
	0x04, // VID_H
	0x30, // PID_L
	0xc6,	// PID_H
	0x00, // bcdDevice rel. 2.00
	0x02,	// bcdDevice rel. 2.00
	0x01, // Index of Manufacturer String Descriptor
	0x02, // Index of Product String Descriptor
	0x03, // Index of Serial Number String Descriptor
	0x01, // Number of Possible Configurations
};

const unsigned char usb_configuration_descriptor[SIZE_CONFIGURATION_DESCRIPTOR] =
{ 
	0x09, // Size of Descriptor in Bytes
	USB_DESC_TYPE_CONFIGURATION, // Configuration Descriptor (0x02)
	LOBYTE(SIZE_CONFIGURATION_DESCRIPTOR), // Total length of config desc in bytes - LO
	HIBYTE(SIZE_CONFIGURATION_DESCRIPTOR), // Total length of config desc in bytes - HI
	0x01, // Number of Interfaces
	0x01, // Value to use as an argument to select this configuration
	0x00, // Index of String Descriptor describing this configuration
#if (USB_SELF_POWERED == 1)
	0xC0, // D7 Reserved, set to 1.(USB 1.0 Bus Powered),D6 Self Powered,D5 Remote Wakeup,D4..0 Reserved, set to 0.
#else
	0x80, // D7 Reserved, set to 1.(USB 1.0 Bus Powered),D6 Self Powered,D5 Remote Wakeup,D4..0 Reserved, set to 0.
#endif
	0x32, // Maximum Power Consumption in 2mA units

	// Interface descriptor
	0x09, // Size of Descriptor in Bytes (9 Bytes)
	USB_DESC_TYPE_INTERFACE, // Interface Descriptor (0x04)
	1, 		// Number of Interface
	0x00, // Value used to select alternative setting
	0x01, // Number of Endpoints used for this interface
	0xff, // Class Code
	0x00, // Subclass Code
	0x00, // Protocol Code - keyboard
	0x00, // Index of String Descriptor Describing this interface

	// Endpoint descriptor
	0x07, // Size of Descriptor in Bytes (7 Bytes)
	USB_DESC_TYPE_ENDPOINT, // Endpoint descriptor (0x05)
	0x81, // IN endpoint number 1 (0x81)
	0x03, // attrib: Interrupt endpoint
	0x08, // maximum packet size - LO
	0x00, // maximum packet size - HI
	0x0a, // poll interval (ms)
};

const unsigned char String_LangID[SIZE_STRING_LANGID] = {
	SIZE_STRING_LANGID,
	USB_DESC_TYPE_STRING,
	0x09, 0x04	//En-US
};

const unsigned char String_Vendor[SIZE_STRING_VENDOR] =
{
    SIZE_STRING_VENDOR, // Size of Vendor string
    USB_DESC_TYPE_STRING,  // bDescriptorType
		'T',0,'i',0,'l',0,'l',0,' ',0,'H',0,'a',0,'r',0,'b',0,'a',0,'u',0,'m',0
};

const uint8_t String_Product[SIZE_STRING_PRODUCT] =
{
    SIZE_STRING_PRODUCT,USB_DESC_TYPE_STRING,	// bLength,bDescriptorType
		'L',0,'C',0,'D',0,'2',0,'U',0,'S',0,'B',0,' ',0,
		'I',0,'n',0,'t',0,'e',0,'r',0,'f',0,'a',0,'c',0,'e',0
};

const unsigned char String_Serial[SIZE_STRING_SERIAL] =
{
    SIZE_STRING_SERIAL,           // bLength
    USB_DESC_TYPE_STRING,       // bDescriptorType
    '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '1', 0
};

const unsigned char* USB_String_Descriptors[LENGTH_STRING_DESCRIPTOR] = {
	String_LangID,
	String_Vendor,
	String_Product,
	String_Serial
};

const unsigned char USB_String_Descriptors_Length[LENGTH_STRING_DESCRIPTOR] = {
	SIZE_STRING_LANGID,
	SIZE_STRING_VENDOR,
	SIZE_STRING_PRODUCT,
	SIZE_STRING_SERIAL
};

