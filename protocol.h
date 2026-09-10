#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include <stdint.h>

#define PROTOCOL_START_BYTE 0xAA
#define PROTOCOL_VERSION    0x09
#define PAYLOAD_MAX_SIZE    128
#define PAYLOAD_TEXT_SIZE   120

/* Primitives */

typedef struct __attribute__((packed)) {
    float x;
    float y;
    float z;
} vector;

typedef struct __attribute__((packed)) {
    float w;
    float i;
    float j;
    float k;
} quaternion;

typedef struct __attribute__((packed)) {
    int32_t latitude;
    int32_t longitude;
    float altitude_meters_abv_sealvl;
    float altitude_meters_abv_ground;
    int8_t fix_type;
} gps;

typedef struct __attribute__((packed)) {
    uint32_t timestamp;
    vector acceleration;
    vector gyro;
    vector magnitude;
} imu;

typedef struct __attribute__((packed)) {
    float pressure_pascal;
    float temperature_celsius;
    float altitude_meters;
} barometer;

typedef struct __attribute__((packed)) {
    float voltage_volts;
    float current_amps;
    float consumed_milliamp_hours;
    uint8_t percent;
} power;

/* Enums */

typedef enum {
    FLIGHT_DISARMED,
    FLIGHT_ARMED,
} flight_state;

typedef enum {
    FLIGHT_MANUAL,
    FLIGHT_ACROBATIC,
    FLIGHT_AUTONOMOUS,
    FLIGHT_WAYPOINT,
    FLIGHT_MISSION,
    FLIGHT_RETURN_TO_LAUNCH,
    FLIGHT_LAND,
} flight_mode;

typedef enum {
    BOOTLOADER_NONE,
    BOOTLOADER_STATS,
    BOOTLOADER_ERASE_APP,
    BOOTLOADER_UPDATE,
    BOOTLOADER_VERIFY,
} bootloader_cmd;

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
} error_code;

typedef enum {
    OLED_PRINT,
    OLED_CLEAR,
} oled_cmd;

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
    LIDAR,
    GENERIC,
} category;

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
    MSG_BOOTLOADER_VERIFY,
    MSG_TELEM_IMU,
    MSG_TELEM_GPS,
    MSG_TELEM_BAROMETER,
    MSG_TELEM_POWER,
    MSG_TELEM_LIDAR,
    MSG_LOG_STRING,
    MSG_LOG_VALUE,
    MSG_ESP32_STATUS,
    MSG_OLED,

    MSG_COUNT
} msg_id;

/* Payloads  (one per MSG_ID) */

typedef struct __attribute__((packed)) {
    uint32_t timestamp;
    uint16_t error_flags;
    uint8_t state;
    uint8_t mode;
} heartbeat_payload;

typedef struct __attribute__((packed)) {
    uint8_t ack_seq;
} ack_payload;

typedef struct __attribute__((packed)) {
    uint8_t nacked_seq;
    uint8_t error;
} nack_payload;

typedef struct __attribute__((packed)) {
    int16_t channels[16];
} rc_channels_payload;

typedef struct __attribute__((packed)) {
    uint8_t state;
} flight_state_payload;

typedef struct __attribute__((packed)) {
    uint8_t mode;
} flight_mode_payload;

typedef struct __attribute__((packed)) {
    uint32_t addr;
    uint16_t len;
    uint8_t cmd;
    uint8_t signature[64];
} bootloader_cmd_payload;

typedef struct __attribute__((packed)) {
    uint32_t addr;
    uint8_t data[64];
} bootloader_data_payload;

typedef struct __attribute__((packed)) {
    uint32_t cur_counter;
    uint32_t bank_a_version;
    uint32_t bank_b_version;
    uint8_t active_bank;
    uint8_t last_update_result;
} bootloader_stats_payload;

typedef struct __attribute__((packed)) {
    imu imu;
} telem_imu_payload;

typedef struct __attribute__((packed)) {
    gps gps;
} telem_gps_payload;

typedef struct __attribute__((packed)) {
    barometer barometer;
} telem_barometer_payload;

typedef struct __attribute__((packed)) {
    power power;
} telem_power_payload;

typedef struct __attribute__((packed)) {
    uint16_t distance;
    uint16_t amp_strength;
    uint16_t temperature;
} telem_lidar_payload;

typedef struct __attribute__((packed)) {
    char text[PAYLOAD_TEXT_SIZE];
} log_string_payload;

typedef struct __attribute__((packed)) {
    uint8_t key_id;
    float value;
    uint32_t timestamp;
} log_value_payload;

typedef struct __attribute__((packed)) {
    uint32_t uptime_ms;
    uint32_t free_heap_bytes;
    uint32_t stm32_last_frame_age_ms;
    uint32_t stm32_frames_ok;
    uint32_t stm32_frames_err;
    uint8_t  stm32_link_up;
    uint8_t  wifi_client_count;
} esp32_status_payload;

typedef struct __attribute__((packed)) {
    uint8_t cmd;
    char text[PAYLOAD_TEXT_SIZE];
} oled_payload;

/* Frame */

typedef struct __attribute__((packed)) {
    uint8_t start_byte;
    uint8_t version;
    uint8_t message_id;
    uint8_t sequence;
    uint8_t payload_len;
    uint8_t payload[PAYLOAD_MAX_SIZE];
    uint16_t crc;
} frame;

static const uint8_t MSG_PAYLOAD_SIZE[] = {
    [MSG_HEARTBEAT]        = sizeof(heartbeat_payload),
    [MSG_ACK]              = sizeof(ack_payload),
    [MSG_NACK]             = sizeof(nack_payload),
    [MSG_RC_CHANNELS]      = sizeof(rc_channels_payload),
    [MSG_FLIGHT_STATE]     = sizeof(flight_state_payload),
    [MSG_FLIGHT_MODE]      = sizeof(flight_mode_payload),
    [MSG_BOOTLOADER_CMD]   = sizeof(bootloader_cmd_payload),
    [MSG_BOOTLOADER_DATA]  = sizeof(bootloader_data_payload),
    [MSG_BOOTLOADER_STATS] = sizeof(bootloader_stats_payload),
    [MSG_TELEM_IMU]        = sizeof(telem_imu_payload),
    [MSG_TELEM_GPS]        = sizeof(telem_gps_payload),
    [MSG_TELEM_BAROMETER]  = sizeof(telem_barometer_payload),
    [MSG_TELEM_POWER]      = sizeof(telem_power_payload),
    [MSG_TELEM_LIDAR]      = sizeof(lidar_payload),
    [MSG_LOG_STRING]       = sizeof(log_string_payload),
    [MSG_LOG_VALUE]        = sizeof(log_value_payload),
    [MSG_ESP32_STATUS]     = sizeof(esp32_status_payload),
    [MSG_OLED]             = sizeof(oled_payload),
};

#endif
