/*
 * fast_hamming_test.c
 *
 * Created: 07.08.2021 10:02:12
 *  Author: nqtronix (https://github.com/nqtronix)
 */ 


#include "fast_hamming.h"
#include "fast_hamming_test.h"

//////////////////////////////////////////////////////////////////////////
// Test Data
//////////////////////////////////////////////////////////////////////////

// random data sourced from: https://www.random.org/cgi-bin/randbyte?nbytes=15&format=h
// to format the data correctly, mark the array data and use the following REGEX (excluding ""):
// search:  "([0-9a-f]+)[\W]"
// replace: "0x$1, "
uint8_t data8[15] =
{
	0x0b, 0x28, 0x48, 0x69, 0x5e, 0xc8, 0xeb, 0x87, 0x7f, 0x28, 0xdf, 0x52, 0x03, 0xb4, 0x46, 
};

//////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////////////////////////////////////////////////////

fast_hamming_err_m fast_hamming_test(void)
{
	// test result
	fast_hamming_err_m error = FAST_HAMMING_ERR_NONE;
	
	// tmp storage
	uint8_t dummy [16];						// write data here to discard
	uint8_t outbuf [16] = {0};				// result of encoding
	size_t outsize = sizeof(outbuf);		// length after encoding
	
	// encode data
	error |= (true == hecc_encode(data8, sizeof(data8), outbuf, &outsize)) ? FAST_HAMMING_ERR_NONE : FAST_HAMMING_ERR_ENCODE;
	const uint8_t code = outbuf[15];		// backup of parity data
	
	// verify no error
	outsize = sizeof(outbuf);
	error |= (true == hecc_decode(outbuf, sizeof(data8)+1, dummy, &outsize)) ? FAST_HAMMING_ERR_NONE : FAST_HAMMING_ERR_NOCHANGE;
	
	// verify data error
	outsize = sizeof(outbuf);
	for (uint32_t idx=0; idx < sizeof(data8)*8; idx++)
	{
		data8[idx>>3] ^= 1 << (idx & 0b111);
			
		error |= (true == hecc_decode(outbuf, sizeof(data8)+1, dummy, &outsize)) ? FAST_HAMMING_ERR_NONE : FAST_HAMMING_ERR_DATA;
		if (outbuf[0] != data8[0])
		{
			outbuf[0] = data8[0];
			error |= FAST_HAMMING_ERR_DATA_RES;
		}
	}
	
	// verify code error
	outsize = sizeof(outbuf);
	for (uint32_t idx=0; idx < 7; idx++)
	{
		outbuf[15] ^= 1<<idx;
		error |= (true == hecc_decode(outbuf, sizeof(data8)+1, dummy, &outsize)) ? FAST_HAMMING_ERR_NONE : FAST_HAMMING_ERR_CODE;
		
		if (outbuf[15] != code)
		{
			outbuf[0] = data8[0];
			error |= FAST_HAMMING_ERR_CODE_RES;
		}
	}
	
	// verify parity error
	outsize = sizeof(outbuf);
	outbuf[15] ^= 1<<7;
	error |= (true == hecc_decode(outbuf, sizeof(data8)+1, dummy, &outsize)) ? FAST_HAMMING_ERR_NONE : FAST_HAMMING_ERR_PARITY;
	if (outbuf[0] != data8[0])
	{
		outbuf[0] = data8[0];
		error |= FAST_HAMMING_ERR_PARITY_RES;
	}

	// verify un-fixable error
	outsize = sizeof(outbuf);
	data8[0] ^= 3;
	error |= (true == hecc_decode(outbuf, sizeof(data8)+1, dummy, &outsize)) ? FAST_HAMMING_ERR_NONE : FAST_HAMMING_ERR_DOUBLE;
	
	return error;
}