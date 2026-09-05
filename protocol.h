#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include <stdint.h>

#define PROTOCOL_START_BYTE 0xAA
#define PROTOCOL_VERSION    0x06
#define PAYLOAD_MAX_SIZE    128
#define PAYLOAD_TEXT_SIZE   120

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
    float altitude_meters_abv_sealvl;
    float altitude_meters_abv_ground;
    int8_t fix_type;
} GPS;

typedef struct __attribute__((packed)) {
    uint32_t timestamp;
    VECTOR acceleration;
    VECTOR gyro;
    VECTOR magnitude;
} IMU;

typedef struct __attribute__((packed)) {
    float pressure_pascal;
    float temperature_celsius;
    float altitude_meters;
} BAROMETER;

typedef struct __attribute__((packed)) {
    float voltage_volts;
    float current_amps;
    float consumed_milliamp_hours;
    uint8_t percent;
} POWER;

/* Enums */

typedef enum {
    FLIGHT_DISARMED,
    FLIGHT_ARMED,
} FLIGHT_STATE;

typedef enum { 
    FLIGHT_MANUAL,
    FLIGHT_ACROBATIC,
    FLIGHT_AUTONOMOUS,
    FLIGHT_WAYPOINT,
    FLIGHT_MISSION,
    FLIGHT_RETURN_TO_LAUNCH,
    FLIGHT_LAND,
} FLIGHT_MODE;

typedef enum {
    BOOTLOADER_NONE,
    BOOTLOADER_STATS,
    BOOTLOADER_ERASE_APP,
    BOOTLOADER_UPDATE,
    BOOTLOADER_VERIFY,
} BOOTLOADER_CMD;

typedef enum {
    ERROR_NONE,
    ERROR_CRC_FAIL,
    ERROR_UNKNOWN_MSG,
    ERROR_WRONG_VERSION,
    ERROR_PAYLOAD_OVERSIZE,
    ERROR_INVALID_STATE,
    ERROR_BUFFER_FULL,
    ERROR_AUTH_FAIL,
    ERROR_FLASH_FAIL,
    ERROR_SENSOR_FAIL,
    ERROR_TIMEOUT,
} ERROR_CODE;

typedef enum {
    OLED_PRINT,
    OLED_CLEAR,
} OLED_CMD;

typedef enum {
    BATTERY,
    FLIGHT_CONTROLLER,
    BOOTLOADER, 
    RTOS,
    MOTOR_CONTROLLER,
    MOTORS,
    CAMERA,
    BARAMETER,
    GYROSCOPE,
    COMMS,
    GENERIC,
} CATEGORY;

/* Message IDs  (flattened) */

typedef enum {
    MSG_HEARTBEAT,
    MSG_ACK,
    MSG_NACK,
    MSG_RC_CHANNELS,
    MSG_FLIGHT_STATE,
    MSG_FLIGHT_MODE,
    MSG_BOOTLOADER_CMD,
    MSG_BOOTLOADER_DATA,
    MSG_BOOTLOADER_STATS,
    MSG_TELEM_IMU,
    MSG_TELEM_GPS,
    MSG_TELEM_BAROMETER,
    MSG_TELEM_POWER,
    MSG_LOG_STRING,
    MSG_LOG_VALUE,
    MSG_ESP32_STATUS,
    MSG_OLED,

    MSG_COUNT
} MSG_ID;

/* Payloads  (one per MSG_ID) */

typedef struct __attribute__((packed)) { 
    uint32_t timestamp; 
    uint16_t error_flags;
    uint8_t state; 
    uint8_t mode;  
} HEARTBEAT_PAYLOAD;

typedef struct __attribute__((packed)) { 
    uint8_t ack_seq; 
} ACK_PAYLOAD;

typedef struct __attribute__((packed)) { 
    uint8_t nacked_seq; 
    uint8_t error;
} NACK_PAYLOAD;

typedef struct __attribute__((packed)) { 
    int16_t channels[16];  
} RC_CHANNELS_PAYLOAD;

typedef struct __attribute__((packed)) { 
    uint8_t state; 
} FLIGHT_STATE_PAYLOAD;

typedef struct __attribute__((packed)) { 
    uint8_t mode; 
} FLIGHT_MODE_PAYLOAD;

typedef struct __attribute__((packed)) { 
    uint32_t addr; 
    uint16_t len; 
    uint8_t cmd;  
    uint8_t signature[64];
} BOOTLOADER_CMD_PAYLOAD;

typedef struct __attribute__((packed)) { 
    uint32_t addr;
    uint8_t data[64]; 
} BOOTLOADER_DATA_PAYLOAD;

typedef struct __attribute__((packed)) {
    uint32_t cur_counter;
    uint32_t bank_a_version;
    uint32_t bank_b_version;
    uint8_t active_bank;
    uint8_t last_update_result;
} BOOTLOADER_STATS_PAYLOAD;

typedef struct __attribute__((packed)) {
    IMU imu;
} TELEM_IMU_PAYLOAD;

typedef struct __attribute__((packed)) {
    GPS gps;
} TELEM_GPS_PAYLOAD;

typedef struct __attribute__((packed)) {
    BAROMETER barometer;
} TELEM_BAROMETER_PAYLOAD;

typedef struct __attribute__((packed)) { 
    POWER power; 
} TELEM_POWER_PAYLOAD;

typedef struct __attribute__((packed)) {  
    char text[PAYLOAD_TEXT_SIZE]; 
} LOG_STRING_PAYLOAD;

typedef struct __attribute__((packed)) {
    uint8_t key_id;
    float value;
    uint32_t timestamp;
} LOG_VALUE_PAYLOAD;

typedef struct __attribute__((packed)) {
    uint32_t uptime_ms;
    uint32_t free_heap_bytes;
    uint32_t stm32_last_frame_age_ms;
    uint32_t stm32_frames_ok;
    uint32_t stm32_frames_err;
    uint8_t  stm32_link_up;
    uint8_t  wifi_client_count;
} ESP32_STATUS_PAYLOAD;

typedef struct __attribute__((packed)) {
    uint8_t cmd;
    char text[PAYLOAD_TEXT_SIZE];
} OLED_PAYLOAD;

/* Frame */

typedef struct __attribute__((packed)) {
    uint8_t start_byte;
    uint8_t version;
    uint8_t message_id;
    uint8_t sequence;
    uint8_t payload_len;
    uint8_t payload[PAYLOAD_MAX_SIZE];
    uint16_t crc;
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
    [MSG_ESP32_STATUS]     = sizeof(ESP32_STATUS_PAYLOAD),
    [MSG_OLED]             = sizeof(OLED_PAYLOAD),
};

#endif