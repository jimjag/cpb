/** @file decoder.h
 *
 * Simple C protocol buffers (cpb) decoder interface.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __CPB_CORE_DECODER_H__
#define __CPB_CORE_DECODER_H__

#include <cpb/cpb.h>


/* Forward declaration */
struct cpb_decoder;

/**
 * This handler is called when the decoder encountered a new message.
 * @param decoder Decoder
 * @param msg_desc Message descriptor
 * @param arg User argument
 */
typedef void (*cpb_decoder_msg_start_handler_t)
    (struct cpb_decoder *decoder,
     const struct cpb_msg_desc *msg_desc, void *arg);

/**
 * This handler is called when the decoder finished decoding a message.
 * @param decoder Decoder
 * @param msg_desc Message descriptor
 * @param arg User argument
 */
typedef void (*cpb_decoder_msg_end_handler_t)
    (struct cpb_decoder *decoder,
     const struct cpb_msg_desc *msg_desc, void *arg);

/**
 * This handler is called when the decoder has decoded a field.
 * @param decoder Decoder
 * @param msg_desc Message descriptor of the message containing the field
 * @param field_desc Field descriptor
 * @param value Field value
 * @param arg User argument
 */
typedef void (*cpb_decoder_field_handler_t)
    (struct cpb_decoder *decoder,
     const struct cpb_msg_desc *msg_desc,
     const struct cpb_field_desc *field_desc,
     union cpb_value *value, void *arg);


/** Decoder stack frame */
struct cpb_decoder_stack_frame {
    struct cpb_buf buf;
    const struct cpb_msg_desc *msg_desc;
};

/** Protocol buffer decoder */
struct cpb_decoder {
    void *arg;
    cpb_decoder_msg_start_handler_t msg_start_handler;
    cpb_decoder_msg_end_handler_t msg_end_handler;
    cpb_decoder_field_handler_t field_handler;
    struct cpb_decoder_stack_frame stack[CPB_MAX_DEPTH];
    int depth;
    int packed;
};

void cpb_decoder_init(struct cpb_decoder *decoder);

void cpb_decoder_arg(struct cpb_decoder *decoder, void *arg);

void cpb_decoder_msg_handler(struct cpb_decoder *decoder,
                            cpb_decoder_msg_start_handler_t msg_start_handler,
                            cpb_decoder_msg_end_handler_t msg_end_handler);

void cpb_decoder_field_handler(struct cpb_decoder *decoder,
                              cpb_decoder_field_handler_t field_handler);

void cpb_decoder_use_debug_handlers(struct cpb_decoder *decoder);

cpb_err_t cpb_decoder_decode(struct cpb_decoder *decoder,
                               const struct cpb_msg_desc *msg_desc,
                               void *data, size_t len, size_t *used);

cpb_err_t cpb_decode_varint(struct cpb_buf *buf, u64_t *varint);

cpb_err_t cpb_decode_32bit(struct cpb_buf *buf, u32_t *value);

cpb_err_t cpb_decode_64bit(struct cpb_buf *buf, u64_t *value);

#endif /* __CPB_CORE_DECODER_H__ */
