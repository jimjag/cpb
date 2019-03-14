// Generated by the protocol buffer compiler.  DO NOT EDIT!

#include <cpb/cpb.h>

#include "test_struct_map_pb2.h"

/* 'StructTest' field descriptors */
const struct cpb_field_desc cpb_fields_test_structtest[] = {
    {
        .number = 1,
        .opts.label = CPB_REQUIRED,
        .opts.typ = CPB_INT32,
        .opts.flags = 0,
        .msg_desc = 0,
#if CPB_FIELD_NAMES
        .name = "field_int32",
#endif
#if CPB_FIELD_DEFAULTS
        .def.int32 = 0,
#endif
    },
    {
        .number = 2,
        .opts.label = CPB_REQUIRED,
        .opts.typ = CPB_INT64,
        .opts.flags = 0,
        .msg_desc = 0,
#if CPB_FIELD_NAMES
        .name = "field_int64",
#endif
#if CPB_FIELD_DEFAULTS
        .def.int64 = 0,
#endif
    },
    {
        .number = 3,
        .opts.label = CPB_REQUIRED,
        .opts.typ = CPB_BOOL,
        .opts.flags = 0,
        .msg_desc = 0,
#if CPB_FIELD_NAMES
        .name = "field_bool",
#endif
#if CPB_FIELD_DEFAULTS
        1,
#endif
    },
    {
        .number = 4,
        .opts.label = CPB_REQUIRED,
        .opts.typ = CPB_ENUM,
        .opts.flags = 0,
        .msg_desc = 0,
#if CPB_FIELD_NAMES
        .name = "field_enum",
#endif
#if CPB_FIELD_DEFAULTS
        .def.int32 = 1,
#endif
    },
    {
        .number = 5,
        .opts.label = CPB_REQUIRED,
        .opts.typ = CPB_STRING,
        .opts.flags = 0,
        .msg_desc = 0,
#if CPB_FIELD_NAMES
        .name = "field_string",
#endif
#if CPB_FIELD_DEFAULTS
        .def.string = "",
#endif
    },
    {
        .number = 6,
        .opts.label = CPB_REQUIRED,
        .opts.typ = CPB_BYTES,
        .opts.flags = 0,
        .msg_desc = 0,
#if CPB_FIELD_NAMES
        .name = "field_bytes",
#endif
#if CPB_FIELD_DEFAULTS
        .def.string = "",
#endif
    },
    {
        .number = 7,
        .opts.label = CPB_REQUIRED,
        .opts.typ = CPB_MESSAGE,
        .opts.flags = 0,
        .msg_desc = test_StructTest_Nested1,
#if CPB_FIELD_NAMES
        .name = "nested1",
#endif
#if CPB_FIELD_DEFAULTS
        .def.null = 0,
#endif
    },
    {
        .number = 8,
        .opts.label = CPB_REPEATED,
        .opts.typ = CPB_MESSAGE,
        .opts.flags = 0,
        .msg_desc = test_StructTest_Nested2,
#if CPB_FIELD_NAMES
        .name = "nested2",
#endif
#if CPB_FIELD_DEFAULTS
        .def.null = 0,
#endif
    },
};

/* 'Nested1' field descriptors */
const struct cpb_field_desc cpb_fields_test_structtest_nested1[] = {
    {
        .number = 1,
        .opts.label = CPB_REQUIRED,
        .opts.typ = CPB_INT32,
        .opts.flags = 0,
        .msg_desc = 0,
#if CPB_FIELD_NAMES
        .name = "field_int32",
#endif
#if CPB_FIELD_DEFAULTS
        .def.int32 = 0,
#endif
    },
    {
        .number = 2,
        .opts.label = CPB_REQUIRED,
        .opts.typ = CPB_INT64,
        .opts.flags = 0,
        .msg_desc = 0,
#if CPB_FIELD_NAMES
        .name = "field_int64",
#endif
#if CPB_FIELD_DEFAULTS
        .def.int64 = 0,
#endif
    },
};

/* 'Nested2' field descriptors */
const struct cpb_field_desc cpb_fields_test_structtest_nested2[] = {
    {
        .number = 1,
        .opts.label = CPB_REQUIRED,
        .opts.typ = CPB_STRING,
        .opts.flags = 0,
        .msg_desc = 0,
#if CPB_FIELD_NAMES
        .name = "field_string",
#endif
#if CPB_FIELD_DEFAULTS
        .def.string = "",
#endif
    },
};

/* Message descriptors */
const struct cpb_msg_desc cpb_messages_test[] = {
    {
        .num_fields = 8,
        .fields = cpb_fields_test_structtest,
#if CPB_MESSAGE_NAMES
        .name = "StructTest",
#endif
    },
    {
        .num_fields = 2,
        .fields = cpb_fields_test_structtest_nested1,
#if CPB_MESSAGE_NAMES
        .name = "Nested1",
#endif
    },
    {
        .num_fields = 1,
        .fields = cpb_fields_test_structtest_nested2,
#if CPB_MESSAGE_NAMES
        .name = "Nested2",
#endif
    },
};

/* Service descriptors */
const struct cpb_service_desc cpb_services_test[] = {
};

