/** @file encoder.c
 * 
 * Implementation of the protocol buffers encoder.
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


#define MSG_RESERVE_BYTES 10

/* Encoder utilities */

/**
 * Encodes a variable integer in base-128 format.
 * See http://code.google.com/apis/protocolbuffers/docs/encoding.html for more
 * information.
 * @param buf Memory buffer
 * @param varint Value to encode
 * @return Returns CPB_ERR_OK if successful or CPB_ERR_END_OF_BUF if there
 * was not enough space left in the memory buffer. 
 */
static cpb_err_t encode_varint(struct cpb_buf *buf, u64_t varint)
{
    do {
        if (cpb_buf_left(buf) < 1)
            return CPB_ERR_END_OF_BUF;
        if (varint > 127) {
            *buf->pos = 0x80 | (varint & 0x7F);
        } else {
            *buf->pos = (varint & 0x7F);
        }
        varint >>= 7;
        buf->pos++;
    } while (varint);
    
    return CPB_ERR_OK;
}

/**
 * Encodes a 32 bit integer.
 * @param buf Memory buffer
 * @param value Value to encode
 * @return Returns CPB_ERR_OK if successful or CPB_ERR_END_OF_BUF if there
 * was not enough space left in the memory buffer. 
 */
static cpb_err_t encode_32bit(struct cpb_buf *buf, u32_t value)
{
    if (cpb_buf_left(buf) < 4)
        return CPB_ERR_END_OF_BUF;
    
    buf->pos[0] = (value) & 0xff;
    buf->pos[1] = (value >> 8) & 0xff;
    buf->pos[2] = (value >> 16) & 0xff;
    buf->pos[3] = (value >> 24) & 0xff;
    buf->pos += 4;
    
    return CPB_ERR_OK;
}

/**
 * Encodes a 64 bit integer.
 * @param buf Memory buffer
 * @param value Value to encode
 * @return Returns CPB_ERR_OK if successful or CPB_ERR_END_OF_BUF if there
 * was not enough space left in the memory buffer. 
 */
static cpb_err_t encode_64bit(struct cpb_buf *buf, u64_t value)
{
    if (cpb_buf_left(buf) < 8)
        return CPB_ERR_END_OF_BUF;
    
    buf->pos[0] = (value) & 0xff;
    buf->pos[1] = (value >> 8) & 0xff;
    buf->pos[2] = (value >> 16) & 0xff;
    buf->pos[3] = (value >> 24) & 0xff;
    value >>= 32;
    buf->pos[4] = (value) & 0xff;
    buf->pos[5] = (value >> 8) & 0xff;
    buf->pos[6] = (value >> 16) & 0xff;
    buf->pos[7] = (value >> 24) & 0xff;
    buf->pos += 8;
    
    return CPB_ERR_OK;
}

/**
 * Pushes the encoder stack.
 * @param encoder Encoder
 * @return Returns the top stack frame.
 */
static struct cpb_encoder_stack_frame *push_stack_frame(struct cpb_encoder *encoder)
{
    encoder->depth++;
    CPB_ASSERT(encoder->depth <= CPB_MAX_DEPTH, "Message nesting too deep");
    return &encoder->stack[encoder->depth - 1];
}

/**
 * Pops the encoder stack.
 * @param encoder Encoder
 * @return Returns the top stack frame.
 */
static struct cpb_encoder_stack_frame *pop_stack_frame(struct cpb_encoder *encoder)
{
    encoder->depth--;
    CPB_ASSERT(encoder->depth > 0, "Message nesting too shallow");
    return &encoder->stack[encoder->depth - 1];
}

/* Encoder */

/**
 * Initializes the encoder.
 * @param encoder Encoder
 */
void cpb_encoder_init(struct cpb_encoder *encoder)
{
    encoder->depth = 0;
}

/**
 * Starts encoding a message.
 * @param encoder Encoder
 * @param msg_desc Root message descriptor
 * @param data Data buffer to encode into
 * @param len Length of data buffer
 */
void cpb_encoder_start(struct cpb_encoder *encoder,
                        const struct cpb_msg_desc *msg_desc,
                        void *data, size_t len)
{
    struct cpb_encoder_stack_frame *frame = &encoder->stack[0];
    
    encoder->depth = 1;
    encoder->packed = 0;
    
    cpb_buf_init(&frame->buf, data, len);
    frame->field_desc = NULL;
    frame->msg_desc = msg_desc;
}

/**
 * Finishes encoding a message.
 * @param encoder Encoder
 * @return Returns the total size of the encoded message.
 */
size_t cpb_encoder_finish(struct cpb_encoder *encoder)
{
    return cpb_buf_used(&encoder->stack[0].buf);
}

/**
 * Starts encoding a nested message.
 * @param encoder Encoder
 * @param field_desc Field descriptor holding the nested message
 * @return Returns CPB_ERR_OK if successful.
 */
cpb_err_t cpb_encoder_nested_start(struct cpb_encoder *encoder,
                                     const struct cpb_field_desc *field_desc)
{
    struct cpb_encoder_stack_frame *frame, *new_frame;
    
    CPB_ASSERT(field_desc->opts.typ == CPB_MESSAGE, "Field is not a message");

    /* Get parent frame */
    frame = &encoder->stack[encoder->depth - 1];

    /* Create a new frame */
    new_frame = push_stack_frame(encoder);
    new_frame->field_desc = field_desc;
    new_frame->msg_desc = field_desc->msg_desc;
    
    /*
     * Reserve a few bytes for the field on the parent frame. This is where
     * the field key (message) and the message length will be stored, once it
     * is known.
     */
    if (cpb_buf_left(&frame->buf) < MSG_RESERVE_BYTES)
        return CPB_ERR_END_OF_BUF;
    cpb_buf_init(&new_frame->buf, frame->buf.pos + MSG_RESERVE_BYTES,
                  cpb_buf_left(&frame->buf) - MSG_RESERVE_BYTES);
    
    return CPB_ERR_OK;
}

/**
 * Ends encoding a nested message.
 * @param encoder Encoder
 * @return Returns CPB_ERR_OK if successful.
 */
cpb_err_t cpb_encoder_nested_end(struct cpb_encoder *encoder)
{
    struct cpb_encoder_stack_frame *frame;
    union cpb_value value;
    
    /* Get current frame */
    frame = &encoder->stack[encoder->depth - 1];

    /* Pop the stack */
    pop_stack_frame(encoder);

    value.message.data = frame->buf.base;
    value.message.len = cpb_buf_used(&frame->buf);
    return cpb_encoder_add_field(encoder, frame->field_desc, &value);
}

/**
 * Starts encoding a packed repeated field.
 * @param encoder Encoder
 * @param field_desc Field descriptor of packed repeated field
 * @return Returns CPB_ERR_OK if successful.
 */
cpb_err_t cpb_encoder_packed_repeated_start(struct cpb_encoder *encoder,
                                              const struct cpb_field_desc *field_desc)
{
    struct cpb_encoder_stack_frame *frame, *new_frame;

    CPB_ASSERT(CPB_IS_PACKED_REPEATED(field_desc),
                "Field is not repeated packed");
    
    CPB_ASSERT(!encoder->packed, "Packed repeated fields must not be nested");
    
    /* Get parent frame */
    frame = &encoder->stack[encoder->depth - 1];

    /* Create a new frame */
    new_frame = push_stack_frame(encoder);
    new_frame->field_desc = field_desc;
    new_frame->msg_desc = NULL;
    
    /*
     * Reserve a few bytes for the field on the parent frame. This is where
     * the field key (type) and the message length will be stored, once it
     * is known.
     */
    if (cpb_buf_left(&frame->buf) < MSG_RESERVE_BYTES)
        return CPB_ERR_END_OF_BUF;
    cpb_buf_init(&new_frame->buf, frame->buf.pos + MSG_RESERVE_BYTES,
                  cpb_buf_left(&frame->buf) - MSG_RESERVE_BYTES);
    
    /* Enter packed repeated mode */
    encoder->packed = 1;
    
    return CPB_ERR_OK;
}

/**
 * Ends encoding a packed repeated field.
 * @param encoder Encoder
 * @return Returns CPB_ERR_OK if successful.
 */
cpb_err_t cpb_encoder_packed_repeated_end(struct cpb_encoder *encoder)
{
    struct cpb_encoder_stack_frame *frame;
    union cpb_value value;
    
    CPB_ASSERT(encoder->packed, "Not in packed repeated mode");

    /* Get current frame */
    frame = &encoder->stack[encoder->depth - 1];

    /* Pop the stack */
    pop_stack_frame(encoder);
    
    /* Leave packed repeated mode */
    encoder->packed = 0;
    
    value.message.data = frame->buf.base;
    value.message.len = cpb_buf_used(&frame->buf);
    return cpb_encoder_add_field(encoder, frame->field_desc, &value);
}

/**
 * Encodes a field.
 * @note This method should not normally be used. Use the cpb_encoder_add_xxx()
 * methods to directly add a field of a given type.
 * @param encoder Encoder
 * @param field_desc Field descriptor of field to encode
 * @param value Field value
 * @return Returns CPB_ERR_OK if successful.
 */
cpb_err_t cpb_encoder_add_field(struct cpb_encoder *encoder,
                                  const struct cpb_field_desc *field_desc,
                                  union cpb_value *value)
{
    cpb_err_t ret;
    struct cpb_encoder_stack_frame *frame;
    int i;
    u64_t key;
    enum wire_type wire_type = 0;
    union wire_value wire_value;
    
    CPB_ASSERT(encoder->depth > 0, "Fields can only be added inside a message");
    
    /* Get current frame */
    frame = &encoder->stack[encoder->depth - 1];
    
    if (encoder->packed) {
        /* Check that packed repeated field is not interleaved with other fields */
        CPB_ASSERT(field_desc == frame->field_desc,
                    "Packed repeated fields must not be interleaved with other"
                    "fields");
        if (field_desc != frame->field_desc)
            return CPB_ERR_INVALID_FIELD;
    } else {
        /* Check that field belongs to the current message */
        for (i = 0; i < frame->msg_desc->num_fields; i++)
            if (field_desc == &frame->msg_desc->fields[i])
                break;
        if (i == frame->msg_desc->num_fields)
            return CPB_ERR_UNKNOWN_FIELD;
    }
    
    /* Encode wire value */
    switch (field_desc->opts.typ) {
    case CPB_DOUBLE:
        wire_type = WT_64BIT;
        memcpy(&wire_value.int64, &value->double_, sizeof(double));
        break;
    case CPB_FLOAT:
        wire_type = WT_32BIT;
        memcpy(&wire_value.int32, &value->float_, sizeof(float));
        break;
    case CPB_INT32:
        wire_type = WT_VARINT;
        wire_value.varint = value->int32;
        break;
    case CPB_UINT32:
        wire_type = WT_VARINT;
        wire_value.varint = value->uint32;
        break;
    case CPB_SINT32:
        /* Zig-zag encoding */
        wire_type = WT_VARINT;
        wire_value.varint = (u32_t) ((value->int32 << 1) ^ (value->int32 >> 31));
        break;
    case CPB_INT64:
        wire_type = WT_VARINT;
        wire_value.varint = value->int64;
        break;
    case CPB_UINT64:
        wire_type = WT_VARINT;
        wire_value.varint = value->uint64;
        break;
    case CPB_SINT64:
        /* Zig-zag encoding */
        wire_type = WT_VARINT;
        wire_value.varint = (u64_t) ((value->int64 << 1) ^ (value->int64 >> 63));
        break;
    case CPB_FIXED32:
        wire_type = WT_32BIT;
        wire_value.int32 = value->uint32;
        break;
    case CPB_FIXED64:
        wire_type = WT_64BIT;
        wire_value.int64 = value->uint64;
        break;
    case CPB_SFIXED32:
        wire_type = WT_32BIT;
        wire_value.int32 = value->int32;
        break;
    case CPB_SFIXED64:
        wire_type = WT_64BIT;
        wire_value.int64 = value->int64;
        break;
    case CPB_BOOL:
        wire_type = WT_VARINT;
        wire_value.varint = value->bool;
        break;
    case CPB_ENUM:
        wire_type = WT_VARINT;
        wire_value.varint = value->enum_;
        break;
    case CPB_STRING:
        wire_type = WT_STRING;
        wire_value.string.data = value->string.str;
        wire_value.string.len = value->string.len;
        break;
    case CPB_BYTES:
        wire_type = WT_STRING;
        wire_value.string.data = value->bytes.data;
        wire_value.string.len = value->bytes.len;
        break;
    case CPB_MESSAGE:
        wire_type = WT_STRING;
        wire_value.string.data = value->message.data;
        wire_value.string.len = value->message.len;
        break;
    }
    
    /* Do not encode field key for packed repeated fields */
    if (!encoder->packed) {
        /* Override wire value if this is a packed repeated field */
        if (CPB_IS_PACKED_REPEATED(field_desc)) {
            wire_type = WT_STRING;
            wire_value.string.data = value->message.data;
            wire_value.string.len = value->message.len;
        }
        
        key = wire_type | (field_desc->number << 3);
        ret = encode_varint(&frame->buf, key);
        if (ret != CPB_ERR_OK)
            return ret;
    }
    
    switch (wire_type) {
    case WT_VARINT:
        ret = encode_varint(&frame->buf, wire_value.varint);
        if (ret != CPB_ERR_OK)
            return ret;
        break;
    case WT_64BIT:
        ret = encode_64bit(&frame->buf, wire_value.int64);
        if (ret != CPB_ERR_OK)
            return ret;
        break;
    case WT_STRING:
        ret = encode_varint(&frame->buf, wire_value.string.len);
        if (ret != CPB_ERR_OK)
            return ret;
        if (cpb_buf_left(&frame->buf) < wire_value.string.len)
            return CPB_ERR_END_OF_BUF;
        /*
         * Use memmove() when writing a message or packed repeated field as the
         * memory areas are overlapping.
         */
        if ((field_desc->opts.typ == CPB_MESSAGE) ||
            CPB_IS_PACKED_REPEATED(field_desc)) {
            memmove(frame->buf.pos, wire_value.string.data, wire_value.string.len);
        } else {
            memcpy(frame->buf.pos, wire_value.string.data, wire_value.string.len);
        }
        frame->buf.pos += wire_value.string.len;
        break;
    case WT_32BIT:
        ret = encode_32bit(&frame->buf, wire_value.int32);
        if (ret != CPB_ERR_OK)
            return ret;
        break;
    default:
        CPB_ASSERT(1, "Unknown wire type");
        break;
    }
    
    return CPB_ERR_OK;
}

/**
 * Encodes a field of type 'double'.
 * @param encoder Encoder
 * @param field_desc Field descriptor of field to encode
 * @param double_ Value
 * @return Returns CPB_ERR_OK if successful.
 */
cpb_err_t cpb_encoder_add_double(struct cpb_encoder *encoder,
                                   const struct cpb_field_desc *field_desc,
                                   double double_)
{
    union cpb_value value;
    value.double_ = double_;
    return cpb_encoder_add_field(encoder, field_desc, &value);
}

/**
 * Encodes a field of type 'float'.
 * @param encoder Encoder
 * @param field_desc Field descriptor of field to encode
 * @param float_ Value
 * @return Returns CPB_ERR_OK if successful.
 */
cpb_err_t cpb_encoder_add_float(struct cpb_encoder *encoder,
                                  const struct cpb_field_desc *field_desc,
                                  float float_)
{
    union cpb_value value;
    value.float_ = float_;
    return cpb_encoder_add_field(encoder, field_desc, &value);
}

/**
 * Encodes a field of type 'int32'.
 * @param encoder Encoder
 * @param field_desc Field descriptor of field to encode
 * @param int32 Value
 * @return Returns CPB_ERR_OK if successful.
 */
cpb_err_t cpb_encoder_add_int32(struct cpb_encoder *encoder,
                                  const struct cpb_field_desc *field_desc,
                                  s32_t int32)
{
    union cpb_value value;
    value.int32 = int32;
    return cpb_encoder_add_field(encoder, field_desc, &value);
}

/**
 * Encodes a field of type 'uint32'.
 * @param encoder Encoder
 * @param field_desc Field descriptor of field to encode
 * @param uint32 Value
 * @return Returns CPB_ERR_OK if successful.
 */
cpb_err_t cpb_encoder_add_uint32(struct cpb_encoder *encoder,
                                   const struct cpb_field_desc *field_desc,
                                   u32_t uint32)
{
    union cpb_value value;
    value.uint32 = uint32;
    return cpb_encoder_add_field(encoder, field_desc, &value);
}

/**
 * Encodes a field of type 'int64'.
 * @param encoder Encoder
 * @param field_desc Field descriptor of field to encode
 * @param int64 Value
 * @return Returns CPB_ERR_OK if successful.
 */
cpb_err_t cpb_encoder_add_int64(struct cpb_encoder *encoder,
                                  const struct cpb_field_desc *field_desc,
                                  s64_t int64)
{
    union cpb_value value;
    value.int64 = int64;
    return cpb_encoder_add_field(encoder, field_desc, &value);
}

/**
 * Encodes a field of type 'uint64'.
 * @param encoder Encoder
 * @param field_desc Field descriptor of field to encode
 * @param uint64 Value
 * @return Returns CPB_ERR_OK if successful.
 */
cpb_err_t cpb_encoder_add_uint64(struct cpb_encoder *encoder,
                                   const struct cpb_field_desc *field_desc,
                                   u64_t uint64)
{
    union cpb_value value;
    value.uint64 = uint64;
    return cpb_encoder_add_field(encoder, field_desc, &value);
}

/**
 * Encodes a field of type 'bool'.
 * @param encoder Encoder
 * @param field_desc Field descriptor of field to encode
 * @param bool Value
 * @return Returns CPB_ERR_OK if successful.
 */
cpb_err_t cpb_encoder_add_bool(struct cpb_encoder *encoder,
                                 const struct cpb_field_desc *field_desc,
                                 cpb_bool_t bool)
{
    union cpb_value value;
    value.bool = bool;
    return cpb_encoder_add_field(encoder, field_desc, &value);
}

/**
 * Encodes a field of type 'enum'.
 * @param encoder Encoder
 * @param field_desc Field descriptor of field to encode
 * @param enum_ Value
 * @return Returns CPB_ERR_OK if successful.
 */
cpb_err_t cpb_encoder_add_enum(struct cpb_encoder *encoder,
                                 const struct cpb_field_desc *field_desc,
                                 cpb_enum_t enum_)
{
    union cpb_value value;
    value.enum_ = enum_;
    return cpb_encoder_add_field(encoder, field_desc, &value);
}

/**
 * Encodes a field of type 'string'.
 * @note The string must be null-termiated.
 * @param encoder Encoder
 * @param field_desc Field descriptor of field to encode
 * @param str Value
 * @return Returns CPB_ERR_OK if successful.
 */
cpb_err_t cpb_encoder_add_string(struct cpb_encoder *encoder,
                                   const struct cpb_field_desc *field_desc,
                                   char *str)
{
    union cpb_value value;
    value.string.str = str;
    value.string.len = strlen(str);
    return cpb_encoder_add_field(encoder, field_desc, &value);
}

/**
 * Encodes a field of type 'bytes'.
 * @param encoder Encoder
 * @param field_desc Field descriptor of field to encode
 * @param data Bytes to encode
 * @param len Number of bytes to encode
 * @return Returns CPB_ERR_OK if successful.
 */
cpb_err_t cpb_encoder_add_bytes(struct cpb_encoder *encoder,
                                  const struct cpb_field_desc *field_desc,
                                  u8_t *data, size_t len)
{
    union cpb_value value;
    value.string.str = (char *) data;
    value.string.len = len;
    return cpb_encoder_add_field(encoder, field_desc, &value);
}
