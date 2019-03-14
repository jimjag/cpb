/** @file cpb.h
 *
 * Simple C protocol buffers (cpb).
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

/** @mainpage Simple C protocol buffers (cpb)
 *
 * @section sec_introduction Introduction
 *
 * cpb (short for Simple C protocol buffers (cpb)) is an implementation of
 * Google's protocol buffers (in short protobuf) in C for systems with limited
 * resources. The latest version of cpb can be found on Google Code:
 *
 * http://code.google.com/p/cpb
 *
 * For more information about protocol buffers in general, see:
 *
 * http://code.google.com/p/protobuf/
 *
 * @section sec_design Design
 *
 * Most protobuf implementations represent messages as hierarchies of objects
 * during runtime, which can be encoded/decoded to/from binary data streams.
 * cpb in contrast, encodes and decodes messages on the fly, much like a SAX
 * parser does with XML. Encoding is done by calling encoder functions for each
 * message and field to be encoded. Decoding uses callback functions to notify
 * about messages and fields. Type information used for encoding and decoding
 * is gathered from the compiled protobuf schema, generated with the protoc
 * compiler using the cpb output module.
 *
 * @section sec_limitations Limitations
 *
 * There are a few inherent limitations in cpb, due to the simplicity of it's
 * design:
 *
 * - The decoder does not warn the client when 'required' fields are missing
 * - The decoder does not warn the client when 'required' or 'optional' fields
 *   have multiple occurances
 * - The encoder does not implicitly encode 'required' fields with their
 *   default values, when the client does not manually encode them
 *
 * @section sec_usage Usage
 *
 * The following shows a few examples of how to use cpb. We use the following
 * protobuf definitions:
 *
 * @code
 *
 * package test;
 *
 * message Info {
 *     required int32 result = 1;
 *     required string msg = 2;
 * }
 *
 * message TestMessage {
 *     required int32 count = 1;
 *     required Info info = 2;
 * }
 *
 * @endcode
 *
 * @subsection subsec_encoder Encoder
 *
 * The following example will encode a simple message of the type 'TestMessage':
 *
 * @code
 *
 * void encode_example(void)
 * {
 *     struct cpb_encoder encoder;
 *     unsigned char buf[128];
 *     size_t len;
 *
 *     // Initialize the encoder
 *     cpb_encoder_init(&encoder);
 *
 *     // Start encoding a message of type 'test.TestMessage' into buf
 *     cpb_encoder_start(&encoder, test_TestMessage, buf, sizeof(buf));
 *
 *     // Encode a 55 to the field 'count'
 *     cpb_encoder_add_int32(&encoder, test_TestMessage_count, 55);
 *
 *     // Start encoding the nested message of type 'test.Info' in field 'info'
 *     cpb_encoder_nested_start(&encoder, test_TestMessage_info);
 *
 *     // Encode a -1 to the field 'result'
 *     cpb_encoder_add_int32(&encoder, test_Info_result, -1);
 *
 *     // Encode a "Unknown" to the field 'msg'
 *     cpb_encoder_add_string(&encoder, test_Info_msg, "Unknown");
 *
 *     // Finish encoding the nested message of type 'test.Info'
 *     cpb_encoder_nested_end(&encoder);
 *
 *     // Finish encoding the message of type 'test.TestMessage'
 *     len = cpb_encoder_finish(&encoder);
 *
 *     // buf now holds the encoded message which is len bytes long
 * }
 *
 * @endcode
 *
 * @subsection subsec_decoder Decoder
 *
 * The following example will decode a simple message of the type 'TestMessage':
 *
 * @code
 *
 * // Structure to decode into
 *
 * struct TestMessage {
 *     int32 count;
 *     struct {
 *         int32 result;
 *         char msg[32];
 *     } info;
 * }
 *
 * void msg_start_handler(struct cpb_decoder *decoder,
 *                        const struct cpb_msg_desc *msg_desc, void *arg)
 * {
 *     // We don't use the message start handler
 * }
 *
 * void msg_end_handler(struct cpb_decoder *decoder,
 *                      const struct cpb_msg_desc *msg_desc, void *arg)
 * {
 *     // We don't use the message end handler
 * }
 *
 * void field_handler(struct cpb_decoder *decoder,
 *                    const struct cpb_msg_desc *msg_desc,
 *                    const struct cpb_field_desc *field_desc,
 *                    union cpb_value *value, void *arg)
 * {
 *     struct TestMessage *msg = arg;
 *
 *     // Copy fields into local structure
 *
 *     if (msg_desc == test_TestMessage) {
 *         if (field_desc == test_TestMessage_count)
 *             msg->count = value->int32;
 *     } else if (msg_desc == test_Info) {
 *         if (field_desc == test_Info_result)
 *             msg->info.result = value->int32;
 *         if (field_desc == test_Info_msg)
 *             strncpy(msg->info.msg, sizeof(msg->info.msg), value->string.str);
 *     }
 * }
 *
 * void decode_example(void)
 * {
 *     struct cpb_decoder decoder;
 *     unsigned char buf[128];
 *     size_t len;
 *     struct TestMessage msg;
 *
 *     // Initialize the decoder
 *     cpb_decoder_init(&decoder);
 *
 *     // Register a pointer to our local structure we want to decode into as
 *     // the argument for the decoder event handlers
 *     cpb_decoder_arg(&decoder, &msg);
 *
 *     // Register event handlers
 *     cpb_decoder_msg_handler(&decoder, msg_start_handler, msg_end_handler);
 *     cpb_decoder_field_handler(&decoder, field_handler);
 *
 *     // Decode the binary buffer from the encode example
 *     cpb_decoder_decode(&decoder, test_TestMessage, buf, len, NULL);
 *
 *     // The local structure 'msg' will now hold the decoded values
 * }
 *
 * @endcode
 *
 * @subsection subsec_struct_map Struct map
 *
 */

#ifndef __CPB_H__
#define __CPB_H__

#include <cpb/core/arch.h>
#include <cpb/core/debug.h>
#include <cpb/core/types.h>
#include <cpb/core/decoder.h>
#include <cpb/core/encoder.h>
#include <cpb/core/misc.h>
#include <cpb/utils/struct_decoder.h>
#include <cpb/utils/struct_map.h>

#endif /* __CPB_H__ */
