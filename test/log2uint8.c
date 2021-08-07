#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "fast_hamming.h"

#include "test/testing.h"

// Copied from fast_hamming.c as it's otherwise not visible
static inline uint8_t log2uint8(uint8_t v) {
#if 1
    return 31 - __builtin_clz(v);
#else
    register uint8_t r = 0; // result of log2(v) will go here

    register uint8_t bit = 4;
    register uint8_t mask = ~0;

    for (size_t i = 0; i < 3; i++) { // unroll for speed ...
        mask &= (~0u << bit);

        if (v & mask) {
            v >>= bit;
            r |= bit;
        }

        mask >>= bit;
        bit >>= 1;
    }

    return r;
#endif
}

bool test_log2uint8() {
    bool result = true;

    TEST(log2uint8(1), 0);
    TEST(log2uint8(2), 1);
    TEST(log2uint8(3), 1);
    TEST(log2uint8(4), 2);
    TEST(log2uint8(7), 2);
    TEST(log2uint8(8), 3);
    TEST(log2uint8(16), 4);
    TEST(log2uint8(32), 5);
    TEST(log2uint8(64), 6);
    TEST(log2uint8(128), 7);
    TEST(log2uint8(255), 7);

    return result;
}

int main(int argc, char** argv) {
    bool result = true;

    result &= test_log2uint8();

    return result ? 0 : 1;
}
