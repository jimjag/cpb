/** @file struct_decoder.h
 * 
 * Simple C protocol buffers (cpb) struct decoder interface.
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

#ifndef __CPB_UTILS_STRUCT_DECODER_H__
#define __CPB_UTILS_STRUCT_DECODER_H__

#include <cpb/cpb.h>


/* Forward declaration */
struct cpb_struct_decoder;

/**
 * This handler is called when the decoder encountered a new message.
 * @param decoder Decoder
 * @param msg_desc Message descriptor
 * @param arg User argument
 */
typedef void (*cpb_struct_decoder_msg_start_handler_t)
    (struct cpb_struct_decoder *decoder,
     const struct cpb_msg_desc *msg_desc, void *arg);

/**
 * This handler is called when the decoder finished decoding a message.
 * @param decoder Decoder
 * @param msg_desc Message descriptor
 * @param arg User argument
 */
typedef void (*cpb_struct_decoder_msg_end_handler_t)
    (struct cpb_struct_decoder *decoder,
     const struct cpb_msg_desc *msg_desc, void *arg);

/**
 * This handler is called when the decoder has decoded a field.
 * @param decoder Decoder
 * @param msg_desc Message descriptor of the message containing the field
 * @param field_desc Field descriptor
 * @param value Field value
 * @param arg User argument
 */
typedef void (*cpb_struct_decoder_field_handler_t)
    (struct cpb_struct_decoder *decoder,
     const struct cpb_msg_desc *msg_desc,
     const struct cpb_field_desc *field_desc,
     union cpb_value *value, void *arg);


struct cpb_struct_decoder_stack_frame {
    const struct cpb_struct_map *map;
    void *base;
    const struct cpb_struct_map_field *last_field;
    int field_index;
};

/** Protocol buffer struct decoder */
struct cpb_struct_decoder {
    struct cpb_decoder decoder;
    void *arg;
    cpb_struct_decoder_msg_start_handler_t msg_start_handler;
    cpb_struct_decoder_msg_end_handler_t msg_end_handler;
    cpb_struct_decoder_field_handler_t field_handler;
    struct cpb_struct_decoder_stack_frame stack[CPB_MAX_DEPTH];
    int depth;
};

void cpb_struct_decoder_init(struct cpb_struct_decoder *sdecoder);

void cpb_struct_decoder_arg(struct cpb_struct_decoder *sdecoder, void *arg);

void cpb_struct_decoder_msg_handler(struct cpb_struct_decoder *sdecoder,
                                     cpb_struct_decoder_msg_start_handler_t msg_start_handler,
                                     cpb_struct_decoder_msg_end_handler_t msg_end_handler);

void cpb_struct_decoder_field_handler(struct cpb_struct_decoder *sdecoder,
                                       cpb_struct_decoder_field_handler_t field_handler);

cpb_err_t cpb_struct_decoder_decode(struct cpb_struct_decoder *sdecoder,
                                      const struct cpb_struct_map *struct_map,
                                      void *struct_base,
                                      void *data, size_t len, size_t *used);


#endif /* __CPB_UTILS_STRUCT_DECODER_H__ */
