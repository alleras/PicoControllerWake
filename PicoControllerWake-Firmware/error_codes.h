#ifndef ERROR_CODES_H
#define ERROR_CODES_H

typedef enum {
    ERR_SUCCESS = 0,
    ERR_MISSING_FLAG,
    ERR_UNKNOWN_COMMAND,
    ERR_INVALID_MAC,
    ERR_DEVICE_NOT_FOUND,
    ERR_DEVICE_ALREADY_EXISTS,
    ERR_WHITELIST_FULL,
    ERR_PARSE_FAILED,
    // Add more as needed
} ErrorCode;

const char* error_code_to_string(ErrorCode code);

#endif