#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "fast_hamming.h"

#include "test/testing.h"

bool test_dummy() {
    bool result = true;

    TEST(true, 1);

    return result;
}

int main(int argc, char** argv) {
    bool result = true;

    result &= test_dummy();

    return result ? 0 : 1;
}
