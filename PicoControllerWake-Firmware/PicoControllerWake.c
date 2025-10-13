#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "tusb.h"

#include "btstack_wrapper.h"

#include "cdc_output.h"
#include "whitelist.h"
#include "bt_scanner.h"
#include "util.h"
#include "pc_waker.h"
#include "flash_storage.h"

#define HARDCODED_CONTROLLER "98:7A:14:BC:99:CE"

static btstack_packet_callback_registration_t hci_event_callback_registration;

static Whitelist* whitelist = NULL;
static BTScanner* btscanner = NULL;
static PCWaker pc_waker;

void onDeviceDetected(const uint8_t* addr, int8_t rssi, bool is_ble, void* user_data) { 
    tud_task();  
    pc_waker_wake(&pc_waker);
    tud_task();
}

void process_incoming_cdc(char cmd_buffer[128], size_t buffer_size){
    // Read commands and communicate to units that need it
    if (!tud_cdc_available())
        return;
    
    uint32_t count = tud_cdc_read(cmd_buffer, buffer_size - 1);
    if (count > 0) {
        cmd_buffer[count] = '\0';
        
        // Strip any trailing newlines/carriage returns
        while (count > 0 && (cmd_buffer[count-1] == '\n' || cmd_buffer[count-1] == '\r')) {
            cmd_buffer[--count] = '\0';
        }
        
        if (count > 0) {  // Only process if there's actual content
            cdc_print_debug("Processing command: '%s'\n", cmd_buffer);
            whitelist_process_command(whitelist, cmd_buffer);
            cdc_print_success("EOF\n");
        }
    }
}

int main(void) {    
    stdio_init_all();
    cyw43_arch_init();
    
    cyw43_arch_enable_sta_mode();
    tusb_init();

    storage_init();
    pc_waker_init(&pc_waker);

    whitelist = whitelist_create(true);
    btscanner = bt_scanner_create(whitelist);

    if (btscanner == NULL) {
        cdc_print_debug("ERROR: Failed to create BT scanner\n");
        whitelist_destroy(whitelist);
        return -1;
    }
    
    bt_scanner_set_detection_callback(btscanner, onDeviceDetected, NULL);
    if (!bt_scanner_init(btscanner)) {
        cdc_print_debug("ERROR: Failed to initialize Bluetooth\n");
    }

    repeat_blink(5, 200);
    
    uint32_t last_blink = 0;
    uint32_t last_key_press = 0;

    static char cmd_buffer[128];

    while (true) {
        tud_task();
        cyw43_arch_poll();
        pc_waker_task(&pc_waker);

        if(!bt_scanner_is_scanning(btscanner)){
            bt_scanner_start_scanning(btscanner);
        }

        bt_scanner_process(btscanner); // For completeness, does nothing right now
        process_incoming_cdc(cmd_buffer, sizeof(cmd_buffer));

        process_blink(1000);
        sleep_ms(1);
    }
    
    return 0;
}