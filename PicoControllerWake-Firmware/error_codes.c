#include "error_codes.h"

// Lookup table for error code strings
static const char* error_code_strings[] = {
    [ERR_SUCCESS] = "SUCCESS",
    [ERR_MISSING_FLAG] = "MISSING_FLAG",
    [ERR_UNKNOWN_COMMAND] = "UNKNOWN_COMMAND",
    [ERR_INVALID_MAC] = "INVALID_MAC",
    [ERR_DEVICE_NOT_FOUND] = "DEVICE_NOT_FOUND",
    [ERR_DEVICE_ALREADY_EXISTS] = "DEVICE_ALREADY_EXISTS",
    [ERR_WHITELIST_FULL] = "WHITELIST_FULL",
    [ERR_PARSE_FAILED] = "PARSE_FAILED",
};

const char* error_code_to_string(ErrorCode code) {
    if (code < 0 || code >= sizeof(error_code_strings) / sizeof(error_code_strings[0])) {
        return "UNKNOWN_ERROR";
    }
    return error_code_strings[code];
}