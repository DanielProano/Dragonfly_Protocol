#ifndef _PROTOCOL_CODEC_
#define _PROTOCOL_CODEC_

#include <stdbool.h>
#include <stddef.h>

#include "protocol.h"

#define TOTAL_HEADER_BYTE_SIZE offsetof(frame, payload)

void protocol_frame_init(void);
int protocol_frame_encode(uint8_t *buffer, size_t buf_size, const frame *in_frame);
int protocol_frame_decode(frame *out_frame, const uint8_t *buffer, size_t buf_size);

#endif