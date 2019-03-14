/** @file debug.h
 *
 * Debugging macros.
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

#ifndef __CPB_CORE_DEBUG_H__
#define __CPB_CORE_DEBUG_H__

#include <cpb/cpb.h>


/** Simple assert macro */
#define CPB_ASSERT(_expr_, _msg_)  \
    do {                           \
        if (!(_expr_)) {           \
            printf(_msg_ "\n");    \
            abort();               \
        }                          \
    } while (0)

/** Simple failure macro */
#define CPB_FAIL(_msg_)            \
    do {                           \
        printf(_msg_ "\n");        \
        abort();                   \
    } while(0)

/* Logging macros */
#define CPB_DEBUG(_format_, _args_...) \
    printf("DBG: " _format_ "\n", ##_args_)
#define CPB_INFO(_format_, _args_...) \
    printf("INF: " _format_ "\n", ##_args_)
#define CPB_ERR(_format_, _args_...) \
    printf("ERR: " _format_ "\n", ##_args_)

#endif /* __CPB_CORE_DEBUG_H__ */
