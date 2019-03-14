/** @file test_struct_map.c
 *
 * Tests the struct map encoder and decoder.
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

#include "generated/test_struct_map_pb2.h"

struct test_struct_nested1 {
    s32_t field_int32;
    s64_t field_int64;
};

struct test_struct_nested2 {
    char field_string[32];
};

struct test_struct {
    s32_t field_int32;
    s64_t field_int64;
    cpb_bool_t field_bool;
    cpb_enum_t field_enum;
    char field_string[32];
    u8_t field_bytes[8];
    struct test_struct_nested1 nested1;
    struct test_struct_nested2 nested2[8];
};

CPB_STRUCT_MAP_BEGIN(test_struct_nested1_map, test_StructTest_Nested1, struct test_struct_nested1)
CPB_STRUCT_MAP_INT32(test_StructTest_Nested1_field_int32, struct test_struct_nested1, field_int32, 1)
CPB_STRUCT_MAP_INT64(test_StructTest_Nested1_field_int64, struct test_struct_nested1, field_int64, 1)
CPB_STRUCT_MAP_END

CPB_STRUCT_MAP_BEGIN(test_struct_nested2_map, test_StructTest_Nested2, struct test_struct_nested2)
CPB_STRUCT_MAP_STRING(test_StructTest_Nested2_field_string, struct test_struct_nested2, field_string, 32, 1)
CPB_STRUCT_MAP_END

CPB_STRUCT_MAP_BEGIN(test_struct_map, test_StructTest, struct test_struct)
CPB_STRUCT_MAP_INT32(test_StructTest_field_int32, struct test_struct, field_int32, 1)
CPB_STRUCT_MAP_INT64(test_StructTest_field_int64, struct test_struct, field_int64, 1)
CPB_STRUCT_MAP_BOOL(test_StructTest_field_bool, struct test_struct, field_bool, 1)
CPB_STRUCT_MAP_ENUM(test_StructTest_field_enum, struct test_struct, field_enum, 1)
CPB_STRUCT_MAP_STRING(test_StructTest_field_string, struct test_struct, field_string, 32, 1)
CPB_STRUCT_MAP_BYTES(test_StructTest_field_bytes, struct test_struct, field_bytes, 8, 1)
CPB_STRUCT_MAP_MESSAGE(test_StructTest_nested1, struct test_struct, nested1, &test_struct_nested1_map, 1)
CPB_STRUCT_MAP_MESSAGE(test_StructTest_nested2, struct test_struct, nested2, &test_struct_nested2_map, 8)
CPB_STRUCT_MAP_END



void print_buf(u8_t *buf, size_t len)
{
    int i = 0;
    
    printf("{ ");
    while (i < len) {
        printf("0x%02x", buf[i]);
        if (i < len - 1)
            printf(", ");
        i++;
    }
    
    printf(" }");
}

int main()
{
    char buf[4096];
    size_t len;
    cpb_err_t ret;
    u8_t bytes[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
    struct test_struct test_struct_instance;
    int i;
    
    struct cpb_encoder encoder;
    struct cpb_struct_decoder sdecoder;

    cpb_encoder_init(&encoder);
    cpb_encoder_start(&encoder, test_StructTest, buf, sizeof(buf));
    cpb_encoder_add_int32(&encoder, test_StructTest_field_int32, 12345);
    cpb_encoder_add_int64(&encoder, test_StructTest_field_int64, 1234567890);
    cpb_encoder_add_bool(&encoder, test_StructTest_field_bool, 1);
    cpb_encoder_add_enum(&encoder, test_StructTest_field_enum, TEST_STRUCTTEST_VALUE1);
    cpb_encoder_add_string(&encoder, test_StructTest_field_string, "this is a test");
    cpb_encoder_add_bytes(&encoder, test_StructTest_field_bytes, bytes, sizeof(bytes));
    
    cpb_encoder_nested_start(&encoder, test_StructTest_nested1);
    cpb_encoder_add_int32(&encoder, test_StructTest_Nested1_field_int32, 123456);
    cpb_encoder_add_int64(&encoder, test_StructTest_Nested1_field_int64, 987654321);
    cpb_encoder_nested_end(&encoder);
    
    for (i = 0; i < 8; i++) {
        char tmp[] = "test string x";
        cpb_encoder_nested_start(&encoder, test_StructTest_nested2);
        tmp[12] = '0' + i;
        cpb_encoder_add_string(&encoder, test_StructTest_Nested2_field_string, tmp);
        cpb_encoder_nested_end(&encoder);
    }
    
    len = cpb_encoder_finish(&encoder);

    printf("encoded message length = %d\n", len);
    
    cpb_struct_decoder_init(&sdecoder);
    ret = cpb_struct_decoder_decode(&sdecoder, &test_struct_map, &test_struct_instance, buf, len, NULL);
    
    printf("ret = %d\n", ret);
    
    printf("test_struct.field_int32 = %d\n", test_struct_instance.field_int32);
    printf("test_struct.field_int64 = %lld\n", test_struct_instance.field_int64);
    printf("test_struct.field_bool = %d\n", test_struct_instance.field_bool);
    printf("test_struct.field_enum = %d\n", test_struct_instance.field_enum);
    printf("test_struct.field_string = '%s'\n", test_struct_instance.field_string);
    printf("test_struct.field_bytes = ");
    print_buf(test_struct_instance.field_bytes, sizeof(test_struct_instance.field_bytes));
    printf("\n");
    
    printf("test_struct.nested1.field_int32 = %d\n", test_struct_instance.nested1.field_int32);
    printf("test_struct.nested1.field_int64 = %lld\n", test_struct_instance.nested1.field_int64);
    
    for (i = 0; i < 8; i++)
        printf("test_struct.nested2[%d].field_string = '%s'\n", i, test_struct_instance.nested2[i].field_string);
    
    return 0;
}
