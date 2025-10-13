#ifndef BTSTACK_WRAPPER_H
#define BTSTACK_WRAPPER_H

// Rename BTstack's HID types to avoid conflict with TinyUSB
#define hid_report_type_t bt_hid_report_type_t
#define HID_REPORT_TYPE_INPUT BT_HID_REPORT_TYPE_INPUT
#define HID_REPORT_TYPE_OUTPUT BT_HID_REPORT_TYPE_OUTPUT
#define HID_REPORT_TYPE_FEATURE BT_HID_REPORT_TYPE_FEATURE

#include "btstack.h"

// Restore original names for BTstack usage
#undef hid_report_type_t
#undef HID_REPORT_TYPE_INPUT
#undef HID_REPORT_TYPE_OUTPUT
#undef HID_REPORT_TYPE_FEATURE

// Create typedef for BTstack's version
typedef bt_hid_report_type_t btstack_hid_report_type_t;

#endif