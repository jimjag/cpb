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

#ifndef __CPB_CORE_ENCODER_H__
#define __CPB_CORE_ENCODER_H__

#include <cpb/cpb.h>

/** Encoder stack frame */
struct cpb_encoder_stack_frame {
    struct cpb_buf buf;
    const struct cpb_field_desc *field_desc;
    const struct cpb_msg_desc *msg_desc;
};

/** Protocol buffer encoder */
struct cpb_encoder {
    struct cpb_encoder_stack_frame stack[CPB_MAX_DEPTH];
    int depth;
    cpb_bool_t packed;
};

void cpb_encoder_init(struct cpb_encoder *encoder);

void cpb_encoder_start(struct cpb_encoder *encoder,
                        const struct cpb_msg_desc *msg_desc,
                        void *data, size_t len);

size_t cpb_encoder_finish(struct cpb_encoder *encoder);

cpb_err_t cpb_encoder_nested_start(struct cpb_encoder *encoder,
                                     const struct cpb_field_desc *field_desc);

cpb_err_t cpb_encoder_nested_end(struct cpb_encoder *encoder);

cpb_err_t cpb_encoder_packed_repeated_start(struct cpb_encoder *encoder,
                                              const struct cpb_field_desc *field_desc);

cpb_err_t cpb_encoder_packed_repeated_end(struct cpb_encoder *encoder);

cpb_err_t cpb_encoder_add_field(struct cpb_encoder *encoder,
                                  const struct cpb_field_desc *field_desc,
                                  union cpb_value *value);

cpb_err_t cpb_encoder_add_double(struct cpb_encoder *encoder,
                                   const struct cpb_field_desc *field_desc,
                                   double double_);

cpb_err_t cpb_encoder_add_float(struct cpb_encoder *encoder,
                                  const struct cpb_field_desc *field_desc,
                                  float float_);

cpb_err_t cpb_encoder_add_int32(struct cpb_encoder *encoder,
                                  const struct cpb_field_desc *field_desc,
                                  s32_t int32);

cpb_err_t cpb_encoder_add_uint32(struct cpb_encoder *encoder,
                                   const struct cpb_field_desc *field_desc,
                                   u32_t uint32);

cpb_err_t cpb_encoder_add_int64(struct cpb_encoder *encoder,
                                  const struct cpb_field_desc *field_desc,
                                  s64_t int64);

cpb_err_t cpb_encoder_add_uint64(struct cpb_encoder *encoder,
                                   const struct cpb_field_desc *field_desc,
                                   u64_t uint64);

cpb_err_t cpb_encoder_add_bool(struct cpb_encoder *encoder,
                                 const struct cpb_field_desc *field_desc,
                                 cpb_bool_t bool);

cpb_err_t cpb_encoder_add_enum(struct cpb_encoder *encoder,
                                 const struct cpb_field_desc *field_desc,
                                 cpb_enum_t enum_);

cpb_err_t cpb_encoder_add_string(struct cpb_encoder *encoder,
                                   const struct cpb_field_desc *field_desc,
                                   char *str);

cpb_err_t cpb_encoder_add_bytes(struct cpb_encoder *encoder,
                                  const struct cpb_field_desc *field_desc,
                                  u8_t *data, size_t len);


#endif /* __CPB_CORE_ENCODER_H__ */
