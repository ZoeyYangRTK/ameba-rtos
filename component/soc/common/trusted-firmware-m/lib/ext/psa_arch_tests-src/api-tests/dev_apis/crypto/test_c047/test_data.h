/** @file
 * Copyright (c) 2019-2021, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#include "val_crypto.h"

typedef struct {
    char                    test_desc[75];
    psa_key_type_t          key_type;
    uint8_t                 key_data[64];
    uint32_t                key_length;
    uint8_t                 data[16];
    size_t                  data_size;
    uint8_t                 expected_mac[64];
    size_t                  mac_size;
    psa_key_usage_t         usage;
    psa_algorithm_t         key_alg;
    psa_status_t            expected_status;
} test_data;

static const test_data check1[] = {
#ifdef ARCH_TEST_HMAC
#ifdef ARCH_TEST_SHA224
{"Test psa_mac_verify HMAC SHA 224\n", PSA_KEY_TYPE_HMAC,
{0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b}, 20,
{0x48, 0x69, 0x20, 0x54, 0x68, 0x65, 0x72, 0x65}, 8,
{0x89, 0x6f, 0xb1, 0x12, 0x8a, 0xbb, 0xdf, 0x19, 0x68, 0x32, 0x10, 0x7c, 0xd4,
 0x9d, 0xf3, 0x3f, 0x47, 0xb4, 0xb1, 0x16, 0x99, 0x12, 0xba, 0x4f, 0x53, 0x68,
 0x4b, 0x22}, 28,
 PSA_KEY_USAGE_VERIFY_HASH, PSA_ALG_HMAC(PSA_ALG_SHA_224),
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_SHA256
{"Test psa_mac_verify HMAC SHA 256\n", PSA_KEY_TYPE_HMAC,
{0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b}, 20,
{0x48, 0x69, 0x20, 0x54, 0x68, 0x65, 0x72, 0x65}, 8,
{0xb0, 0x34, 0x4c, 0x61, 0xd8, 0xdb, 0x38, 0x53, 0x5c, 0xa8, 0xaf, 0xce, 0xaf,
 0x0b, 0xf1, 0x2b, 0x88, 0x1d, 0xc2, 0x00, 0xc9, 0x83, 0x3d, 0xa7, 0x26, 0xe9,
 0x37, 0x6c, 0x2e, 0x32, 0xcf, 0xf7}, 32,
 PSA_KEY_USAGE_VERIFY_HASH, PSA_ALG_HMAC(PSA_ALG_SHA_256),
 PSA_SUCCESS
},

{"Test psa_mac_verify - Incompactible HMAC for CMAC\n", PSA_KEY_TYPE_HMAC,
{0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b}, 20,
{0x48, 0x69, 0x20, 0x54, 0x68, 0x65, 0x72, 0x65}, 8,
{0xb0, 0x34, 0x4c, 0x61, 0xd8, 0xdb, 0x38, 0x53, 0x5c, 0xa8, 0xaf, 0xce, 0xaf,
 0x0b, 0xf1, 0x2b, 0x88, 0x1d, 0xc2, 0x00, 0xc9, 0x83, 0x3d, 0xa7, 0x26, 0xe9,
 0x37, 0x6c, 0x2e, 0x32, 0xcf, 0xf7}, 32,
 PSA_KEY_USAGE_VERIFY_HASH, PSA_ALG_CMAC,
 PSA_ERROR_INVALID_ARGUMENT
},

{"Test psa_mac_verify - Invalid usage\n", PSA_KEY_TYPE_HMAC,
{0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b}, 20,
{0x48, 0x69, 0x20, 0x54, 0x68, 0x65, 0x72, 0x65}, 8,
{0xb0, 0x34, 0x4c, 0x61, 0xd8, 0xdb, 0x38, 0x53, 0x5c, 0xa8, 0xaf, 0xce, 0xaf,
 0x0b, 0xf1, 0x2b, 0x88, 0x1d, 0xc2, 0x00, 0xc9, 0x83, 0x3d, 0xa7, 0x26, 0xe9,
 0x37, 0x6c, 0x2e, 0x32, 0xcf, 0xf7}, 32,
 PSA_KEY_USAGE_EXPORT, PSA_ALG_HMAC(PSA_ALG_SHA_256),
 PSA_ERROR_NOT_PERMITTED
},

{"Test psa_mac_verify - Truncated MAC too large\n", PSA_KEY_TYPE_HMAC,
{0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b}, 20,
{0x48, 0x69, 0x20, 0x54, 0x68, 0x65, 0x72, 0x65}, 8,
{0xb0, 0x34, 0x4c, 0x61, 0xd8, 0xdb, 0x38, 0x53, 0x5c, 0xa8, 0xaf, 0xce, 0xaf,
 0x0b, 0xf1, 0x2b, 0x88, 0x1d, 0xc2, 0x00, 0xc9, 0x83, 0x3d, 0xa7, 0x26, 0xe9,
 0x37, 0x6c, 0x2e, 0x32, 0xcf, 0xf7}, 32,
 PSA_KEY_USAGE_VERIFY_HASH,  PSA_ALG_TRUNCATED_MAC(PSA_ALG_HMAC(PSA_ALG_SHA_256), 33),
 PSA_ERROR_INVALID_ARGUMENT
},

{"Test psa_mac_verify - Truncated MAC too small\n", PSA_KEY_TYPE_HMAC,
{0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b}, 20,
{0x48, 0x69, 0x20, 0x54, 0x68, 0x65, 0x72, 0x65}, 8,
{0xb0, 0x34, 0x4c, 0x61, 0xd8, 0xdb, 0x38, 0x53, 0x5c, 0xa8, 0xaf, 0xce, 0xaf,
 0x0b, 0xf1, 0x2b, 0x88, 0x1d, 0xc2, 0x00, 0xc9, 0x83, 0x3d, 0xa7, 0x26, 0xe9,
 0x37, 0x6c, 0x2e, 0x32, 0xcf, 0xf7}, 32,
 PSA_KEY_USAGE_VERIFY_HASH, PSA_ALG_TRUNCATED_MAC(PSA_ALG_HMAC(PSA_ALG_SHA_256), 1),
 PSA_ERROR_NOT_SUPPORTED
},

{"Test psa_mac_verify - bad algorithm (unknown MAC algorithm)\n", PSA_KEY_TYPE_HMAC,
{0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b}, 20,
{0x48, 0x69, 0x20, 0x54, 0x68, 0x65, 0x72, 0x65}, 8,
{0xb0, 0x34, 0x4c, 0x61, 0xd8, 0xdb, 0x38, 0x53, 0x5c, 0xa8, 0xaf, 0xce, 0xaf,
 0x0b, 0xf1, 0x2b, 0x88, 0x1d, 0xc2, 0x00, 0xc9, 0x83, 0x3d, 0xa7, 0x26, 0xe9,
 0x37, 0x6c, 0x2e, 0x32, 0xcf, 0xf7}, 32,
 PSA_KEY_USAGE_VERIFY_HASH, PSA_ALG_HMAC(0),
 PSA_ERROR_NOT_SUPPORTED
},

#endif

#ifdef ARCH_TEST_SHA512
{"Test psa_mac_verify HMAC SHA 512\n", PSA_KEY_TYPE_HMAC,
{0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b}, 20,
{0x48, 0x69, 0x20, 0x54, 0x68, 0x65, 0x72, 0x65}, 8,
{0x87, 0xaa, 0x7c, 0xde, 0xa5, 0xef, 0x61, 0x9d, 0x4f, 0xf0, 0xb4, 0x24, 0x1a,
 0x1d, 0x6c, 0xb0, 0x23, 0x79, 0xf4, 0xe2, 0xce, 0x4e, 0xc2, 0x78, 0x7a, 0xd0,
 0xb3, 0x05, 0x45, 0xe1, 0x7c, 0xde, 0xda, 0xa8, 0x33, 0xb7, 0xd6, 0xb8, 0xa7,
 0x02, 0x03, 0x8b, 0x27, 0x4e, 0xae, 0xa3, 0xf4, 0xe4, 0xbe, 0x9d, 0x91, 0x4e,
 0xeb, 0x61, 0xf1, 0x70, 0x2e, 0x69, 0x6c, 0x20, 0x3a, 0x12, 0x68, 0x54}, 64,
 PSA_KEY_USAGE_VERIFY_HASH, PSA_ALG_HMAC(PSA_ALG_SHA_512),
 PSA_SUCCESS
},
#endif

#ifdef ARCH_TEST_SHA224
{"Test psa_mac_verify HMAC SHA 224 (truncated to 8 Byte)\n", PSA_KEY_TYPE_HMAC,
{0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b}, 20,
{0x48, 0x69, 0x20, 0x54, 0x68, 0x65, 0x72, 0x65}, 8,
{0x89, 0x6f, 0xb1, 0x12, 0x8a, 0xbb, 0xdf, 0x19, 0x68}, 8,
 PSA_KEY_USAGE_VERIFY_HASH, PSA_ALG_TRUNCATED_MAC(PSA_ALG_HMAC(PSA_ALG_SHA_224), 8),
 PSA_SUCCESS
},
#endif
#endif

#ifdef ARCH_TEST_AES_128
#ifdef ARCH_TEST_CMAC
{"Test psa_mac_verify CMAC AES 128\n", PSA_KEY_TYPE_AES,
{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09,
 0xcf, 0x4f, 0x3c}, 16,
{0x48, 0x69, 0x20, 0x54, 0x68, 0x65, 0x72, 0x65}, 8,
{0x9A, 0x8F, 0xFF, 0x8D, 0xA3, 0x5B, 0x97, 0xCB, 0x4C, 0x95, 0xF0, 0xFA, 0x6A,
 0xE7, 0xE0, 0x77}, 16,
 PSA_KEY_USAGE_VERIFY_HASH, PSA_ALG_CMAC,
 PSA_SUCCESS
},
#endif
#endif

{"Test psa_mac_verify - Invalid key type\n", PSA_KEY_TYPE_RAW_DATA,
{0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09,
 0xcf, 0x4f, 0x3c}, 16,
{0x48, 0x69, 0x20, 0x54, 0x68, 0x65, 0x72, 0x65}, 8,
{0x9A, 0x8F, 0xFF, 0x8D, 0xA3, 0x5B, 0x97, 0xCB, 0x4C, 0x95, 0xF0, 0xFA, 0x6A,
 0xE7, 0xE0, 0x77}, 16,
 PSA_KEY_USAGE_VERIFY_HASH, PSA_ALG_CMAC,
 PSA_ERROR_INVALID_ARGUMENT
},

#ifdef ARCH_TEST_HMAC
#ifdef ARCH_TEST_SHA256
{"Test psa_mac_verify small size buffer\n", PSA_KEY_TYPE_HMAC,
{0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b}, 20,
{0x48, 0x69, 0x20, 0x54, 0x68, 0x65, 0x72, 0x65}, 8,
{0xb0, 0x34, 0x4c, 0x61, 0xd8, 0xdb, 0x38, 0x53, 0x5c, 0xa8, 0xaf, 0xce, 0xaf,
 0x0b, 0xf1, 0x2b, 0x88, 0x1d, 0xc2, 0x00, 0xc9, 0x83, 0x3d, 0xa7, 0x26, 0xe9,
 0x37, 0x6c, 0x2e, 0x32, 0xcf, 0xf7}, 30,
 PSA_KEY_USAGE_VERIFY_HASH, PSA_ALG_HMAC(PSA_ALG_SHA_256),
 PSA_ERROR_INVALID_SIGNATURE
},
#endif

#ifdef ARCH_TEST_SHA224
{"Test psa_mac_verify incorrect expected MAC\n", PSA_KEY_TYPE_HMAC,
{0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b}, 20,
{0x48, 0x69, 0x20, 0x54, 0x68, 0x65, 0x72, 0x65}, 8,
{0x89, 0x6f, 0xb1, 0x12, 0x8a, 0xbb, 0xdf, 0x19, 0x68, 0x32, 0x10, 0x7c, 0xd4,
 0x9d, 0xf3, 0x3f, 0x47, 0xb4, 0xb1, 0x16, 0x99, 0x12, 0xba, 0x4f, 0x53, 0x68,
 0x4b, 0x20}, 28,
 PSA_KEY_USAGE_VERIFY_HASH, PSA_ALG_HMAC(PSA_ALG_SHA_224),
 PSA_ERROR_INVALID_SIGNATURE
},
#endif
#endif
};
