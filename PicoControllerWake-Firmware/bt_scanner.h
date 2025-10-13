#ifndef BT_SCANNER_H
#define BT_SCANNER_H

#include <stdint.h>
#include <stdbool.h>
#include "whitelist.h"
#include "btstack_defines.h"

// Forward declaration
typedef struct BTScanner BTScanner;

// Function pointer type for detection callback
typedef void (*DetectionCallback)(const uint8_t* addr, int8_t rssi, bool is_ble, void* user_data);

// Create and destroy scanner
BTScanner* bt_scanner_create(Whitelist* whitelist);
void bt_scanner_destroy(BTScanner* scanner);

// Initialize scanner
bool bt_scanner_init(BTScanner* scanner);

void bt_scanner_start_scanning(BTScanner* scanner);
void bt_scanner_stop_scanning(BTScanner* scanner);
bool bt_scanner_is_scanning(const BTScanner* scanner);
void bt_scanner_set_detection_callback(BTScanner* scanner, DetectionCallback cb, void* user_data);

// Process Bluetooth events (call during main loop)
void bt_scanner_process(BTScanner* scanner);

#endif // BT_SCANNER_H