#include "fast_hamming.h"

__attribute__((nonnull(1,2)))
static inline void hecc_encode_impl(const uint8_t *inbuf, uint8_t *outbuf, size_t insize) {
    uint8_t p1 = 0;
    uint8_t p2 = 0;

    size_t power2 = 4;
    uint8_t data = 0;

    insize *= 8;

    for(size_t i = 3, j = 0; j < insize; i++) {
        if(power2 == i) {
            power2 *= 2;
            continue;
        }

        if(0 == j % 8) {
            *outbuf++ = data = *inbuf++;
            p2 ^= __builtin_parity(data);
        }

        if(data & 1) {
            p1 = p1 ^ (uint8_t)i;
        }
        data >>= 1;

        j++;
    }

#if 1
    p2 ^= __builtin_parity(p1);
#else
    p2 ^= p1;
    p2 ^= p2 >> 4;
    p2 ^= p2 >> 2;
    p2 ^= p2 >> 1;
    p2 &= 1;
#endif

    *outbuf = p1 + (p2 << 7);
}

__attribute__((nonnull(1,3,4)))
bool hecc_encode(const uint8_t *inbuf, size_t insize, uint8_t *outbuf, size_t *outsize) {
    if(insize > (~(size_t)0 >> 4) * 15) {
        *outsize = 0;
        return false;
    }

    size_t reqsize = insize + 14;	//Round up to next full block
    reqsize /= 15;			//Number of ECC data
    reqsize += insize;			//Number of total bytes

    if(*outsize < reqsize) {
        *outsize = reqsize;
        return false;
    }

    *outsize = 0;

    while(insize >= 15) {
        hecc_encode_impl(inbuf, outbuf, 15);

        inbuf += 15;
        insize -= 15;

        outbuf += 16;
        *outsize += 16;
    }

    if(insize) {
        uint8_t buf[15] = {0};

        for(size_t i = 0; i < insize; i++) {
            buf[i] = inbuf[i];
        }

        hecc_encode_impl(buf, outbuf, insize);

        inbuf += insize;

        outbuf += insize + 1;
        *outsize += insize + 1;
    }

    return true;
}

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

__attribute__((nonnull(1,2)))
static inline bool hecc_decode_impl(const uint8_t *inbuf, uint8_t *outbuf, size_t insize) {
    if(insize < 2) {
        return false;
    }

    uint8_t p1 = 0;
    uint8_t p2 = 0;

    size_t power2 = 4;
    uint8_t data = 0;

    uint8_t check = inbuf[--insize];

    for(size_t i = 0; i < insize; i++) {
        outbuf[i] = inbuf[i];
    }

    insize *= 8;

    for(size_t i = 3, j = 0; j < insize; i++) {
        if(power2 == i) {
            power2 *= 2;
            continue;
        }

        if(0 == j % 8) {
            data = *inbuf++;
            p2 ^= __builtin_parity(data);
        }

        if(data & 1) {
            p1 = p1 ^ (uint8_t)i;
        }
        data >>= 1;

        j++;
    }

    p1 ^= check & 0x7f;

    if(p1) {
        //Check overall message parity
        uint8_t pm = check;
#if 1
        p2 ^= __builtin_parity(pm);
#else
        pm ^= pm >> 4;
        pm ^= pm >> 2;
        pm ^= pm >> 1;
        pm &= 1;
        p2 ^= pm;
#endif

        if(!p2) {
            // At least two errors ...
            return false;
        }

        //Check for non-power of two in p1:
        //A power of two means one of the parity bits that's wrong
        if(0 != (p1 & (p1 - 1))) {
            // Calculate error location
            p2 = p1 - log2uint8(p1) - 2;
            pm = p2 / 8;
            p2 = p2 % 8;

            //Actually fix that error
            outbuf[pm] ^= 1 << p2;
        }
    }

    return true;
}

__attribute__((nonnull(1,3,4)))
bool hecc_decode(const uint8_t *inbuf, size_t insize, uint8_t *outbuf, size_t *outsize) {
    if(insize % 16 == 1) {		//This indicates a truncated message
        *outsize = 0;
        return false;
    }

    size_t reqsize = insize / 16;	//Number of full blocks
    reqsize += !!(insize % 16);		//Partial block
    reqsize = insize - reqsize;		//Remove ECC info from input size

    if(*outsize < reqsize) {
        *outsize = reqsize;
        return false;
    }

    *outsize = 0;

    while(insize >= 16) {
        if(!hecc_decode_impl(inbuf, outbuf, 16)) {
            return false;
        }

        inbuf += 16;
        insize -= 16;

        outbuf += 15;
        *outsize += 15;
    }

    if(insize) {
        uint8_t buf[16] = {0};

        if(!hecc_decode_impl(inbuf, buf, insize)) {
            return false;
        }

        for(size_t i = 0; i < insize - 1; i++) {
            outbuf[i] = buf[i];
        }

        inbuf += insize;

        outbuf += insize - 1;
        *outsize += insize - 1;
    }

    return true;
}
