#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define TEST(EXPR, EXPECTED) \
    do { \
        printf("Testing %s ... ", #EXPR); \
        const int r = EXPR, v = EXPECTED; \
        if(r == v) { \
            printf("OK\n"); \
        } else { \
            printf("FAIL: %s != %s (%d != %d)\n", #EXPR, #EXPECTED, r, v); \
            result = false; \
        }; \
    } while(0)
