#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include <stdint.h>

#define PROTOCOL_START_BYTE 0xAA
#define PROTOCOL_VERSION    0x01
#define PAYLOAD_MAX_SIZE    128

/* Primitives */

typedef struct __attribute__((packed)) {
    float x;
    float y;
    float z;
} VECTOR;

typedef struct __attribute__((packed)) {
    float w;
    float i;
    float j;
    float k;
} QUATERNION;

typedef struct __attribute__((packed)) {
    int32_t latitude;
    int32_t longitude;
    float meters_above_sealvl;
    float meters_above_ground;
    int8_t fix_type;
} GPS;

typedef struct __attribute__((packed)) {
    uint32_t timestamp;
    VECTOR acceleration;
    VECTOR gyro;
    VECTOR magnitude;
} IMU;

/* Enums */

typedef enum : uint8_t {
    FLIGHT_DISARMED,
    FLIGHT_ARMED,
} FLIGHT_STATE;

typedef enum : uint8_t { 
    FLIGHT_MANUAL,
    FLIGHT_ACROBATIC,
    FLIGHT_AUTONOMOUS,
    FLIGHT_WAYPOINT,
    FLIGHT_MISSION,
    FLIGHT_RETURN_TO_LAUNCH,
    FLIGHT_LAND,
} FLIGHT_MODE;

typedef enum : uint8_t {
    BOOTLOADER_NONE,
    BOOTLOADER_STATS,
    BOOTLOADER_ERASE_APP,
    BOOTLOADER_UPDATE,
    BOOTLOADER_VERIFY,
} BOOTLOADER_CMD;

typedef enum : uint8_t {
    ERR_NONE,
    ERR_CRC_FAIL,
    ERR_UNKNOWN_MSG,
    ERR_WRONG_VERSION,
    ERR_PAYLOAD_OVERSIZE,
    ERR_INVALID_STATE,
    ERR_BUFFER_FULL,
    ERR_AUTH_FAIL,
    ERR_FLASH_FAIL,
    ERR_SENSOR_FAIL,
    ERR_TIMEOUT,
} ERROR_CODE;

/* Message IDs  (flattened) */

typedef enum : uint8_t {
    MSG_HEARTBEAT,
    MSG_ACK,
    MSG_NACK,
    MSG_SYS_STATUS,
    MSG_RC_CHANNELS,
    MSG_FLIGHT_STATE,
    MSG_FLIGHT_MODE,
    MSG_BOOTLOADER_CMD,
    MSG_BOOTLOADER_DATA,
    MSG_TELEM_IMU,
    MSG_TELEM_GPS,
    MSG_TELEM_BAROMETER,
    MSG_TELEM_POWER,
    MSG_LOG_STRING,
    MSG_LOG_VALUE,
} MSG_ID;

/* Payloads  (one per MSG_ID) */

typedef struct __attribute__((packed)) { 
    uint32_t timestamp; 
    FLIGHT_STATE state; 
    FLIGHT_MODE mode; 
    uint16_t error_flags; 
} HEARTBEAT_PAYLOAD;

typedef struct __attribute__((packed)) { 
    uint8_t acked_seq; 
    ERROR_CODE error; 
} ACK_PAYLOAD;

typedef struct __attribute__((packed)) { 
    uint8_t nacked_seq; 
    ERROR_CODE error; 
} NACK_PAYLOAD;

typedef struct __attribute__((packed)) { 
    uint32_t timestamp; 
    int16_t channels[16]; 
    uint16_t valid_mask; 
} RC_CHANNELS_PAYLOAD;

typedef struct __attribute__((packed)) { 
    FLIGHT_STATE requested_state; 
} FLIGHT_STATE_PAYLOAD;

typedef struct __attribute__((packed)) { 
    FLIGHT_MODE requested_mode; 
} FLIGHT_MODE_PAYLOAD;

typedef struct __attribute__((packed)) { 
    BOOTLOADER_CMD cmd; 
    uint32_t addr; 
    uint16_t len; 
    uint8_t nonce[8]; 
} BOOTLOADER_CMD_PAYLOAD;

typedef struct __attribute__((packed)) { 
    uint32_t addr; 
    uint8_t len; 
    uint8_t data[64]; 
} BOOTLOADER_DATA_PAYLOAD;

typedef struct __attribute__((packed)) { 
    IMU imu; 
} TELEM_IMU_PAYLOAD;

typedef struct __attribute__((packed)) { 
    GPS gps; 
} TELEM_GPS_PAYLOAD;

typedef struct __attribute__((packed)) {
    float pressure_pa; 
    float temp_c; 
    float alt_m; 
} TELEM_BAROMETER_PAYLOAD;

typedef struct __attribute__((packed)) { 
    float voltage; 
    float current; 
    float consumed_mah; 
    uint8_t percent; 
} TELEM_POWER_PAYLOAD;

typedef struct __attribute__((packed)) { 
    uint8_t level; 
    char text[120]; 
} LOG_STRING_PAYLOAD;

typedef struct __attribute__((packed)) { 
    uint8_t key_id; 
    float value; 
    uint32_t timestamp; 
} LOG_VALUE_PAYLOAD;

/* Frame */

typedef struct __attribute__((packed)) {
    uint8_t start_byte;
    uint8_t version;
    MSG_ID message_id;
    uint8_t sequence;
    uint8_t payload_len;
    uint8_t payload[PAYLOAD_MAX_SIZE];
    uint16_t CRC;
} FRAME;

static const uint8_t MSG_PAYLOAD_SIZE[] = {
    [MSG_HEARTBEAT]        = sizeof(HEARTBEAT_PAYLOAD),
    [MSG_ACK]              = sizeof(ACK_PAYLOAD),
    [MSG_NACK]             = sizeof(NACK_PAYLOAD),
    [MSG_RC_CHANNELS]      = sizeof(RC_CHANNELS_PAYLOAD),
    [MSG_FLIGHT_STATE]     = sizeof(FLIGHT_STATE_PAYLOAD),
    [MSG_FLIGHT_MODE]      = sizeof(FLIGHT_MODE_PAYLOAD),
    [MSG_BOOTLOADER_CMD]   = sizeof(BOOTLOADER_CMD_PAYLOAD),
    [MSG_BOOTLOADER_DATA]  = sizeof(BOOTLOADER_DATA_PAYLOAD),
    [MSG_TELEM_IMU]        = sizeof(TELEM_IMU_PAYLOAD),
    [MSG_TELEM_GPS]        = sizeof(TELEM_GPS_PAYLOAD),
    [MSG_TELEM_BAROMETER]  = sizeof(TELEM_BAROMETER_PAYLOAD),
    [MSG_TELEM_POWER]      = sizeof(TELEM_POWER_PAYLOAD),
    [MSG_LOG_STRING]       = sizeof(LOG_STRING_PAYLOAD),
    [MSG_LOG_VALUE]        = sizeof(LOG_VALUE_PAYLOAD),
};

#endif