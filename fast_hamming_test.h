/*
 * fast_hamming_test.h
 *
 * Created: 07.08.2021 10:02:23
 *  Author: nqtronix (https://github.com/nqtronix)
 */ 


#ifndef FAST_HAMMING_TEST_H_
#define FAST_HAMMING_TEST_H_


//////////////////////////////////////////////////////////////////////////
// Enums & Defines
//////////////////////////////////////////////////////////////////////////

typedef enum
{
	FAST_HAMMING_ERR_NONE		= 0,
	FAST_HAMMING_ERR_ENCODE		= 1<<0,		// encoding failed
	FAST_HAMMING_ERR_NOCHANGE	= 1<<1,		// decoding of unchanged data failed
	FAST_HAMMING_ERR_DATA		= 1<<2,		// decoding of 1 bit error in data failed
	FAST_HAMMING_ERR_DATA_RES	= 1<<3,		// decoding of 1 bit error in data was not correctly restored
	FAST_HAMMING_ERR_CODE		= 1<<4,		// decoding of 1 bit error in code failed
	FAST_HAMMING_ERR_CODE_RES	= 1<<5,		// decoding of 1 bit error in code was not correctly restored
	FAST_HAMMING_ERR_PARITY		= 1<<6,		// decoding of 1 bit error in parity failed
	FAST_HAMMING_ERR_PARITY_RES	= 1<<7,		// decoding of 1 bit error in parity was not correctly restored
	FAST_HAMMING_ERR_DOUBLE		= 1<<8,		// decoding of 2 bit errors was NOT detected
} fast_hamming_err_m;	// *_m = mask; multiple values can be joined with a binary or

//////////////////////////////////////////////////////////////////////////
// Function Declarations
//////////////////////////////////////////////////////////////////////////

fast_hamming_err_m fast_hamming_test(void);



#endif /* FAST_HAMMING_TEST_H_ */