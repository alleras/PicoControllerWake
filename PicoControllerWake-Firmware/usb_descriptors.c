#include "tusb.h"
#include <string.h>

// Device Descriptors
#define USBD_VID   0x2E8A  // Raspberry Pi
#define USBD_PID   0x000A  // Pico

tusb_desc_device_t const desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0200,
    .bDeviceClass       = 0x00,
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = 0x00,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor           = USBD_VID,
    .idProduct          = USBD_PID,
    .bcdDevice          = 0x0100,
    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,
    .bNumConfigurations = 0x01
};

uint8_t const * tud_descriptor_device_cb(void) {
    return (uint8_t const *) &desc_device;
}

// HID Report Descriptor
uint8_t const desc_hid_report[] = {
    TUD_HID_REPORT_DESC_KEYBOARD()
};

// Invoked when received GET HID REPORT DESCRIPTOR
uint8_t const * tud_hid_descriptor_report_cb(uint8_t instance) {
    (void) instance;
    return desc_hid_report;
}

// Configuration Descriptor
enum {
    ITF_NUM_CDC = 0,
    ITF_NUM_CDC_DATA,
    ITF_NUM_HID,
    ITF_NUM_TOTAL
};

#define CONFIG_TOTAL_LEN  (TUD_CONFIG_DESC_LEN + TUD_CDC_DESC_LEN + TUD_HID_DESC_LEN)

uint8_t const desc_configuration[] = {
    // Config number, interface count, string index, total length, attribute, power in mA
    TUD_CONFIG_DESCRIPTOR(1, ITF_NUM_TOTAL, 0, CONFIG_TOTAL_LEN, 0xA0, 100),
    
    // Interface number, string index, EP notification address and size, EP data address (out, in) and size
    TUD_CDC_DESCRIPTOR(ITF_NUM_CDC, 4, 0x81, 8, 0x02, 0x82, 64),
    
    // Interface number, string index, protocol, report descriptor len, EP In address, size & polling interval
    TUD_HID_DESCRIPTOR(ITF_NUM_HID, 0, HID_ITF_PROTOCOL_KEYBOARD, sizeof(desc_hid_report), 0x83, 16, 10)
};


uint8_t const * tud_descriptor_configuration_cb(uint8_t index) {
    (void) index;
    return desc_configuration;
}

// String Descriptors
char const* string_desc_arr[] = {
    (const char[]) { 0x09, 0x04 },  // 0: Language (English)
    "Raspberry Pi",                  // 1: Manufacturer
    "Pico Wake Bridge",              // 2: Product
    "123456",                        // 3: Serial Number
    "Pico CDC",                      // 4: CDC Interface
};

// Invoked when received GET STRING DESCRIPTOR request
// Converts normal C strings to USB string descriptor format (UTF-16LE with length header)
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
    (void) langid;
    static uint16_t _desc_str[32];
    
    uint8_t chr_count;
    
    if (index == 0) {
        memcpy(&_desc_str[1], string_desc_arr[0], 2);
        chr_count = 1;
    } else {
        if (index >= sizeof(string_desc_arr)/sizeof(string_desc_arr[0])) {
            return NULL;
        }
        
        const char* str = string_desc_arr[index];
        chr_count = strlen(str);
        if (chr_count > 31) chr_count = 31;
        
        for(uint8_t i = 0; i < chr_count; i++) {
            _desc_str[1 + i] = str[i];
        }
    }
    
    // First byte is length (including header), second byte is string type
    _desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * chr_count + 2);
    
    return _desc_str;
}

// Invoked when CDC interface received data from host
void tud_cdc_rx_cb(uint8_t itf)
{
    (void) itf;
}

// Invoked when a TX is complete and therefore space becomes available in TX buffer
void tud_cdc_tx_complete_cb(uint8_t itf)
{
    (void) itf;
}

// Invoked when line state DTR & RTS are changed via SET_CONTROL_LINE_STATE
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts)
{
    (void) itf;
    (void) dtr;
    (void) rts;
}

// Invoked when line coding is changed via SET_LINE_CODING
void tud_cdc_line_coding_cb(uint8_t itf, cdc_line_coding_t const* p_line_coding)
{
    (void) itf;
    (void) p_line_coding;
}

// Invoked when device is mounted (configured)
void tud_mount_cb(void)
{
    // Device enumerated successfully
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
    // Device unmounted
}