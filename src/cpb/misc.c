/** @file misc.c
 * 
 * Misc functions.
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

#include <cpb/cpb.h>

#include "private.h"

static const struct cpb_struct_map_field *find_map_field(
        const struct cpb_struct_map *map,
        const struct cpb_field_desc *field_desc)
{
    const struct cpb_struct_map_field *field;
    
    for (field = map->fields; field->field_desc; field++)
        if (field->field_desc == field_desc)
            return field;
    return NULL;
}

#define FIELD_BASE(_field_, _base_, _index_) \
    ((_base_) + (_field_)->ofs + ((_field_)->len * (_index_)))

static void unpack_field(struct cpb_struct_decoder *sdecoder,
                         const struct cpb_struct_map_field *field,
                         union cpb_value *value)
{
    size_t len;
    struct cpb_struct_decoder_stack_frame *frame;
    
    frame = &sdecoder->stack[sdecoder->depth];
    
    /* Reset field index if a new field is encountered */
    if (field != frame->last_field)
        frame->field_index = 0;
    frame->last_field = field;
    
    if (field->count >= field->count)

    switch (field->field_desc->opts.typ) {
    case CPB_DOUBLE:
        CPB_ASSERT(field->len == sizeof(double), "Field type mismatch");
        *((double *) FIELD_BASE(field, frame->base, frame->field_index)) = value->double_;
        frame->field_index++;
        break;
    case CPB_FLOAT:
        CPB_ASSERT(field->len == sizeof(float), "Field type mismatch");
        *((float *) FIELD_BASE(field, frame->base, frame->field_index)) = value->float_;
        frame->field_index++;
        break;
    case CPB_INT32:
    case CPB_SINT32:
    case CPB_SFIXED32:
        CPB_ASSERT(field->len == sizeof(s32_t), "Field type mismatch");
        *((s32_t *) FIELD_BASE(field, frame->base, frame->field_index)) = value->int32;
        frame->field_index++;
        break;
    case CPB_UINT32:
    case CPB_FIXED32:
        CPB_ASSERT(field->len == sizeof(u32_t), "Field type mismatch");
        *((u32_t *) FIELD_BASE(field, frame->base, frame->field_index)) = value->uint32;
        frame->field_index++;
        break;
    case CPB_INT64:
    case CPB_SINT64:
    case CPB_SFIXED64:
        CPB_ASSERT(field->len == sizeof(s64_t), "Field type mismatch");
        *((s64_t *) FIELD_BASE(field, frame->base, frame->field_index)) = value->int64;
        frame->field_index++;
        break;
    case CPB_UINT64:
    case CPB_FIXED64:
        CPB_ASSERT(field->len == sizeof(u64_t), "Field type mismatch");
        *((u64_t *) FIELD_BASE(field, frame->base, frame->field_index)) = value->uint64;
        frame->field_index++;
        break;
    case CPB_BOOL:
        CPB_ASSERT(field->len == sizeof(cpb_bool_t), "Field type mismatch");
        *((cpb_bool_t *) FIELD_BASE(field, frame->base, frame->field_index)) = value->bool;
        frame->field_index++;
        break;
    case CPB_ENUM:
        CPB_ASSERT(field->len == sizeof(cpb_enum_t), "Field type mismatch");
        *((cpb_enum_t *) FIELD_BASE(field, frame->base, frame->field_index)) = value->enum_;
        frame->field_index++;
        break;
    case CPB_STRING:
        len = field->len < value->string.len + 1 ? field->len : value->string.len + 1;
        memcpy(FIELD_BASE(field, frame->base, frame->field_index), value->string.str, len);
        ((char *) FIELD_BASE(field, frame->base, frame->field_index))[len - 1] = '\0';
        frame->field_index++;
        break;
    case CPB_BYTES:
        len = field->len < value->bytes.len ? field->len : value->bytes.len;
        memcpy(FIELD_BASE(field, frame->base, frame->field_index), value->bytes.data, len);
        frame->field_index++;
        break;
    case CPB_MESSAGE:
        printf("submessage\n");
        break;
    }
    
}

static void sdecoder_msg_start_handler(struct cpb_decoder *decoder,
                                    const struct cpb_msg_desc *msg_desc,
                                    void *arg)
{
    struct cpb_struct_decoder *sdecoder = arg;
    struct cpb_struct_decoder_stack_frame *frame, *last_frame;

    printf("msg start\n");

    sdecoder->depth++;
    frame = &sdecoder->stack[sdecoder->depth];
    
    if (sdecoder->depth > 0) {
        last_frame = &sdecoder->stack[sdecoder->depth - 1];
        frame->map = (const struct cpb_struct_map *) last_frame->last_field->len;
        frame->base = last_frame->base + last_frame->last_field->ofs +
            (frame->map->struct_size * last_frame->field_index);
        frame->last_field = NULL;
        frame->field_index = 0;
        last_frame->field_index++;
    }
    
    CPB_ASSERT(frame->map->msg_desc == msg_desc, "Message type mismatch");
    
    if (sdecoder->msg_start_handler)
        sdecoder->msg_start_handler(sdecoder, msg_desc, sdecoder->arg);
}

static void sdecoder_msg_end_handler(struct cpb_decoder *decoder,
                                  const struct cpb_msg_desc *msg_desc,
                                  void *arg)
{
    struct cpb_struct_decoder *sdecoder = arg;
    struct cpb_struct_decoder_stack_frame *frame;
    
    printf("msg end\n");
    
    sdecoder->depth--;
    frame = &sdecoder->stack[sdecoder->depth];

    if (sdecoder->msg_end_handler)
        sdecoder->msg_end_handler(sdecoder, msg_desc, sdecoder->arg);
}

static void sdecoder_field_handler(struct cpb_decoder *decoder,
                                const struct cpb_msg_desc *msg_desc,
                                const struct cpb_field_desc *field_desc,
                                union cpb_value *value, void *arg)
{
    struct cpb_struct_decoder *sdecoder = arg;
    struct cpb_struct_decoder_stack_frame *frame = &sdecoder->stack[sdecoder->depth];
    const struct cpb_struct_map_field *field;
    
    field = find_map_field(frame->map, field_desc);
    if (field)
        unpack_field(sdecoder, field, value);

    if (sdecoder->field_handler)
        sdecoder->field_handler(sdecoder, msg_desc, field_desc, value, sdecoder->arg);
}



/* Struct decoder */

/**
 * Initializes the struct decoder.
 * @param sdecoder Struct decoder
 */
void cpb_struct_decoder_init(struct cpb_struct_decoder *sdecoder)
{
    /* Initialize decoder */
    cpb_decoder_init(&sdecoder->decoder);
    cpb_decoder_arg(&sdecoder->decoder, sdecoder);
    cpb_decoder_msg_handler(&sdecoder->decoder, 
            sdecoder_msg_start_handler, sdecoder_msg_end_handler);
    cpb_decoder_field_handler(&sdecoder->decoder,
            sdecoder_field_handler);
    
    /* Initialize internals */
    sdecoder->arg = NULL;
    sdecoder->msg_start_handler = NULL;
    sdecoder->msg_end_handler = NULL;
    sdecoder->field_handler = NULL;
}

/**
 * Sets the user argument to be passed back with the handlers.
 * @param sdecoder Struct decoder
 * @param arg User argument
 */
void cpb_struct_decoder_arg(struct cpb_struct_decoder *sdecoder, void *arg)
{
    sdecoder->arg = arg;
}

/**
 * Sets the message start and end handlers.
 * @param sdecoder Struct decoder
 * @param msg_start_handler Message start handler
 * @param msg_end_handler Message end handler
 */
void cpb_struct_decoder_msg_handler(struct cpb_struct_decoder *sdecoder,
                                     cpb_struct_decoder_msg_start_handler_t msg_start_handler,
                                     cpb_struct_decoder_msg_end_handler_t msg_end_handler)
{
    sdecoder->msg_start_handler = msg_start_handler;
    sdecoder->msg_end_handler = msg_end_handler;
}

/**
 * Sets the field handler.
 * @param sdecoder Struct decoder
 * @param field_handler Field handler
 */
void cpb_struct_decoder_field_handler(struct cpb_struct_decoder *sdecoder,
                                       cpb_struct_decoder_field_handler_t field_handler)
{
    sdecoder->field_handler = field_handler;
}

/**
 * Decodes a protocol buffer into a struct.
 * @param sdecoder Struct decoder
 * @param struct_map Struct map used for decoding
 * @param struct_base Base of the struct to decode into
 * @param data Data to decode
 * @param len Length of data to decode
 * @param used Returns the number of decoded bytes when not NULL.
 * @return Returns CPB_ERR_OK when data was successfully decoded.
 */
cpb_err_t cpb_struct_decoder_decode(struct cpb_struct_decoder *sdecoder,
                                      const struct cpb_struct_map *struct_map,
                                      void *struct_base,
                                      void *data, size_t len, size_t *used)
{
    sdecoder->depth = -1;
    sdecoder->stack[0].map = struct_map;
    sdecoder->stack[0].base = struct_base;
    sdecoder->stack[0].last_field = NULL;
    sdecoder->stack[0].field_index = 0;
    
    return cpb_decoder_decode(&sdecoder->decoder, struct_map->msg_desc, data, len, used);
}


/**
 * Returns a textual description of an cpb error code.
 * @param err Error code
 * @return Returns the textual description.
 */
const char *cpb_err_text(cpb_err_t err)
{
    switch (err) {
    case CPB_ERR_OK:
        return "OK";
    case CPB_ERR_CANCEL:
        return "Cancelled by user";
    case CPB_ERR_UNKNOWN_FIELD:
        return "Unknown field";
    case CPB_ERR_INVALID_FIELD:
        return "Invalid field";
    case CPB_ERR_END_OF_BUF:
        return "End of buffer";
    case CPB_ERR_MEM:
        return "Memory allocation failed";
    case CPB_ERR_NET_INIT:
        return "Network initialization failed";
    default:
        return "Unknown";
    }
}

/**
 * Initializes a memory buffer. Sets the position to the base address.
 * @param buf Memory buffer
 * @param data Base address of memory
 * @param len Length of memory
 */
void cpb_buf_init(struct cpb_buf *buf, void *data, size_t len)
{
    buf->base = data;
    buf->pos = data;
    buf->end = &buf->base[len];
}

/**
 * Returns the number of used bytes in the buffer.
 * @param buf Memory buffer
 * @return Returns the number of used bytes.
 */
size_t cpb_buf_used(struct cpb_buf *buf)
{
    return buf->pos - buf->base;
}

/**
 * Returns the number of bytes left in the buffer.
 * @param buf Memory buffer
 * @return Returns the number of bytes left.
 */
size_t cpb_buf_left(struct cpb_buf *buf)
{
    return buf->end - buf->pos;
}
