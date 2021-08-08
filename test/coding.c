/*
 * coding.c
 *
 * Based on work by:
 *   Created: 07.08.2021 10:02:12
 *   Author: nqtronix (https://github.com/nqtronix)
 *   License: MIT
 *
 * This modified version is licensed unter AGPLv3.
 *   Updated by BenBE (https://github.com/BenBE)
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "fast_hamming.h"

#include "test/testing.h"

/*
 * Test Data
 *
 * random data sourced from: https://www.random.org/cgi-bin/randbyte?nbytes=15&format=h
 * to format the data correctly, mark the array data and use the following REGEX (excluding ""):
 * search:  "([0-9a-f]+)[\W]"
 * replace: "0x$1, "
 */
const uint8_t data8[15] = {
    0x0b, 0x28, 0x48, 0x69, 0x5e, 0xc8, 0xeb, 0x87, 0x7f, 0x28, 0xdf, 0x52, 0x03, 0xb4, 0x46,
};

const uint8_t data8_enc[16] = {
    0x0b, 0x28, 0x48, 0x69, 0x5e, 0xc8, 0xeb, 0x87, 0x7f, 0x28, 0xdf, 0x52, 0x03, 0xb4, 0x46, 0xd9,
};


bool test_selftest() {
    bool result = true;

    TEST(sizeof(data8), sizeof(data8_enc) - 1);
    TEST(memcmp(data8, data8_enc, sizeof(data8)), 0);

    return result;
}

bool test_coding_encode() {
    bool result = true;

    // tmp storage
    uint8_t outbuf[16] = {0};			// result of encoding
    size_t outsize = sizeof(outbuf);		// length after encoding

    // encode data
    TEST(hecc_encode(data8, sizeof(data8), outbuf, &outsize), true);
    const uint8_t code = outbuf[15];		// backup of parity data

    TEST(outsize, sizeof(data8) + 1);
    TEST(outsize, sizeof(data8_enc));

    TEST(code, data8_enc[15]);

    TEST(memcmp(outbuf, data8_enc, outsize), 0);

    return result;
}

bool test_coding_decode() {
    bool result = true;

    // verify no error
    uint8_t dummy[15] = {0};			// write data here to discard
    size_t outsize = sizeof(dummy);

    TEST(hecc_decode(data8_enc, sizeof(data8_enc), dummy, &outsize), true);

    return result;
}

bool test_coding_roundtrip() {
    bool result = true;

    // tmp storage
    uint8_t outbuf[16] = {0};			// result of encoding
    size_t outsize = sizeof(outbuf);		// length after encoding

    // encode data
    TEST(hecc_encode(data8, sizeof(data8), outbuf, &outsize), true);

    TEST(outsize, sizeof(data8) + 1);
    TEST(outsize, sizeof(data8_enc));

    TEST(memcmp(outbuf, data8_enc, outsize), 0);

    // verify no error
    uint8_t dummy[15] = {0};			// write data here to discard

    TEST(hecc_decode(outbuf, outsize, dummy, &outsize), true);
    TEST(outsize, sizeof(data8));

    TEST(memcmp(dummy, data8, outsize), 0);

    return result;
}

bool test_error_single() {
    bool result = true;

    uint8_t outbuf[16] = {0};			// result of encoding
    size_t outsize = 0u;			// length after encoding

    uint8_t dummy[15] = {0};			// write data here to discard

    // verify data error
    for (uint32_t idx = 0; idx < sizeof(data8) * 8; idx++) {
        memset(dummy, 0, sizeof(dummy));
        memcpy(outbuf, data8_enc, sizeof(outbuf));
        outbuf[idx>>3] ^= 1 << (idx % 8);
        outsize = sizeof(dummy);

        TEST(hecc_decode(outbuf, sizeof(data8) + 1, dummy, &outsize), true);

        TEST(outsize, sizeof(data8));
        TEST(memcmp(dummy, data8, sizeof(data8)), 0);
    }

    // verify code error
    for (uint32_t idx = 0; idx < 7; idx++) {
        memset(dummy, 0, sizeof(dummy));
        memcpy(outbuf, data8_enc, sizeof(outbuf));
        outbuf[15] ^= 1 << idx;
        outsize = sizeof(dummy);

        TEST(hecc_decode(outbuf, sizeof(data8) + 1, dummy, &outsize), true);

        TEST(outsize, sizeof(data8));
        TEST(memcmp(dummy, data8, sizeof(data8)), 0);
    }

    // verify parity error
    outsize = sizeof(outbuf);

    memset(dummy, 0, sizeof(dummy));
    memcpy(outbuf, data8_enc, sizeof(outbuf));
    outbuf[15] ^= 1 << 7;
    outsize = sizeof(dummy);

    TEST(hecc_decode(outbuf, sizeof(data8) + 1, dummy, &outsize), true);

    TEST(outsize, sizeof(data8));
    TEST(memcmp(dummy, data8, sizeof(data8)), 0);

    return result;
}

bool test_error_double() {
    bool result = true;

    uint8_t outbuf[16] = {0};			// result of encoding
    size_t outsize = sizeof(outbuf);		// length after encoding

    uint8_t dummy[15] = {0};			// write data here to discard

    // verify un-fixable error
    memset(dummy, 0, sizeof(dummy));
    memcpy(outbuf, data8_enc, sizeof(outbuf));
    outbuf[0] ^= 3;
    outsize = 0;

    TEST(hecc_decode(outbuf, sizeof(data8) + 1, dummy, &outsize), false);

    return result;
}

int main(int argc, char** argv) {
    bool result = true;

    result &= test_selftest();

    result &= test_coding_encode();
    result &= test_coding_encode();
    result &= test_coding_roundtrip();

    result &= test_error_single();
    result &= test_error_double();

    return result ? 0 : 1;
}
