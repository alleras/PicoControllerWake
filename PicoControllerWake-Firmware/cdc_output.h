#ifndef CDC_OUTPUT_H
#define CDC_OUTPUT_H

#include <stdarg.h>
#include "error_codes.h"

void cdc_printf(const char* fmt, ...);
void cdc_print_success(const char* fmt, ...);
void cdc_print_error(ErrorCode error_code, const char* fmt, ...);
void cdc_print_debug(const char* fmt, ...);

#endif