#ifndef FLASH_STORAGE_H
#define FLASH_STORAGE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Storage keys - each gets its own sector
typedef enum {
    STORAGE_WHITELIST = 0,
    /* Storage 1 - 9 reserved */
    STORAGE_MAX = 10
} StorageKey;

/**
 * Initialize the flash storage system
 */
void storage_init(void);

/**
 * Save data to flash storage
 * @param key Storage key identifying which sector to use
 * @param data Pointer to data to save
 * @param size Size of data in bytes
 * @return true if successful, false otherwise
 */
bool storage_save(StorageKey key, const void* data, size_t size);

/**
 * Load data from flash storage
 * @param key Storage key identifying which sector to read
 * @param buffer Buffer to load data into
 * @param buffer_size Size of buffer
 * @param actual_size Optional: returns actual size of data loaded
 * @return true if successful, false otherwise
 */
bool storage_load(StorageKey key, void* buffer, size_t buffer_size, size_t* actual_size);

/**
 * Clear/erase data for a specific storage key
 * @param key Storage key to clear
 * @return true if successful, false otherwise
 */
bool storage_clear(StorageKey key);

/**
 * Check if valid data exists for a storage key
 * @param key Storage key to check
 * @return true if valid data exists, false otherwise
 */
bool storage_exists(StorageKey key);

#endif // FLASH_STORAGE_H