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
#include <cpb/core/encoder2.h>

#include "private.h"


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
size_t cpb_encode_varint(u8_t *buf, u64_t varint)
{
    size_t len = 0;

    do {
        if (buf) {
            if (varint > 127) {
                *buf++ = 0x80 | (varint & 0x7F);
            } else {
                *buf++ = (varint & 0x7F);
            }
        }
        varint >>= 7;
        len++;
    } while (varint);

    return len;
}

/**
 * Encodes a 32 bit integer.
 * @param buf Memory buffer
 * @param value Value to encode
 * @return Returns CPB_ERR_OK if successful or CPB_ERR_END_OF_BUF if there
 * was not enough space left in the memory buffer.
 */
size_t cpb_encode_32bit(u8_t *buf, u32_t value)
{
    size_t len = 4;

    if (buf) {
        *buf++ = (value) & 0xff;
        *buf++ = (value >> 8) & 0xff;
        *buf++ = (value >> 16) & 0xff;
        *buf++ = (value >> 24) & 0xff;
    }

    return len;
}

/**
 * Encodes a 64 bit integer.
 * @param buf Memory buffer
 * @param value Value to encode
 * @return Returns CPB_ERR_OK if successful or CPB_ERR_END_OF_BUF if there
 * was not enough space left in the memory buffer.
 */
size_t cpb_encode_64bit(u8_t *buf, u64_t value)
{
    size_t len = 8;

    if (buf) {
        *buf++ = (value) & 0xff;
        *buf++ = (value >> 8) & 0xff;
        *buf++ = (value >> 16) & 0xff;
        *buf++ = (value >> 24) & 0xff;
        value >>= 32;
        *buf++ = (value) & 0xff;
        *buf++ = (value >> 8) & 0xff;
        *buf++ = (value >> 16) & 0xff;
        *buf++ = (value >> 24) & 0xff;
    }

    return len;
}

/* Encoder */

/**
 * Initializes the encoder.
 * @param encoder Encoder
 */
void cpb_encoder2_init(struct cpb_encoder2 *encoder)
{
}

/**
 * Starts encoding a message.
 * @param encoder Encoder
 * @param msg_desc Root message descriptor
 * @param data Data buffer to encode into
 * @param len Length of data buffer
 */
void cpb_encoder2_start(struct cpb_encoder2 *encoder,
                         const struct cpb_msg_desc *msg_desc)
{
    encoder->packed = 0;
}

/**
 * Starts encoding a packed repeated field.
 * @param encoder Encoder
 * @param field_desc Field descriptor of packed repeated field
 * @return Returns CPB_ERR_OK if successful.
 */
cpb_err_t cpb_encoder2_packed_repeated_start(struct cpb_encoder2 *encoder,
                                               const struct cpb_field_desc *field_desc)
{
    CPB_ASSERT(CPB_IS_PACKED_REPEATED(field_desc), "Field is not repeated packed");
    CPB_ASSERT(!encoder->packed, "Packed repeated fields must not be nested");
    encoder->packed = 1;
    return CPB_ERR_OK;
}

/**
 * Ends encoding a packed repeated field.
 * @param encoder Encoder
 * @return Returns CPB_ERR_OK if successful.
 */
cpb_err_t cpb_encoder2_packed_repeated_end(struct cpb_encoder2 *encoder)
{
    CPB_ASSERT(encoder->packed, "Not in packed repeated mode");
    encoder->packed = 0;
    return CPB_ERR_OK;
}

/**
 * Encodes a field.
 * @note This method should not normally be used. Use the cpb_encoder2_add_xxx()
 * methods to directly add a field of a given type.
 * @param encoder Encoder
 * @param field_desc Field descriptor of field to encode
 * @param value Field value
 * @return Returns CPB_ERR_OK if successful.
 */
size_t cpb_encoder2_add_field(struct cpb_encoder2 *encoder,
                               const struct cpb_field_desc *field_desc,
                               union cpb_value *value,
                               u8_t* buf)
{
    size_t len = 0;
    size_t size = 0;
    u64_t key;
    enum wire_type wire_type = 0;
    union wire_value wire_value;

/* TODO ASSERT that we are not repeat packing a non-numeric type */

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

    /* Don't output wire key in packed mode */
    if (!encoder->packed) {
        /* Override wire value if this is a packed repeated field */
        if (CPB_IS_PACKED_REPEATED(field_desc)) {
            wire_type = WT_STRING;
            wire_value.string.data = value->message.data;
            wire_value.string.len = value->message.len;
        }

        key = wire_type | (field_desc->number << 3);
        size = cpb_encode_varint(buf, key);
        len += size;
        if (buf) buf += size;
    }

    switch (wire_type) {
    case WT_VARINT:
        size = cpb_encode_varint(buf, wire_value.varint);
        len += size;
        if (buf) buf += size;
        break;
    case WT_64BIT:
        size = cpb_encode_64bit(buf, wire_value.int64);
        len += size;
        if (buf) buf += size;
        break;
    case WT_STRING:
        size = cpb_encode_varint(buf, wire_value.string.len);
        len += size;
        if (buf) {
            buf += size;
            /*
             * Use memmove() when writing a message or packed repeated field
             * as the memory areas are overlapping.
             */
            if ((field_desc->opts.typ == CPB_MESSAGE) ||
                CPB_IS_PACKED_REPEATED(field_desc)) {
                memmove(buf, wire_value.string.data, wire_value.string.len);
            } else {
                memcpy(buf, wire_value.string.data, wire_value.string.len);
            }
        }
        size = wire_value.string.len;
        len += size;
        if (buf) buf += size;
        break;
    case WT_32BIT:
        size = cpb_encode_32bit(buf, wire_value.int32);
        len += size;
        if (buf) buf += size;
        break;
    default:
        CPB_ASSERT(1, "Unknown wire type");
        break;
    }

    return len;
}

