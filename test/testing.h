#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define TEST(EXPR, EXPECTED) \
    do { \
        printf("Testing \e[1m%s\e[0m ... ", #EXPR); \
        const int r = EXPR, v = EXPECTED; \
        if(r == v) { \
            printf("\e[1;32mOK\e[0m\n"); \
        } else { \
            printf("\e[1;31mFAIL\e[0m: %s != %s \e[1m(\e[31m%d\e[0;1m != \e[32m%d\e[0;1m)\e[0m\n", #EXPR, #EXPECTED, r, v); \
            result = false; \
        }; \
    } while(0)
