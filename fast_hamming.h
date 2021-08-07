/*
 * fast_hamming_test.h
 *
 * Created: 05.09.2020
 *  Author: BenBE (https://github.com/BenBE)
 */ 


#ifndef FAST_HAMMING_H_
#define FAST_HAMMING_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


//////////////////////////////////////////////////////////////////////////
// Function Declarations
//////////////////////////////////////////////////////////////////////////

// encodes any input data. You must specify the maximum size of the output buffer, after encoding
// this value will be update to reflect the used space. 'insize' and 'outsize' are in bytes
// TODO: explain all parameters and the return value!
__attribute__((nonnull(1,3,4)))
bool hecc_encode(const uint8_t *inbuf, size_t insize, uint8_t *outbuf, size_t *outsize);

__attribute__((nonnull(1,3,4)))
bool hecc_decode(const uint8_t *inbuf, size_t insize, uint8_t *outbuf, size_t *outsize);


#endif /* FAST_HAMMING_H_ */