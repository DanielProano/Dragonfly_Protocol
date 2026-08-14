#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

#define PROTOCOL_START_BYTE 0xAA
#define PROTOCOL_VERSION 0x01
#define PAYLOAD_MAX_SIZE 128

typedef struct {
    float x;
    float y;
    float z;
} VECTOR;

typedef struct {
    float w;
    float i;
    float j;
    float k;
} QUATERNION;

typedef struct {
    int32_t latitude;
    int32_t longitude;
    float meters_above_sealvl;
    float meters_above_ground;
    int8_t fix_type;
} GPS;

typedef struct {
    uint32_t timestamp;
    VECTOR acceleration;
    VECTOR gyro;
    VECTOR magnitude;
} IMU;

typedef enum : uint8_t {
    /* System */
    MSG_SYS_STATUS,
    MSG_HEARTBEAT,
    MSG_ACK,
    MSG_NACK,

    /* Modes */
    MSG_FLIGHT_MODE,
    MSG_FLIGHT_STATE,

    /* Commands */
    MSG_BOOTLOADER_CMD,
    MSG_BOOTLOADER_DATA,

    /* Info */
    TELEM_IMU,
    TELEM_GPS,
    TELEM_BAROMETER,
    TELEM_POWER,

    /* Debug */
    MSG_LOG_STRING,
    MSG_LOG_VALUE,
} MSG_ID;

typedef enum : uint8_t {
    SYS_OK,
    SYS_PROBLEM,
} SYSTEM_STATUS;

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

typedef struct {
    uint8_t start_byte;
    uint8_t version;
    MSG_ID message_id;
    uint8_t sequence;
    uint8_t payload_len;
    uint8_t payload[PAYLOAD_MAX_SIZE];
    uint16_t CRC;
} FRAME;

typedef struct __attribute__((packed)) { 
    uint8_t acked_seq; 
    ERROR_CODE error; 
} ACK;

typedef struct __attribute__((packed)) { 
    uint32_t timestamp; 
    FLIGHT_STATE state; 
    FLIGHT_MODE mode; 
    uint16_t error_flags; 
} Heartbeat;

typedef struct __attribute__((packed)) { 
    uint32_t timestamp; 
    int16_t ch[16]; 
    uint16_t valid_mask; 
} RcChannels;

typedef struct __attribute__((packed)) { 
    FLIGHT_STATE requested_state; 
} ArmCmd;

typedef struct __attribute__((packed)) { 
    FLIGHT_MODE requested_mode; 
} FlightModeCmd;

typedef struct __attribute__((packed)) { 
    BOOTLOADER_CMD cmd; 
    uint32_t addr; 
    uint16_t len; 
    uint8_t nonce[8]; 
} BootloaderCmdMsg;

typedef struct __attribute__((packed)) { 
    uint32_t addr; 
    uint8_t len; 
    uint8_t data[64]; 
} BootloaderData;

typedef struct __attribute__((packed)) { 
    uint32_t uptime_ms; 
    uint8_t cpu_load; 
    SYSTEM_STATUS status; 
} SysStatus;

typedef struct __attribute__((packed)) { 
    IMU imu; 
} TelemetryImu;

typedef struct __attribute__((packed)) { 
    GPS gps; 
    float vel_ned[3]; 
} TelemetryGps;

typedef struct __attribute__((packed)) {
    float pressure_pa; 
    float temp_c; 
    float alt_m; 
} TelemetryBaro;

typedef struct __attribute__((packed)) { 
    float voltage; 
    float current; 
    float consumed_mah; 
    uint8_t percent; 
} TelemetryPower;

typedef struct __attribute__((packed)) { 
    uint8_t level; 
    char text[120]; 
} LogString;

typedef struct __attribute__((packed)) { 
    uint8_t key_id; 
    float value; 
    uint32_t timestamp; 
} LogValue;

#endif