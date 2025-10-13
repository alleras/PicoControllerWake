#include "cdc_output.h"
#include "tusb.h"
#include "error_codes.h"
#include <stdarg.h>

// Helper function for prefixed printf
static void cdc_printf_with_prefix(const char* prefix, const char* fmt, va_list args) {
    char buffer[256];
    
    // Write prefix
    int offset = snprintf(buffer, sizeof(buffer), "%s", prefix);
    
    // Append formatted message
    int len = vsnprintf(buffer + offset, sizeof(buffer) - offset, fmt, args);
    
    // Send if valid
    if (len > 0 && offset + len < (int)sizeof(buffer) && tud_cdc_connected()) {
        tud_cdc_write(buffer, (uint32_t)(offset + len));
        tud_cdc_write_flush();
    }
}

void cdc_printf(const char* fmt, ...) {
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    
    if (len > 0 && len < (int)sizeof(buffer) && tud_cdc_connected()) {
        tud_cdc_write(buffer, (uint32_t)len);
        tud_cdc_write_flush();
    }
}

void cdc_print_success(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    cdc_printf_with_prefix("OK|", fmt, args);
    va_end(args);
}

void cdc_print_error(ErrorCode error_code, const char* fmt, ...) {
    char prefix[64];
    snprintf(prefix, sizeof(prefix), "ERR|%s|", error_code_to_string(error_code));
    
    va_list args;
    va_start(args, fmt);
    cdc_printf_with_prefix(prefix, fmt, args);
    va_end(args);
}

void cdc_print_debug(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    cdc_printf_with_prefix("DEBUG|", fmt, args);
    va_end(args);
}