#ifndef BTSTACK_CONFIG_H
#define BTSTACK_CONFIG_H

// Port related features
#define HAVE_EMBEDDED_TIME_MS

// BTstack features - enable both BLE and Classic for scanning
//#define ENABLE_BLE // already defined
//#define ENABLE_CLASSIC // already defined
#define ENABLE_LE_PERIPHERAL
#define ENABLE_LE_CENTRAL
#define ENABLE_CLASSIC_PAIRING
#define ENABLE_L2CAP_ENHANCED_RETRANSMISSION_MODE

#define CYW43_ENABLE_BLUETOOTH 1

// Logging
#define ENABLE_LOG_INFO
#define ENABLE_LOG_ERROR
#define ENABLE_PRINTF_HEXDUMP

// BTstack configuration - buffers and limits
#define HCI_OUTGOING_PRE_BUFFER_SIZE 4
#define HCI_ACL_PAYLOAD_SIZE (255 + 4)
#define HCI_ACL_CHUNK_SIZE_ALIGNMENT 4
#define MAX_NR_HCI_CONNECTIONS 1
#define MAX_NR_GATT_CLIENTS 1        // For BLE GATT operations
#define MAX_NR_SM_LOOKUP_ENTRIES 3
#define MAX_NR_WHITELIST_ENTRIES 16
#define MAX_NR_LE_DEVICE_DB_ENTRIES 16

// Limit buffers to avoid CYW43 shared bus overrun
#define MAX_NR_CONTROLLER_ACL_BUFFERS 3
#define MAX_NR_CONTROLLER_SCO_PACKETS 3

// Flow control for CYW43 shared bus
#define ENABLE_HCI_CONTROLLER_TO_HOST_FLOW_CONTROL
#define HCI_HOST_ACL_PACKET_LEN (255 + 4)
#define HCI_HOST_ACL_PACKET_NUM 3
#define HCI_HOST_SCO_PACKET_LEN 120
#define HCI_HOST_SCO_PACKET_NUM 3

// Flash storage for device database
#define NVM_NUM_DEVICE_DB_ENTRIES 16
#define NVM_NUM_LINK_KEYS 16

// Fixed-size ATT database (no malloc)
#define MAX_ATT_DB_SIZE 512

// Security features
#define HAVE_ASSERT
#define HCI_RESET_RESEND_TIMEOUT_MS 1000
#define ENABLE_SOFTWARE_AES128
#define ENABLE_MICRO_ECC_FOR_LE_SECURE_CONNECTIONS

#endif // BTSTACK_CONFIG_H