#ifndef WHITELIST_H
#define WHITELIST_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Opaque pointer to Whitelist
typedef struct Whitelist Whitelist;

// Create and destroy whitelist
Whitelist* whitelist_create(bool load_on_init);
void whitelist_destroy(Whitelist* wl);

// Check if a MAC address is whitelisted
bool whitelist_is_whitelisted(Whitelist* wl, const uint8_t* addr);

// Add a MAC address to the whitelist
bool whitelist_add_device(Whitelist* wl, const uint8_t* addr);
bool whitelist_add_device_str(Whitelist* wl, const char* mac_str);

// Remove a MAC address from the whitelist
bool whitelist_remove_device(Whitelist* wl, const uint8_t* addr);
bool whitelist_remove_device_str(Whitelist* wl, const char* mac_str);

// Clear all entries
void whitelist_clear(Whitelist* wl);

// Load from flash storage
void whitelist_load_from_flash(Whitelist* wl);

// Save to flash storage
void whitelist_save_to_flash(Whitelist* wl);

// Process command from PC (e.g., "bluetooth-whitelist -add AA:BB:CC:DD:EE:FF")
void whitelist_process_command(Whitelist* wl, const char* cmd);

// Get count of whitelisted devices
size_t whitelist_get_count(const Whitelist* wl);

#ifdef __cplusplus
}
#endif

#endif // WHITELIST_H