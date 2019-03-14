/** @file types.h
 *
 * Public types and definitions needed by the library.
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

#ifndef __CPB_CORE_TYPES_H__
#define __CPB_CORE_TYPES_H__

#include <cpb/cpb.h>

/* Maximum depth of message embedding */
#ifndef CPB_MAX_DEPTH
#define CPB_MAX_DEPTH 32
#endif

/* Maximum number of required fields in a message */
#ifndef CPB_MAX_REQUIRED_FIELDS
#define CPB_MAX_REQUIRED_FIELDS 16
#endif

/* Provide field names as strings */
#ifndef CPB_FIELD_NAMES
#define CPB_FIELD_NAMES 1
#endif

/* Provide field default values */
#ifndef CPB_FIELD_DEFAULTS
#define CPB_FIELD_DEFAULTS 1
#endif

/* Provide message names as strings */
#ifndef CPB_MESSAGE_NAMES
#define CPB_MESSAGE_NAMES 1
#endif

/* Provide method names as strings */
#ifndef CPB_METHOD_NAMES
#define CPB_METHOD_NAMES 1
#endif

/* Provide service names as strings */
#ifndef CPB_SERVICE_NAMES
#define CPB_SERVICE_NAMES 1
#endif

/** Protocol buffer error codes */
typedef enum {
    CPB_ERR_OK,                /**< OK */
    CPB_ERR_CANCEL,            /**< Cancelled by user */
    CPB_ERR_UNKNOWN_FIELD,     /**< User tried to encode an unknown field */
    CPB_ERR_INVALID_FIELD,     /**< Invalid field in current context */
    CPB_ERR_END_OF_BUF,        /**< End of buffer reached */
    CPB_ERR_MEM,               /**< Memory allocation failed */
    /* Socket service error codes */
    CPB_ERR_NET_INIT,          /**< Network initialization failed */
} cpb_err_t;

/* Field labels */
#define CPB_OPTIONAL       1
#define CPB_REQUIRED       2
#define CPB_REPEATED       3

/* Field value types */
#define CPB_DOUBLE         1
#define CPB_FLOAT          2
#define CPB_INT64          3
#define CPB_UINT64         4
#define CPB_INT32          5
#define CPB_FIXED64        6
#define CPB_FIXED32        7
#define CPB_BOOL           8
#define CPB_STRING         9
#define CPB_GROUP          10
#define CPB_MESSAGE        11
#define CPB_BYTES          12
#define CPB_UINT32         13
#define CPB_ENUM           14
#define CPB_SFIXED32       15
#define CPB_SFIXED64       16
#define CPB_SINT32         17
#define CPB_SINT64         18

/* Field flags */
#define CPB_HAS_DEFAULT    (1 << 0)
#define CPB_IS_PACKED      (1 << 1)
#define CPB_IS_DEPRECATED  (1 << 2)

/** Protocol buffer field options */
typedef struct {
    unsigned int label : 2;
    unsigned int typ : 6;
    unsigned int flags : 8;
} cpb_field_opts_t;

/** Protocol buffer bool type */
typedef int cpb_bool_t;

/** Protocol buffer enum type */
typedef int cpb_enum_t;

/** Protocol buffer value */
union cpb_value {
    double double_;
    float float_;
    s32_t int32;
    s64_t int64;
    u32_t uint32;
    u64_t uint64;
    cpb_bool_t bool;
    struct {
        char *str;
        size_t len;
    } string;
    struct {
        u8_t *data;
        size_t len;
    } bytes;
    struct {
        void *data;
        size_t len;
    } message;
    cpb_enum_t enum_;
    int null;
};

/* Forward declaration */
struct cpb_msg_desc;

/** Protocol buffer field descriptor */
struct cpb_field_desc {
    u32_t number;               /**< Field number */
    cpb_field_opts_t opts;     /**< Field options (label, value type, flags) */
    const struct cpb_msg_desc *msg_desc; /**< Message descriptor, if field is message */
#if CPB_FIELD_NAMES
    const char *name;           /**< Field name */
#endif
#if CPB_FIELD_DEFAULTS
    union cpb_value def;       /**< Field default value */
#endif
};

/** Checks if a field is 'packed repeated' */
#define CPB_IS_PACKED_REPEATED(field_desc)                                 \
    ((field_desc)->opts.label == CPB_REPEATED &&                           \
     (field_desc)->opts.flags & CPB_IS_PACKED)

/** Protocol buffer message descriptor */
struct cpb_msg_desc {
    u32_t num_fields;           /**< Number of fields */
    const struct cpb_field_desc *fields; /**< Array of field descriptors */
#if CPB_MESSAGE_NAMES
    const char *name;
#endif
};

/* Forward declaration */
struct cpb_service_desc;

/** Protocol buffer method descriptor */
struct cpb_method_desc {
    const struct cpb_service_desc *service; /**< Service descriptor */
    const struct cpb_msg_desc *req_desc; /**< Request message descriptor */
    const struct cpb_msg_desc *res_desc; /**< Response message descriptor */
#if CPB_METHOD_NAMES
    const char *name;           /**< Method name */
#endif
};

/** Protocol buffer service descriptor */
struct cpb_service_desc {
    const u32_t num_methods;    /**< Number of methods */
    const struct cpb_method_desc *methods; /**< Array of method descriptors */
#if CPB_SERVICE_NAMES
    const char *name;           /**< Service name */
#endif
};

/** RPC result codes. */
typedef enum {
    CPB_RPC_OK,                /**< Call was successfully processed on the server */
    CPB_RPC_NOT_CONNECTED,     /**< Not connected to the server */
    CPB_RPC_FAILED,            /**< Call failed to execute on the server */
} cpb_rpc_result_t;

/** Simple memory buffer */
struct cpb_buf {
    u8_t *base;     /**< Buffers base address */
    u8_t *pos;      /**< Buffers current position */
    u8_t *end;      /**< Buffers end address (first invalid byte) */
};

#endif /* __CPB_CORE_TYPES_H__ */
