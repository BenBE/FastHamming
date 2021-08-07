/*
 * main.c
 *
 * Created: 07.08.2021 09:48:38
 * Author : nqtronix (https://github.com/nqtronix)
 */ 

#include "sam.h"
#include "fast_hamming.h"
#include "fast_hamming_test.h"

int main(void)
{
	// run test routine
	volatile fast_hamming_err_m result = fast_hamming_test();

	// stop point for debugger
	asm("nop");

    while (1);
}
