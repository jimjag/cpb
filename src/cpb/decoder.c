/** @file decoder.c
 * 
 * Implementation of the protocol buffers decoder.
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


/* Debug handlers */

static int debug_indent;

static void debug_print_indent(void)
{
    int i;
    
    for (i = 0; i < debug_indent; i++)
        printf("  ");
}

static void debug_msg_start_handler(struct cpb_decoder *decoder,
                                    const struct cpb_msg_desc *msg_desc,
                                    void *arg)
{
    const char *name;

#if CPB_MESSAGE_NAMES
    name = msg_desc->name;
#else
    name = "<message>";
#endif
    
    debug_print_indent();
    printf("%s:\n", name);
    debug_indent++;
}

static void debug_msg_end_handler(struct cpb_decoder *decoder,
                                  const struct cpb_msg_desc *msg_desc,
                                  void *arg)
{
    debug_indent--;
}

static void debug_field_handler(struct cpb_decoder *decoder,
                                const struct cpb_msg_desc *msg_desc,
                                const struct cpb_field_desc *field_desc,
                                union cpb_value *value, void *arg)
{
    static char *typ_names[] = {
        "(double)",
        "(float)",
        "(int32)",
        "(int64)",
        "(uint32)",
        "(uint64)",
        "(sint32)",
        "(sint64)",
        "(fixed32)",
        "(fixed64)",
        "(sfixed32)",
        "(sfixed64)",
        "(bool)",
        "(enum)",
        "(string)",
        "(bytes)",
        "(message)",
    };
    
    const char *name;
    
#if CPB_FIELD_NAMES
    name = field_desc->name;
#else
    name = "<field>";
#endif
    
    debug_print_indent();
    printf("%-20s %-10s = ", name, typ_names[field_desc->opts.typ]);
    
    switch (field_desc->opts.typ) {
    case CPB_DOUBLE:
        printf("%f", value->double_);
        break;
    case CPB_FLOAT:
        printf("%f", value->float_);
        break;
    case CPB_INT32:
    case CPB_SINT32:
    case CPB_SFIXED32:
        printf("%d", value->int32);
        break;
    case CPB_INT64:
    case CPB_SINT64:
    case CPB_SFIXED64:
        printf("%lld", value->int64);
        break;
    case CPB_UINT32:
    case CPB_FIXED32:
        printf("%u", value->int32);
        break;
    case CPB_UINT64:
    case CPB_FIXED64:
        printf("%llu", value->int64);
        break;
    case CPB_BOOL:
        printf("%s", value->bool ? "true" : "false");
        break;
    case CPB_ENUM:
        printf("%d", value->enum_);
        break;
    case CPB_STRING:
        while (value->string.len--)
            printf("%c", *value->string.str++);
        break;
    case CPB_BYTES:
        while (value->bytes.len--)
            printf("%02x ", *value->bytes.data++);
        break;
    default:
        break;
    }
    
    printf("\n");
}

/* Decoder utilities */

/**
 * Decodes a variable integer in base-128 format.
 * See http://code.google.com/apis/protocolbuffers/docs/encoding.html for more
 * information.
 * @param buf Memory buffer
 * @param varint Buffer to decode into
 * @return Returns CPB_ERR_OK if successful or CPB_ERR_END_OF_BUF if there
 * were not enough bytes in the memory buffer. 
 */
cpb_err_t cpb_decode_varint(struct cpb_buf *buf, u64_t *varint)
{
    int bitpos;
    
    *varint = 0;
    for (bitpos = 0; *buf->pos & 0x80 && bitpos < 64; bitpos += 7, buf->pos++) {
        *varint |= (u64_t) (*buf->pos & 0x7f) << bitpos;
        if (buf->end - buf->pos < 2)
            return CPB_ERR_END_OF_BUF;
    }
    *varint |= (u64_t) (*buf->pos & 0x7f) << bitpos;
    buf->pos++;
    
    return CPB_ERR_OK;
}

/**
 * Decodes a 32 bit integer
 * @param buf Memory buffer
 * @param value Buffer to decode into
 * @return Returns CPB_ERR_OK if successful or CPB_ERR_END_OF_BUF if there
 * were not enough bytes in the memory buffer. 
 */
cpb_err_t cpb_decode_32bit(struct cpb_buf *buf, u32_t *value)
{
    if (cpb_buf_left(buf) < 4)
        return CPB_ERR_END_OF_BUF;

    *value = buf->pos[0] | (buf->pos[1] << 8) |
             (buf->pos[2] << 16) | (buf->pos[3] << 24);
    buf->pos += 4;
    
    return CPB_ERR_OK;
}

/**
 * Decodes a 64 bit integer
 * @param buf Memory buffer
 * @param value Buffer to decode into
 * @return Returns CPB_ERR_OK if successful or CPB_ERR_END_OF_BUF if there
 * were not enough bytes in the memory buffer. 
 */
cpb_err_t cpb_decode_64bit(struct cpb_buf *buf, u64_t *value)
{
    int i;
    
    if (cpb_buf_left(buf) < 8)
        return CPB_ERR_END_OF_BUF;
    
    *value = 0;
    for (i = 7; i >= 0; i--)
        *value = (*value << 8) | buf->pos[i];
    buf->pos += 8;
    
    return CPB_ERR_OK;
}

static enum wire_type field_wire_type(const struct cpb_field_desc *field_desc)
{
    switch (field_desc->opts.typ) {
    case CPB_DOUBLE:
        return WT_64BIT;
    case CPB_FLOAT:
        return WT_32BIT;
    case CPB_INT32:
    case CPB_INT64:
    case CPB_UINT32:
    case CPB_UINT64:
    case CPB_SINT32:
    case CPB_SINT64:
        return WT_VARINT;
    case CPB_FIXED32:
        return WT_32BIT;
    case CPB_FIXED64:
        return WT_64BIT;
    case CPB_SFIXED32:
        return WT_32BIT;
    case CPB_SFIXED64:
        return WT_64BIT;
    case CPB_BOOL:
    case CPB_ENUM:
        return WT_VARINT;
    case CPB_STRING:
    case CPB_BYTES:
    case CPB_MESSAGE:
        return WT_STRING;
    default:
        return WT_ERROR;
    }
}

/**
 * Pushes the decoder stack.
 * @param decoder Dncoder
 * @return Returns the top stack frame.
 */
static struct cpb_decoder_stack_frame *push_stack_frame(struct cpb_decoder *decoder)
{
    decoder->depth++;
    CPB_ASSERT(decoder->depth <= CPB_MAX_DEPTH, "Message nesting too deep");
    return &decoder->stack[decoder->depth - 1];
}

/* Decoder */

/**
 * Initializes the decoder.
 * @param decoder Decoder
 */
void cpb_decoder_init(struct cpb_decoder *decoder)
{
    decoder->arg = NULL;
    decoder->msg_start_handler = NULL;
    decoder->msg_end_handler = NULL;
    decoder->field_handler = NULL;
}

/**
 * Sets the user argument to be passed back with the handlers.
 * @param decoder Decoder
 * @param arg User argument
 */
void cpb_decoder_arg(struct cpb_decoder *decoder, void *arg)
{
    decoder->arg = arg;
}

/**
 * Sets the message start and end handlers.
 * @param decoder Decoder
 * @param msg_start_handler Message start handler
 * @param msg_end_handler Message end handler
 */
void cpb_decoder_msg_handler(struct cpb_decoder *decoder,
                            cpb_decoder_msg_start_handler_t msg_start_handler,
                            cpb_decoder_msg_end_handler_t msg_end_handler)
{
    decoder->msg_start_handler = msg_start_handler;
    decoder->msg_end_handler = msg_end_handler;
}

/**
 * Sets the field handler.
 * @param decoder Decoder
 * @param field_handler Field handler
 */
void cpb_decoder_field_handler(struct cpb_decoder *decoder,
                              cpb_decoder_field_handler_t field_handler)
{
    decoder->field_handler = field_handler;
}

/**
 * Setups the decoder to use the verbose debug handlers which output the
 * message contents to the console.
 * @param decoder Decoder 
 */
void cpb_decoder_use_debug_handlers(struct cpb_decoder *decoder)
{
    cpb_decoder_msg_handler(decoder, debug_msg_start_handler,
                             debug_msg_end_handler);
    cpb_decoder_field_handler(decoder, debug_field_handler);
}

/**
 * Decodes a protocol buffer.
 * @param decoder Decoder
 * @param msg_desc Root message descriptor of the protocol buffer
 * @param data Data to decode
 * @param len Length of data to decode
 * @param used Returns the number of decoded bytes when not NULL.
 * @return Returns CPB_ERR_OK when data was successfully decoded.
 */
cpb_err_t cpb_decoder_decode(struct cpb_decoder *decoder,
                               const struct cpb_msg_desc *msg_desc,
                               void *data, size_t len, size_t *used)
{
    cpb_err_t ret;
    int i;
    u64_t key;
    int number;
    const struct cpb_field_desc *field_desc = NULL;
    enum wire_type wire_type;
    union wire_value wire_value;
    union cpb_value value;
    struct cpb_decoder_stack_frame *frame, *new_frame;
    
    /* Setup initial stack frame */
    decoder->depth = 1;
    decoder->packed = 0;
    frame = &decoder->stack[decoder->depth - 1];
    cpb_buf_init(&frame->buf, data, len);
    frame->msg_desc = msg_desc;
    
    while (decoder->depth >= 1) {
decode_nested:
        
        /* Get current frame */
        frame = &decoder->stack[decoder->depth - 1];
        
        /* Notify start message */
        if (frame->msg_desc && cpb_buf_used(&frame->buf) == 0)
            if (decoder->msg_start_handler)
                decoder->msg_start_handler(decoder, frame->msg_desc, decoder->arg);

        /* Process buffer */
        while (cpb_buf_left(&frame->buf) > 0) {
            
            if (decoder->packed) {
                wire_type = field_wire_type(field_desc);
            } else {
                /* Decode the field key */
                ret = cpb_decode_varint(&frame->buf, &key);
                if (ret != CPB_ERR_OK)
                    return ret;
            
                number = key >> 3;
                wire_type = key & 0x07;
            
                /* Find the field descriptor */
                for (i = 0; i < frame->msg_desc->num_fields; i++)
                    if (frame->msg_desc->fields[i].number == number) {
                        field_desc = &frame->msg_desc->fields[i];
                        break;
                    }
            }
            
            /* Decode field's wire value */
            switch(wire_type) {
            case WT_VARINT:
                ret = cpb_decode_varint(&frame->buf, &wire_value.varint);
                if (ret != CPB_ERR_OK)
                    return ret;
                break;
            case WT_64BIT:
                ret = cpb_decode_64bit(&frame->buf, &wire_value.int64);
                if (ret != CPB_ERR_OK)
                    return ret;
                break;
            case WT_STRING:
                ret = cpb_decode_varint(&frame->buf, &wire_value.string.len);
                if (ret != CPB_ERR_OK)
                    return ret;
                if (wire_value.string.len > cpb_buf_left(&frame->buf))
                    return CPB_ERR_END_OF_BUF;
                wire_value.string.data = frame->buf.pos;
                frame->buf.pos += wire_value.string.len;
                break;
            case WT_32BIT:
                ret = cpb_decode_32bit(&frame->buf, &wire_value.int32);
                if (ret != CPB_ERR_OK)
                    return ret;
                break;
            default:
                CPB_ASSERT(1, "Unknown wire type");
                break;
            }
            
            /* Skip unknown fields */
            if (!field_desc)
                continue;
            
            /* Handle packed repeated fields */
            if ((wire_type == WT_STRING) &&
                CPB_IS_PACKED_REPEATED(field_desc)) {
                
                /* Create new stack frame */
                new_frame = push_stack_frame(decoder);
                cpb_buf_init(&new_frame->buf, wire_value.string.data, wire_value.string.len);
                new_frame->msg_desc = frame->msg_desc;
                
                /* Enter packed repeated mode */
                decoder->packed = 1;
                
                goto decode_nested;
            }
            
            switch (field_desc->opts.typ) {
            case CPB_DOUBLE:
                memcpy(&value.double_, &wire_value.int64, sizeof(double));
                break;
            case CPB_FLOAT:
                memcpy(&value.float_, &wire_value.int32, sizeof(float));
                break;
            case CPB_INT32:
                value.int32 = wire_value.varint;
                break;
            case CPB_INT64:
                value.int64 = wire_value.varint;
                break;
            case CPB_UINT32:
                value.uint32 = wire_value.varint;
                break;
            case CPB_UINT64:
                value.uint64 = wire_value.varint;
                break;
            case CPB_SINT32:
                /* Zig-zag encoding */
                value.int32 = (wire_value.varint >> 1) ^ -((s32_t) (wire_value.varint & 1));
                break;
            case CPB_SINT64:
                /* Zig-zag encoding */
                value.int64 = (wire_value.varint >> 1) ^ -((s64_t) (wire_value.varint & 1));
                break;
            case CPB_FIXED32:
                value.uint32 = wire_value.int32;
                break;
            case CPB_FIXED64:
                value.uint64 = wire_value.int64;
                break;
            case CPB_SFIXED32:
                value.int32 = wire_value.int32;
                break;
            case CPB_SFIXED64:
                value.int64 = wire_value.int64;
                break;
            case CPB_BOOL:
                value.bool = wire_value.varint;
                break;
            case CPB_ENUM:
                value.enum_ = wire_value.varint;
                break;
            case CPB_STRING:
                value.string.len = wire_value.string.len;
                value.string.str = wire_value.string.data;
                break;
            case CPB_BYTES:
                value.bytes.len = wire_value.string.len;
                value.bytes.data = wire_value.string.data;
                break;
            case CPB_MESSAGE:
            default:
                if (decoder->field_handler)
                    decoder->field_handler(decoder, msg_desc, field_desc, NULL, decoder->arg);
                
                /* Create new stack frame */
                new_frame = push_stack_frame(decoder);
                cpb_buf_init(&new_frame->buf, wire_value.string.data, wire_value.string.len);
                new_frame->msg_desc = field_desc->msg_desc;
                
                goto decode_nested;
            }
            
            if (decoder->field_handler)
                decoder->field_handler(decoder, frame->msg_desc, field_desc, &value, decoder->arg);
        }
        
        /* Notify end message */
        if (frame->msg_desc)
            if (decoder->msg_end_handler)
                decoder->msg_end_handler(decoder, frame->msg_desc, decoder->arg);
        
        /* Pop the stack */
        decoder->depth--;
        
        /* Leave packed repeated mode */
        decoder->packed = 0;
    }
    
    if (used)
        *used = cpb_buf_used(&decoder->stack[0].buf);
    
    return CPB_ERR_OK;
}
