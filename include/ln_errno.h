// ln_errno.h
// For __LONG_NUMBER__ version 0.0.1.

#ifndef __LONG_NUMBER_ERRNO_H__
#define __LONG_NUMBER_ERRNO_H__

#define LN_EOFM   -1 	// OUT_OF_MEMORY.
#define LN_EATIZB -2 	// ATTEMPT_TO_INIT_ZERO_BYTES.
#define LN_EATAZB -3 	// ATTEMPT_TO_ALLOCATE_ZERO_BYTES.
#define LN_EATANE -4 	// ATTEMP_TO_ACCES_A_NONEXISTENT_ENTITY.
#define LN_EONP	  -5	// OPERATION_NOT_PERMITTED.
#define LN_EIO	  -6	// ERROR_INPUT_OUTPUT.


#include <stdint.h>
int16_t ln_errno;

#endif //__LONG_NUMBER_ERRNO_H__
