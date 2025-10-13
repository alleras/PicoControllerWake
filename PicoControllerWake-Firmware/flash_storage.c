#include "flash_storage.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include "pico/stdlib.h"
#include <string.h>

#define STORAGE_MAGIC 0xDEADBEEF

// Each storage key gets one sector, starting from the end of flash
#define STORAGE_BASE_OFFSET (PICO_FLASH_SIZE_BYTES - (STORAGE_MAX * FLASH_SECTOR_SIZE))

static inline uint32_t get_storage_offset(StorageKey key) {
    return STORAGE_BASE_OFFSET + (key * FLASH_SECTOR_SIZE);
}

static inline const uint8_t* get_storage_ptr(StorageKey key) {
    return (const uint8_t*)(XIP_BASE + get_storage_offset(key));
}

void storage_init(void) {
    // Reserved
}

bool storage_save(StorageKey key, const void* data, size_t size) {
    if (key >= STORAGE_MAX || data == NULL || size == 0) {
        return false;
    }
    
    // Data must fit in one sector 
    // We subtract 8 to account for the magic number, and the data size
    if (size > FLASH_SECTOR_SIZE - 8) {
        return false;
    }
    
    // Prepare buffer
    uint8_t buffer[FLASH_SECTOR_SIZE];
    memset(buffer, 0xFF, sizeof(buffer));
    
    // Write magic number
    uint32_t magic = STORAGE_MAGIC;
    memcpy(buffer, &magic, sizeof(uint32_t));
    
    // Write data size
    uint32_t data_size = (uint32_t)size;
    memcpy(buffer + sizeof(uint32_t), &data_size, sizeof(uint32_t));
    
    // Write actual data
    memcpy(buffer + 8, data, size);
    
    // Write to flash
    uint32_t offset = get_storage_offset(key);
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(offset, FLASH_SECTOR_SIZE);
    flash_range_program(offset, buffer, FLASH_SECTOR_SIZE);
    restore_interrupts(ints);
    
    return true;
}

bool storage_load(StorageKey key, void* buffer, size_t buffer_size, size_t* actual_size) {
    if (key >= STORAGE_MAX || buffer == NULL || buffer_size == 0) {
        return false;
    }
    
    const uint8_t* flash_data = get_storage_ptr(key);
    
    // Check magic number
    uint32_t magic;
    memcpy(&magic, flash_data, sizeof(uint32_t));
    if (magic != STORAGE_MAGIC) {
        // No valid data
        return false;
    }
    
    // Read data size
    uint32_t data_size;
    memcpy(&data_size, flash_data + sizeof(uint32_t), sizeof(uint32_t));
    
    // Sanity check
    if (data_size > FLASH_SECTOR_SIZE - 8) {
        return false;
    }
    
    // Check if buffer is large enough
    if (data_size > buffer_size) {
        return false;
    }
    
    // Copy data to buffer
    memcpy(buffer, flash_data + 8, data_size);
    
    if (actual_size != NULL) {
        *actual_size = data_size;
    }
    
    return true;
}

bool storage_clear(StorageKey key) {
    if (key >= STORAGE_MAX) {
        return false;
    }
    
    uint8_t buffer[FLASH_SECTOR_SIZE];
    memset(buffer, 0xFF, sizeof(buffer));
    
    uint32_t offset = get_storage_offset(key);
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(offset, FLASH_SECTOR_SIZE);
    restore_interrupts(ints);
    
    return true;
}

bool storage_exists(StorageKey key) {
    if (key >= STORAGE_MAX) {
        return false;
    }
    
    const uint8_t* flash_data = get_storage_ptr(key);
    
    // Check magic number, if it's there then it's valid data and the storage exists
    uint32_t magic;
    memcpy(&magic, flash_data, sizeof(uint32_t));
    
    return (magic == STORAGE_MAGIC);
}