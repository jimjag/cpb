/** @file encoder.h
 * 
 * Simple C protocol buffers (cpb) encoder interface.
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

#ifndef __CPB_CORE_ENCODER2_H__
#define __CPB_CORE_ENCODER2_H__

#include <cpb/cpb.h>

/** Protocol buffer encoder */
struct cpb_encoder2 {
    cpb_bool_t packed;
};

size_t cpb_encode_varint(u8_t *buf, u64_t varint);

size_t cpb_encode_32bit(u8_t *buf, u32_t value);

size_t cpb_encode_64bit(u8_t *buf, u64_t value);

void cpb_encoder2_init(struct cpb_encoder2 *encoder);

void cpb_encoder2_start(struct cpb_encoder2 *encoder,
                         const struct cpb_msg_desc *msg_desc);

cpb_err_t cpb_encoder2_packed_repeated_start(struct cpb_encoder2 *encoder,
                                     const struct cpb_field_desc *field_desc);

cpb_err_t cpb_encoder2_packed_repeated_end(struct cpb_encoder2 *encoder);

size_t cpb_encoder2_add_field(struct cpb_encoder2 *encoder,
                               const struct cpb_field_desc *field_desc,
                               union cpb_value *value,
                               u8_t* buf);

#endif /* __CPB_CORE_ENCODER2_H__ */
