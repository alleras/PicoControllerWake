#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
// COMMON CONFIGURATION
//--------------------------------------------------------------------

// defined by compiler flags for flexibility
#ifndef CFG_TUSB_MCU
#define CFG_TUSB_MCU OPT_MCU_RP2350
#endif

#ifndef CFG_TUSB_OS
#define CFG_TUSB_OS OPT_OS_PICO
#endif

#ifndef CFG_TUSB_DEBUG
#define CFG_TUSB_DEBUG 0
#endif

// Enable device stack
#define CFG_TUD_ENABLED 1

// RHPort number used for device
#define BOARD_TUD_RHPORT 0

// RHPort max operational speed
#define BOARD_TUD_MAX_SPEED OPT_MODE_DEFAULT_SPEED

#define CFG_TUSB_RHPORT0_MODE (OPT_MODE_DEVICE)

//--------------------------------------------------------------------
// DEVICE CONFIGURATION
//--------------------------------------------------------------------

#ifndef CFG_TUD_ENDPOINT0_SIZE
#define CFG_TUD_ENDPOINT0_SIZE 64
#endif

//------------- CLASS -------------//
#define CFG_TUD_CDC 1  // Enable CDC (serial communication)
#define CFG_TUD_HID 1  // Enable HID (keyboard/mouse/gamepad)

// CDC FIFO size of TX and RX
#define CFG_TUD_CDC_RX_BUFSIZE 256
#define CFG_TUD_CDC_TX_BUFSIZE 256

// HID buffer size
#define CFG_TUD_HID_EP_BUFSIZE 16

#ifdef __cplusplus
}
#endif

#endif /* _TUSB_CONFIG_H_ */