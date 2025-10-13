#include "whitelist.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "cdc_output.h"
#include "flash_storage.h"

#include "error_codes.h"

#define MAX_DEVICES 100

typedef struct {
    uint8_t addr[6];
} Device;

struct Whitelist {
    Device* devices;
    size_t count;
};

// Forward declarations
static bool parse_mac_address(const char* mac_str, uint8_t* addr);
static bool device_equals(const Device* a, const Device* b);
static int find_device(Whitelist* wl, const Device* dev);

Whitelist* whitelist_create(bool load_on_init) {
    Whitelist* wl = (Whitelist*)malloc(sizeof(Whitelist));
    if (wl == NULL) {
        return NULL;
    }
    
    wl->count = 0;
    wl->devices = (Device*)malloc(sizeof(Device) * MAX_DEVICES);
    
    if (wl->devices == NULL) {
        free(wl);
        return NULL;
    }
    
    if (load_on_init) {
        whitelist_load_from_flash(wl);
    }
    
    return wl;
}

void whitelist_destroy(Whitelist* wl) {
    if (wl == NULL) return;
    
    if (wl->devices != NULL) {
        free(wl->devices);
    }
    free(wl);
}

bool whitelist_is_whitelisted(Whitelist* wl, const uint8_t* addr) {
    if (wl == NULL || addr == NULL) return false;
    
    Device dev;
    memcpy(dev.addr, addr, 6);
    
    return find_device(wl, &dev) >= 0;
}

bool whitelist_add_device(Whitelist* wl, const uint8_t* addr) {
    if (wl == NULL || addr == NULL) return false;
    
    Device dev;
    memcpy(dev.addr, addr, 6);
    
    // Check if already exists
    if (find_device(wl, &dev) >= 0) {
        cdc_print_error(ERR_DEVICE_ALREADY_EXISTS, "Cannot add device. Already exists!\n");
        return false;
    }
    
    if (wl->count >= MAX_DEVICES) {
        cdc_print_error(ERR_WHITELIST_FULL, "Whitelist is full (max %d devices)\n", MAX_DEVICES);
        return false;
    }
    
    // Add device
    memcpy(&wl->devices[wl->count], &dev, sizeof(Device));
    wl->count++;
    
    whitelist_save_to_flash(wl);

    return true;
}

bool whitelist_add_device_str(Whitelist* wl, const char* mac_str) {
    if (wl == NULL || mac_str == NULL) return false;
    
    uint8_t addr[6];
    if (!parse_mac_address(mac_str, addr)){
        cdc_print_error(ERR_INVALID_MAC, "Cannot parse MAC: %s\n", mac_str);
        return false;
    }

    return whitelist_add_device(wl, addr);
}

bool whitelist_remove_device(Whitelist* wl, const uint8_t* addr) {
    if (wl == NULL || addr == NULL) return false;
    
    Device dev;
    memcpy(dev.addr, addr, 6);
    
    int index = find_device(wl, &dev);
    if (index == -1) {
        cdc_print_error(ERR_DEVICE_NOT_FOUND, "Device not found, could not remove\n");
        return false;
    }

    // Shift remaining devices down
    for (size_t i = index; i < wl->count - 1; i++) {
        memcpy(&wl->devices[i], &wl->devices[i + 1], sizeof(Device));
    }
    wl->count--;
    whitelist_save_to_flash(wl);

    return true;
}

bool whitelist_remove_device_str(Whitelist* wl, const char* mac_str) {
    if (wl == NULL || mac_str == NULL) return false;
    
    uint8_t addr[6];
    if (!parse_mac_address(mac_str, addr)){
        cdc_print_error(ERR_INVALID_MAC, "Cannot parse MAC: %s\n", mac_str);
        return false;
    }

    return whitelist_remove_device(wl, addr);
}

void whitelist_clear(Whitelist* wl) {
    if (wl == NULL) return;
    
    wl->count = 0;
    whitelist_save_to_flash(wl);
}

size_t whitelist_get_count(const Whitelist* wl) {
    if (wl == NULL) return 0;
    return wl->count;
}

static bool parse_mac_address(const char* mac_str, uint8_t* addr) {
    if (mac_str == NULL || addr == NULL) return false;
    
    // Parse "AA:BB:CC:DD:EE:FF" format
    unsigned int bytes[6];
    int result = sscanf(mac_str, "%02x:%02x:%02x:%02x:%02x:%02x",
                       &bytes[0], &bytes[1], &bytes[2],
                       &bytes[3], &bytes[4], &bytes[5]);
    
    if (result != 6) {
        return false;
    }
    
    for (int i = 0; i < 6; i++) {
        addr[i] = (uint8_t)bytes[i];
    }
    
    return true;
}

static bool device_equals(const Device* a, const Device* b) {
    return memcmp(a->addr, b->addr, 6) == 0;
}

static int find_device(Whitelist* wl, const Device* dev) {
    for (size_t i = 0; i < wl->count; i++) {
        if (device_equals(&wl->devices[i], dev)) {
            return (int)i;
        }
    }
    return -1;
}

void whitelist_load_from_flash(Whitelist* wl) {
    if (wl == NULL) return;

    size_t buffer_size = MAX_DEVICES * sizeof(Device);
    size_t actual_size;
    
    if (storage_load(STORAGE_WHITELIST, wl->devices, buffer_size, &actual_size)) {
        wl->count = actual_size / sizeof(Device);
    }
}

void whitelist_save_to_flash(Whitelist* wl) {
    if (wl == NULL) return;

    size_t data_size = wl->count * sizeof(Device);
    storage_save(STORAGE_WHITELIST, wl->devices, data_size);
}

void whitelist_process_command(Whitelist* wl, const char* cmd) {
    if (wl == NULL || cmd == NULL) return;
    
    if (strncmp(cmd, "bluetooth-whitelist", 19) != 0) {
        return;
    }
    
    // Find the flag (skip "bluetooth-whitelist ")
    const char* flag = cmd + 19;
    while (*flag == ' ') flag++; // Skip spaces
    
    if (*flag != '-') {
        cdc_print_error(ERR_MISSING_FLAG, "Missing flag. Use 'bluetooth-whitelist -help' for usage\n");
        return;
    }
    
    if (strncmp(flag, "--add ", 5) == 0) {
        const char* mac = flag + 5;
        if (whitelist_add_device_str(wl, mac)){
            cdc_print_success("%s\n", mac);
        }
    }
    else if (strncmp(flag, "--remove ", 8) == 0) {
        const char* mac = flag + 8;
        if(whitelist_remove_device_str(wl, mac)){
            cdc_print_success("%s\n", mac);
        }
    }
    else if (strcmp(flag, "--clear") == 0) {
        whitelist_clear(wl);
        cdc_print_success("N\\A\n");
    }
    else if (strcmp(flag, "--list") == 0) {
        cdc_print_success("ARRAY COUNT=%zu\n", wl->count);
        for (size_t i = 0; i < wl->count; i++) {
            cdc_printf("%02X:%02X:%02X:%02X:%02X:%02X\n",
                   wl->devices[i].addr[0], wl->devices[i].addr[1], wl->devices[i].addr[2],
                   wl->devices[i].addr[3], wl->devices[i].addr[4], wl->devices[i].addr[5]);
        }
    }
    else if (strcmp(flag, "-help") == 0) {
        cdc_print_success("Bluetooth Whitelist Commands:\n");
        cdc_printf("bluetooth-whitelist -add <MAC>     Add device to whitelist\n");
        cdc_printf("bluetooth-whitelist -remove <MAC>  Remove device from whitelist\n");
        cdc_printf("bluetooth-whitelist -list          Show all whitelisted devices\n");
        cdc_printf("bluetooth-whitelist -clear         Clear all whitelisted devices\n");
    }
    else {
        cdc_print_error(ERR_UNKNOWN_COMMAND, "Unknown command: %s\n", flag);
        cdc_printf("Use 'bluetooth-whitelist -help' for usage\n");
    }
}