#ifndef _PROTOCOL_CODEC_
#define _PROTOCOL_CODEC_

#include <stdbool.h>
#include <stdio.h>
#include "protocol.h"

int protocol_frame_encode(uint8_t *buffer, size_t buf_size, const FRAME *frame);
int protocol_frame_decode(FRAME *frame, const uint8_t *buffer, size_t buf_size);

#endif