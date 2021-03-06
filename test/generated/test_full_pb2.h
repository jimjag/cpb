// Generated by the protocol buffer compiler.  DO NOT EDIT!

#ifndef __TEST_FULL_PB2_H__
#define __TEST_FULL_PB2_H__

#include <cpb/cpb.h>


/* 'TestEnumSmall' enumeration values */
#define FOO_VALUE 0
#define FOO_OTHER_VALUE 1

/* 'TestEnum' enumeration values */
#define FOO_VALUE0 0
#define FOO_VALUE1 1
#define FOO_VALUE127 127
#define FOO_VALUE128 128
#define FOO_VALUE16383 16383
#define FOO_VALUE16384 16384
#define FOO_VALUE2097151 2097151
#define FOO_VALUE2097152 2097152
#define FOO_VALUE268435455 268435455
#define FOO_VALUE268435456 268435456

/* 'TestEnumDupValues' enumeration values */
#define FOO_VALUE_A 42
#define FOO_VALUE_B 42
#define FOO_VALUE_C 42
#define FOO_VALUE_D 666
#define FOO_VALUE_E 666
#define FOO_VALUE_F 1000
#define FOO_VALUE_AA 1000
#define FOO_VALUE_BB 1001

extern const struct cpb_msg_desc cpb_messages_foo[];

/* Message descriptor pointers */
#define foo_SubMess (&cpb_messages_foo[0])
#define foo_TestFieldNo15 (&cpb_messages_foo[1])
#define foo_TestFieldNo16 (&cpb_messages_foo[2])
#define foo_TestFieldNo2047 (&cpb_messages_foo[3])
#define foo_TestFieldNo2048 (&cpb_messages_foo[4])
#define foo_TestFieldNo262143 (&cpb_messages_foo[5])
#define foo_TestFieldNo262144 (&cpb_messages_foo[6])
#define foo_TestFieldNo33554431 (&cpb_messages_foo[7])
#define foo_TestFieldNo33554432 (&cpb_messages_foo[8])
#define foo_TestMess (&cpb_messages_foo[9])
#define foo_TestMessPacked (&cpb_messages_foo[10])
#define foo_TestMessOptional (&cpb_messages_foo[11])
#define foo_TestMessRequiredInt32 (&cpb_messages_foo[12])
#define foo_TestMessRequiredSInt32 (&cpb_messages_foo[13])
#define foo_TestMessRequiredSFixed32 (&cpb_messages_foo[14])
#define foo_TestMessRequiredInt64 (&cpb_messages_foo[15])
#define foo_TestMessRequiredSInt64 (&cpb_messages_foo[16])
#define foo_TestMessRequiredSFixed64 (&cpb_messages_foo[17])
#define foo_TestMessRequiredUInt32 (&cpb_messages_foo[18])
#define foo_TestMessRequiredFixed32 (&cpb_messages_foo[19])
#define foo_TestMessRequiredUInt64 (&cpb_messages_foo[20])
#define foo_TestMessRequiredFixed64 (&cpb_messages_foo[21])
#define foo_TestMessRequiredFloat (&cpb_messages_foo[22])
#define foo_TestMessRequiredDouble (&cpb_messages_foo[23])
#define foo_TestMessRequiredBool (&cpb_messages_foo[24])
#define foo_TestMessRequiredEnum (&cpb_messages_foo[25])
#define foo_TestMessRequiredEnumSmall (&cpb_messages_foo[26])
#define foo_TestMessRequiredString (&cpb_messages_foo[27])
#define foo_TestMessRequiredBytes (&cpb_messages_foo[28])
#define foo_TestMessRequiredMessage (&cpb_messages_foo[29])
#define foo_EmptyMess (&cpb_messages_foo[30])
#define foo_DefaultRequiredValues (&cpb_messages_foo[31])
#define foo_DefaultOptionalValues (&cpb_messages_foo[32])
#define foo_AllocValues (&cpb_messages_foo[33])

extern const struct cpb_field_desc cpb_fields_foo_submess[];

/* 'SubMess' field descriptor pointers */
#define foo_SubMess_test (&cpb_fields_foo_submess[0])

extern const struct cpb_field_desc cpb_fields_foo_testfieldno15[];

/* 'TestFieldNo15' field descriptor pointers */
#define foo_TestFieldNo15_test (&cpb_fields_foo_testfieldno15[0])

extern const struct cpb_field_desc cpb_fields_foo_testfieldno16[];

/* 'TestFieldNo16' field descriptor pointers */
#define foo_TestFieldNo16_test (&cpb_fields_foo_testfieldno16[0])

extern const struct cpb_field_desc cpb_fields_foo_testfieldno2047[];

/* 'TestFieldNo2047' field descriptor pointers */
#define foo_TestFieldNo2047_test (&cpb_fields_foo_testfieldno2047[0])

extern const struct cpb_field_desc cpb_fields_foo_testfieldno2048[];

/* 'TestFieldNo2048' field descriptor pointers */
#define foo_TestFieldNo2048_test (&cpb_fields_foo_testfieldno2048[0])

extern const struct cpb_field_desc cpb_fields_foo_testfieldno262143[];

/* 'TestFieldNo262143' field descriptor pointers */
#define foo_TestFieldNo262143_test (&cpb_fields_foo_testfieldno262143[0])

extern const struct cpb_field_desc cpb_fields_foo_testfieldno262144[];

/* 'TestFieldNo262144' field descriptor pointers */
#define foo_TestFieldNo262144_test (&cpb_fields_foo_testfieldno262144[0])

extern const struct cpb_field_desc cpb_fields_foo_testfieldno33554431[];

/* 'TestFieldNo33554431' field descriptor pointers */
#define foo_TestFieldNo33554431_test (&cpb_fields_foo_testfieldno33554431[0])

extern const struct cpb_field_desc cpb_fields_foo_testfieldno33554432[];

/* 'TestFieldNo33554432' field descriptor pointers */
#define foo_TestFieldNo33554432_test (&cpb_fields_foo_testfieldno33554432[0])

extern const struct cpb_field_desc cpb_fields_foo_testmess[];

/* 'TestMess' field descriptor pointers */
#define foo_TestMess_test_int32 (&cpb_fields_foo_testmess[0])
#define foo_TestMess_test_sint32 (&cpb_fields_foo_testmess[1])
#define foo_TestMess_test_sfixed32 (&cpb_fields_foo_testmess[2])
#define foo_TestMess_test_int64 (&cpb_fields_foo_testmess[3])
#define foo_TestMess_test_sint64 (&cpb_fields_foo_testmess[4])
#define foo_TestMess_test_sfixed64 (&cpb_fields_foo_testmess[5])
#define foo_TestMess_test_uint32 (&cpb_fields_foo_testmess[6])
#define foo_TestMess_test_fixed32 (&cpb_fields_foo_testmess[7])
#define foo_TestMess_test_uint64 (&cpb_fields_foo_testmess[8])
#define foo_TestMess_test_fixed64 (&cpb_fields_foo_testmess[9])
#define foo_TestMess_test_float (&cpb_fields_foo_testmess[10])
#define foo_TestMess_test_double (&cpb_fields_foo_testmess[11])
#define foo_TestMess_test_boolean (&cpb_fields_foo_testmess[12])
#define foo_TestMess_test_enum_small (&cpb_fields_foo_testmess[13])
#define foo_TestMess_test_enum (&cpb_fields_foo_testmess[14])
#define foo_TestMess_test_string (&cpb_fields_foo_testmess[15])
#define foo_TestMess_test_bytes (&cpb_fields_foo_testmess[16])
#define foo_TestMess_test_message (&cpb_fields_foo_testmess[17])

extern const struct cpb_field_desc cpb_fields_foo_testmesspacked[];

/* 'TestMessPacked' field descriptor pointers */
#define foo_TestMessPacked_test_int32 (&cpb_fields_foo_testmesspacked[0])
#define foo_TestMessPacked_test_sint32 (&cpb_fields_foo_testmesspacked[1])
#define foo_TestMessPacked_test_sfixed32 (&cpb_fields_foo_testmesspacked[2])
#define foo_TestMessPacked_test_int64 (&cpb_fields_foo_testmesspacked[3])
#define foo_TestMessPacked_test_sint64 (&cpb_fields_foo_testmesspacked[4])
#define foo_TestMessPacked_test_sfixed64 (&cpb_fields_foo_testmesspacked[5])
#define foo_TestMessPacked_test_uint32 (&cpb_fields_foo_testmesspacked[6])
#define foo_TestMessPacked_test_fixed32 (&cpb_fields_foo_testmesspacked[7])
#define foo_TestMessPacked_test_uint64 (&cpb_fields_foo_testmesspacked[8])
#define foo_TestMessPacked_test_fixed64 (&cpb_fields_foo_testmesspacked[9])
#define foo_TestMessPacked_test_float (&cpb_fields_foo_testmesspacked[10])
#define foo_TestMessPacked_test_double (&cpb_fields_foo_testmesspacked[11])
#define foo_TestMessPacked_test_boolean (&cpb_fields_foo_testmesspacked[12])
#define foo_TestMessPacked_test_enum_small (&cpb_fields_foo_testmesspacked[13])
#define foo_TestMessPacked_test_enum (&cpb_fields_foo_testmesspacked[14])

extern const struct cpb_field_desc cpb_fields_foo_testmessoptional[];

/* 'TestMessOptional' field descriptor pointers */
#define foo_TestMessOptional_test_int32 (&cpb_fields_foo_testmessoptional[0])
#define foo_TestMessOptional_test_sint32 (&cpb_fields_foo_testmessoptional[1])
#define foo_TestMessOptional_test_sfixed32 (&cpb_fields_foo_testmessoptional[2])
#define foo_TestMessOptional_test_int64 (&cpb_fields_foo_testmessoptional[3])
#define foo_TestMessOptional_test_sint64 (&cpb_fields_foo_testmessoptional[4])
#define foo_TestMessOptional_test_sfixed64 (&cpb_fields_foo_testmessoptional[5])
#define foo_TestMessOptional_test_uint32 (&cpb_fields_foo_testmessoptional[6])
#define foo_TestMessOptional_test_fixed32 (&cpb_fields_foo_testmessoptional[7])
#define foo_TestMessOptional_test_uint64 (&cpb_fields_foo_testmessoptional[8])
#define foo_TestMessOptional_test_fixed64 (&cpb_fields_foo_testmessoptional[9])
#define foo_TestMessOptional_test_float (&cpb_fields_foo_testmessoptional[10])
#define foo_TestMessOptional_test_double (&cpb_fields_foo_testmessoptional[11])
#define foo_TestMessOptional_test_boolean (&cpb_fields_foo_testmessoptional[12])
#define foo_TestMessOptional_test_enum_small (&cpb_fields_foo_testmessoptional[13])
#define foo_TestMessOptional_test_enum (&cpb_fields_foo_testmessoptional[14])
#define foo_TestMessOptional_test_string (&cpb_fields_foo_testmessoptional[15])
#define foo_TestMessOptional_test_bytes (&cpb_fields_foo_testmessoptional[16])
#define foo_TestMessOptional_test_message (&cpb_fields_foo_testmessoptional[17])

extern const struct cpb_field_desc cpb_fields_foo_testmessrequiredint32[];

/* 'TestMessRequiredInt32' field descriptor pointers */
#define foo_TestMessRequiredInt32_test (&cpb_fields_foo_testmessrequiredint32[0])

extern const struct cpb_field_desc cpb_fields_foo_testmessrequiredsint32[];

/* 'TestMessRequiredSInt32' field descriptor pointers */
#define foo_TestMessRequiredSInt32_test (&cpb_fields_foo_testmessrequiredsint32[0])

extern const struct cpb_field_desc cpb_fields_foo_testmessrequiredsfixed32[];

/* 'TestMessRequiredSFixed32' field descriptor pointers */
#define foo_TestMessRequiredSFixed32_test (&cpb_fields_foo_testmessrequiredsfixed32[0])

extern const struct cpb_field_desc cpb_fields_foo_testmessrequiredint64[];

/* 'TestMessRequiredInt64' field descriptor pointers */
#define foo_TestMessRequiredInt64_test (&cpb_fields_foo_testmessrequiredint64[0])

extern const struct cpb_field_desc cpb_fields_foo_testmessrequiredsint64[];

/* 'TestMessRequiredSInt64' field descriptor pointers */
#define foo_TestMessRequiredSInt64_test (&cpb_fields_foo_testmessrequiredsint64[0])

extern const struct cpb_field_desc cpb_fields_foo_testmessrequiredsfixed64[];

/* 'TestMessRequiredSFixed64' field descriptor pointers */
#define foo_TestMessRequiredSFixed64_test (&cpb_fields_foo_testmessrequiredsfixed64[0])

extern const struct cpb_field_desc cpb_fields_foo_testmessrequireduint32[];

/* 'TestMessRequiredUInt32' field descriptor pointers */
#define foo_TestMessRequiredUInt32_test (&cpb_fields_foo_testmessrequireduint32[0])

extern const struct cpb_field_desc cpb_fields_foo_testmessrequiredfixed32[];

/* 'TestMessRequiredFixed32' field descriptor pointers */
#define foo_TestMessRequiredFixed32_test (&cpb_fields_foo_testmessrequiredfixed32[0])

extern const struct cpb_field_desc cpb_fields_foo_testmessrequireduint64[];

/* 'TestMessRequiredUInt64' field descriptor pointers */
#define foo_TestMessRequiredUInt64_test (&cpb_fields_foo_testmessrequireduint64[0])

extern const struct cpb_field_desc cpb_fields_foo_testmessrequiredfixed64[];

/* 'TestMessRequiredFixed64' field descriptor pointers */
#define foo_TestMessRequiredFixed64_test (&cpb_fields_foo_testmessrequiredfixed64[0])

extern const struct cpb_field_desc cpb_fields_foo_testmessrequiredfloat[];

/* 'TestMessRequiredFloat' field descriptor pointers */
#define foo_TestMessRequiredFloat_test (&cpb_fields_foo_testmessrequiredfloat[0])

extern const struct cpb_field_desc cpb_fields_foo_testmessrequireddouble[];

/* 'TestMessRequiredDouble' field descriptor pointers */
#define foo_TestMessRequiredDouble_test (&cpb_fields_foo_testmessrequireddouble[0])

extern const struct cpb_field_desc cpb_fields_foo_testmessrequiredbool[];

/* 'TestMessRequiredBool' field descriptor pointers */
#define foo_TestMessRequiredBool_test (&cpb_fields_foo_testmessrequiredbool[0])

extern const struct cpb_field_desc cpb_fields_foo_testmessrequiredenum[];

/* 'TestMessRequiredEnum' field descriptor pointers */
#define foo_TestMessRequiredEnum_test (&cpb_fields_foo_testmessrequiredenum[0])

extern const struct cpb_field_desc cpb_fields_foo_testmessrequiredenumsmall[];

/* 'TestMessRequiredEnumSmall' field descriptor pointers */
#define foo_TestMessRequiredEnumSmall_test (&cpb_fields_foo_testmessrequiredenumsmall[0])

extern const struct cpb_field_desc cpb_fields_foo_testmessrequiredstring[];

/* 'TestMessRequiredString' field descriptor pointers */
#define foo_TestMessRequiredString_test (&cpb_fields_foo_testmessrequiredstring[0])

extern const struct cpb_field_desc cpb_fields_foo_testmessrequiredbytes[];

/* 'TestMessRequiredBytes' field descriptor pointers */
#define foo_TestMessRequiredBytes_test (&cpb_fields_foo_testmessrequiredbytes[0])

extern const struct cpb_field_desc cpb_fields_foo_testmessrequiredmessage[];

/* 'TestMessRequiredMessage' field descriptor pointers */
#define foo_TestMessRequiredMessage_test (&cpb_fields_foo_testmessrequiredmessage[0])

extern const struct cpb_field_desc cpb_fields_foo_emptymess[];

/* 'EmptyMess' field descriptor pointers */

extern const struct cpb_field_desc cpb_fields_foo_defaultrequiredvalues[];

/* 'DefaultRequiredValues' field descriptor pointers */
#define foo_DefaultRequiredValues_v_int32 (&cpb_fields_foo_defaultrequiredvalues[0])
#define foo_DefaultRequiredValues_v_uint32 (&cpb_fields_foo_defaultrequiredvalues[1])
#define foo_DefaultRequiredValues_v_int64 (&cpb_fields_foo_defaultrequiredvalues[2])
#define foo_DefaultRequiredValues_v_uint64 (&cpb_fields_foo_defaultrequiredvalues[3])
#define foo_DefaultRequiredValues_v_float (&cpb_fields_foo_defaultrequiredvalues[4])
#define foo_DefaultRequiredValues_v_double (&cpb_fields_foo_defaultrequiredvalues[5])
#define foo_DefaultRequiredValues_v_string (&cpb_fields_foo_defaultrequiredvalues[6])
#define foo_DefaultRequiredValues_v_bytes (&cpb_fields_foo_defaultrequiredvalues[7])

extern const struct cpb_field_desc cpb_fields_foo_defaultoptionalvalues[];

/* 'DefaultOptionalValues' field descriptor pointers */
#define foo_DefaultOptionalValues_v_int32 (&cpb_fields_foo_defaultoptionalvalues[0])
#define foo_DefaultOptionalValues_v_uint32 (&cpb_fields_foo_defaultoptionalvalues[1])
#define foo_DefaultOptionalValues_v_int64 (&cpb_fields_foo_defaultoptionalvalues[2])
#define foo_DefaultOptionalValues_v_uint64 (&cpb_fields_foo_defaultoptionalvalues[3])
#define foo_DefaultOptionalValues_v_float (&cpb_fields_foo_defaultoptionalvalues[4])
#define foo_DefaultOptionalValues_v_double (&cpb_fields_foo_defaultoptionalvalues[5])
#define foo_DefaultOptionalValues_v_string (&cpb_fields_foo_defaultoptionalvalues[6])
#define foo_DefaultOptionalValues_v_bytes (&cpb_fields_foo_defaultoptionalvalues[7])

extern const struct cpb_field_desc cpb_fields_foo_allocvalues[];

/* 'AllocValues' field descriptor pointers */
#define foo_AllocValues_o_bytes (&cpb_fields_foo_allocvalues[0])
#define foo_AllocValues_r_string (&cpb_fields_foo_allocvalues[1])
#define foo_AllocValues_a_string (&cpb_fields_foo_allocvalues[2])
#define foo_AllocValues_a_bytes (&cpb_fields_foo_allocvalues[3])
#define foo_AllocValues_a_mess (&cpb_fields_foo_allocvalues[4])

extern const struct cpb_service_desc cpb_services_foo[];

/* Service descriptor pointers */

#endif /* __TEST_FULL_PB2_H__ */
