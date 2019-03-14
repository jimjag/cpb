/** @file test_simple.c
 *
 * A simple test more intended to be an example of how to use the library.
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

#include "generated/test_simple_pb2.h"

int main()
{
    char buf[4096];
    size_t len;
    cpb_err_t ret;
    
    struct cpb_decoder decoder;
    struct cpb_encoder encoder;
    
    cpb_encoder_init(&encoder);
    cpb_encoder_start(&encoder, test_Person, buf, sizeof(buf));
    cpb_encoder_add_string(&encoder, test_Person_name, "Jim Jagielski");
    cpb_encoder_add_int32(&encoder, test_Person_id, 1237);
    cpb_encoder_add_string(&encoder, test_Person_email, "jimjag@gmail.com");
    cpb_encoder_nested_start(&encoder, test_Person_phone);
    cpb_encoder_add_string(&encoder, test_PhoneNumber_number, "123456789");
    cpb_encoder_add_enum(&encoder, test_PhoneNumber_type, TEST_PHONENUMBER_MOBILE);
    cpb_encoder_nested_end(&encoder);
    cpb_encoder_nested_start(&encoder, test_Person_phone);
    cpb_encoder_add_string(&encoder, test_PhoneNumber_number, "+123456789");
    cpb_encoder_add_enum(&encoder, test_PhoneNumber_type, TEST_PHONENUMBER_HOME);
    cpb_encoder_nested_end(&encoder);
    cpb_encoder_nested_start(&encoder, test_Person_phone);
    cpb_encoder_add_string(&encoder, test_PhoneNumber_number, "++123456789");
    cpb_encoder_add_enum(&encoder, test_PhoneNumber_type, TEST_PHONENUMBER_WORK);
    cpb_encoder_nested_end(&encoder);
    len = cpb_encoder_finish(&encoder);

    printf("encoded message length = %d\n", len);
    
    cpb_decoder_init(&decoder);
    cpb_decoder_use_debug_handlers(&decoder);
    ret = cpb_decoder_decode(&decoder, test_Person, &buf, len, NULL);
    
    printf("ret = %d\n", ret);
    
    return 0;
}
