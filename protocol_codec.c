#include <stdint.h>
#include <string.h>

#include "crc.h"
#include "protocol_codec.h"
#include "protocol.h"
#include "macros.h"

static bool g_configured = false;

void protocol_frame_init(void) {
    compute_crc16_table();
    g_configured = true;
}

int protocol_frame_encode(uint8_t *buffer, size_t buf_size, const frame *in_frame) {
    RETURN_IF_NULL(in_frame, -1);
    RETURN_IF_NULL(buffer, -1);
    RETURN_IF_EMPTY(buf_size, -1);

    if (!g_configured) {
        return -1;
    }

    if (in_frame->payload_len > PAYLOAD_MAX_SIZE) {
        return -1;
    }

    size_t total_len = TOTAL_HEADER_BYTE_SIZE + in_frame->payload_len + sizeof(in_frame->crc);

    if (total_len > buf_size) {
        return -1;
    }

    buffer[0] = PROTOCOL_START_BYTE;
    buffer[1] = PROTOCOL_VERSION;
    buffer[2] = in_frame->message_id;
    buffer[3] = in_frame->sequence;
    buffer[4] = in_frame->payload_len;

    memcpy(&buffer[5], in_frame->payload, in_frame->payload_len);

    size_t crc_offset = TOTAL_HEADER_BYTE_SIZE + in_frame->payload_len;
    uint16_t crc = compute_crc16(buffer, crc_offset);

    buffer[crc_offset] = (uint8_t) (crc & 0xFF);
    buffer[crc_offset + 1] = (uint8_t) (crc >> 8);

    return (int) total_len;
}

int protocol_frame_decode(frame *out_frame, const uint8_t *buffer, size_t buf_size) {
    RETURN_IF_NULL(out_frame, -1);
    RETURN_IF_NULL(buffer, -1);
    RETURN_IF_EMPTY(buf_size, -1);

    if (!g_configured)
        return -1;

    if (buf_size < TOTAL_HEADER_BYTE_SIZE + sizeof(out_frame->crc))
        return -1;

    if (buffer[0] != PROTOCOL_START_BYTE)
        return -1;

    if (buffer[1] != PROTOCOL_VERSION)
        return -1;

    uint8_t payload_len = buffer[4];

    if (payload_len > PAYLOAD_MAX_SIZE)
        return -1;

    size_t total_size = TOTAL_HEADER_BYTE_SIZE + payload_len + sizeof(out_frame->crc);

    if (buf_size < total_size)
        return -1;

    size_t crc_offset = TOTAL_HEADER_BYTE_SIZE + payload_len;
    uint16_t received_crc = (uint16_t) buffer[crc_offset] | (uint16_t) (buffer[crc_offset + 1] << 8);

    uint16_t calc_crc = compute_crc16(buffer, crc_offset);

    if (received_crc != calc_crc)
        return -1;

    out_frame->start_byte   = buffer[0];
    out_frame->version      = buffer[1];
    out_frame->message_id   = buffer[2];
    out_frame->sequence     = buffer[3];
    out_frame->payload_len  = payload_len;

    memcpy(out_frame->payload, &buffer[5], payload_len);

    out_frame->crc = received_crc;

    return (int) total_size;
}
