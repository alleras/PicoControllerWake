#include "bt_scanner.h"

#include "btstack.h"

#include <pico/cyw43_driver.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include <stdlib.h>
#include "cdc_output.h"

struct BTScanner {
    Whitelist* whitelist;
    DetectionCallback detection_callback;
    void* callback_user_data;
    bool scanning;
};

static BTScanner* instance = NULL;
static btstack_packet_callback_registration_t hci_event_callback_registration;

// Forward declarations of internal functions
static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size);
static void handle_inquiry_result(BTScanner* scanner, uint8_t *packet);
static void handle_advertisement(BTScanner* scanner, uint8_t *packet);

BTScanner* bt_scanner_create(Whitelist* wl) {
    BTScanner* scanner = (BTScanner*)malloc(sizeof(BTScanner));
    if (scanner == NULL) {
        return NULL;
    }
    
    scanner->whitelist = wl;
    scanner->detection_callback = NULL;
    scanner->callback_user_data = NULL;
    scanner->scanning = false;
    
    instance = scanner;
    
    return scanner;
}

void bt_scanner_destroy(BTScanner* scanner) {
    if (scanner == NULL) return;
    
    if (scanner->scanning) {
        bt_scanner_stop_scanning(scanner);
    }
    
    if (instance == scanner) {
        instance = NULL;
    }
    
    free(scanner);
}

bool bt_scanner_init(BTScanner* scanner) {
    if (scanner == NULL) return false;
    
    hci_power_control(HCI_POWER_ON);

    cdc_print_debug("BT Scanner initialized\n");
    return true;
}

void bt_scanner_start_scanning(BTScanner* scanner) {
    if (scanner == NULL) return;
    if (scanner->scanning) return;
    
    cdc_print_debug("Starting Bluetooth scanning...\n");
    
    // Register HCI packet handler
    hci_event_callback_registration.callback = &packet_handler; 
    hci_add_event_handler(&hci_event_callback_registration);
    
    cdc_print_debug("Callbacks registered...\n");

    // Start BLE scanning
    gap_set_scan_parameters(1, 0x0030, 0x0030);  // Active scanning, 30ms interval/window
    gap_start_scan();
    
    // Start Classic Bluetooth inquiry
    //gap_inquiry_start(5);  // 5 second inquiry
    
    scanner->scanning = true;
    cdc_print_debug("Scanning started\n");
}

void bt_scanner_stop_scanning(BTScanner* scanner) {
    if (scanner == NULL) return;
    if (!scanner->scanning) return;
    
    gap_stop_scan();
    scanner->scanning = false;
    cdc_print_debug("Scanning stopped\n");
}

bool bt_scanner_is_scanning(const BTScanner* scanner) {
    if (scanner == NULL) return false;
    return scanner->scanning;
}

void bt_scanner_set_detection_callback(BTScanner* scanner, DetectionCallback cb, void* user_data) {
    if (scanner == NULL) return;
    
    scanner->detection_callback = cb;
    scanner->callback_user_data = user_data;
}

void bt_scanner_process(BTScanner* scanner) {
    (void)scanner; // Suppress unused parameter warning
}

static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size) {
    (void)channel; // Suppress unused parameter warning
    (void)size;    // Suppress unused parameter warning
    
    if (packet_type != HCI_EVENT_PACKET) return;
    if (instance == NULL) return;
    
    uint8_t event = hci_event_packet_get_type(packet);
    
    switch (event) {
        case GAP_EVENT_INQUIRY_RESULT:
            handle_inquiry_result(instance, packet);
            break;
            
        case GAP_EVENT_ADVERTISING_REPORT:
            handle_advertisement(instance, packet);
            break;
            
        case GAP_EVENT_INQUIRY_COMPLETE:
            cdc_print_debug("Classic inquiry complete, restarting...\n");
            //gap_inquiry_start(5);
            break;
            
        default:
            break;
    }
}

static void handle_inquiry_result(BTScanner* scanner, uint8_t *packet) {
    bd_addr_t addr;
    gap_event_inquiry_result_get_bd_addr(packet, addr);
    int8_t rssi = gap_event_inquiry_result_get_rssi(packet);

    // If no whitelist, report everything
    // If whitelist exists, check if device is whitelisted
    bool should_report = (scanner->whitelist == NULL) || 
                         whitelist_is_whitelisted(scanner->whitelist, addr);
    
    if (should_report) {
        cdc_print_debug("[Classic] Found device: %02X:%02X:%02X:%02X:%02X:%02X RSSI: %d dBm\n",
               addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], rssi);
        
        if (scanner->detection_callback) {
            scanner->detection_callback(addr, rssi, false, scanner->callback_user_data);
        }
    }
}

static void handle_advertisement(BTScanner* scanner, uint8_t *packet) {
    bd_addr_t addr;
    gap_event_advertising_report_get_address(packet, addr);
    uint8_t adv_event_type = gap_event_advertising_report_get_advertising_event_type(packet);
    int8_t rssi = gap_event_advertising_report_get_rssi(packet);
    
    // Check if it's a connectable advertisement (ADV_IND or ADV_DIRECT_IND)
    if (adv_event_type == 0 || adv_event_type == 1) {
        // If no whitelist, report everything
        // If whitelist exists, check if device is whitelisted
        bool should_report = (scanner->whitelist == NULL) || 
                             whitelist_is_whitelisted(scanner->whitelist, addr);
        
        if (should_report) {
            cdc_print_debug("[BLE] Found device: %02X:%02X:%02X:%02X:%02X:%02X RSSI: %d dBm (ADV type: %d)\n",
                   addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], rssi, adv_event_type);
            
            if (scanner->detection_callback) {
                scanner->detection_callback(addr, rssi, true, scanner->callback_user_data);
            }
        }
    }
}