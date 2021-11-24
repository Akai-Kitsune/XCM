//----------------------------------------------------------------------------------------
/*
    <LICENSE>
    library for work with long number.
    Copyright (C) 2021 Aleksei Likhachev. All right reserved.
 
    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See the KI General Public License version for more details.
*/
//----------------------------------------------------------------------------------------
//
//  xlong_obj.h
//  Xenon CM

// Version 0.1.9
// 2fbee84e70c686a6565397f739b57bb2d0b8a7b8

#ifndef __XLONG_OBJECT_H__
#define __XLONG_OBJECT_H__

// Config
// The base of the number system.
#define _XLN_BASE 0x40000000
// A mask to check the number for overflow. Not used
#define _XLN_MASK_OVERFLOW 0x80000000
// A mask to cut off a number modulo the base of the system.
#define _XLN_MASK 0x3fffffff
// A constant to shift the number by thirty bits.
#define _XLN_SHIFT 0x1e
// A type for true
#define LN_TRUE 1
// A type for false
#define LN_FALSE 0
// A type for bool
#define lnbool int8_t
// For syntaxis
#define xln xln
// Flag to connect raw libraries in a standard way.
#define __XLN_INCLUDE_DEFOLT_LIB
// Flag to enable the error output function.
#define _XLN_PRINT_ERROR
// Flag to enable your own processing core.
#define _XLN_XPU_OFF
// Flag to enable the different parts for debugging. Not used.
#define _XLN_DEBUG
// Flag to output the digits of the numbers on the different lines.
#define _XLN_PRINT_NUM_BY_NUM1
// Function to initialize the kernel.
#define _XLN_XPU_INIT x_kernel* xpu_kernel = (x_kernel*)xk_create(1, NULL)
// Function for calling the kernel.
#define _XLN_CALL(func, data, xln_kernel) xk_calculate(xln_kernel, func, 1, data)
// The function returns the largest size of the two numbers.
#define _XLN_MAX_SIZE(left, right) (left->_current > right->_current ? left->_current : right->_current)
// The function call prints the error and returns the error code.
#define error(str, id) printError(str, id)
// The function call prints the warning and returns the error code.
#define warning(str, id) printError(str, id)
// The function prints str and id
#define printError(str, id) printf(str);

#ifdef _XLN_DEBUG
#   define SUMDEBUGOFF
#   define SUBDEBUGOFF
#   define SMULINTDEBUGOFF
#   define SMULDEBUGOFF
#   define SDIVDEBUG
#endif

#ifndef _XLN_PRINT_ERROR
#   undef printError
#   define printError(str, id)
#endif

#ifdef __XLN_INCLUDE_DEFOLT_LIB
    // For printf and scanf.
#   include <stdio.h>
    // For malloc, realloc.
#   include <stdlib.h>
    // For any type int.
#   include <stdint.h>
    // For string, memcpy, memmove.
#   include <string.h>
#   ifdef _XLN_INCLUDE_ERRNO
        // For math function
#       include <math.h>
#   endif
#   ifdef _XLN_XPU
#   include "kernel.h"
#   endif

#endif

/*
 Our integer is stored in a structure with "size" fields to indicate the allocated memory.
 "_сurrent" to indicate the last occupied cell. And "_mem" to store the digits of the number in
 the number system modulo 2^32.
 */
typedef struct xlong_number{
    uint32_t _size;
    uint32_t _current;
    int8_t _sign;
    uint32_t* _mem;
}xln;

/*
 This is a tuple of data for storing various objects and passing them around. The "_mem"
 stores pointers to pointers to the object.
 */
typedef struct ln_tuple{
    void** _mem;
}ln_tuple;

// MEMORY
//----------------------------------------------------------------------------------------

/*
 This function allocates memory for the structure and "_size" cells to store it.
 returns a pointer to the allocated memory or NULL in case of an error
 */
xln* xln_alloc(uint32_t size);

/*
 This function reallocates memory for the structure and "_size" cells to store it.
 returns a pointer to the allocated memory or NULL in case of an error
 */
xln* xln_realloc(xln* obj, uint32_t new_size);

/*
 This function frees the previously allocated memory for the object
 */
void xln_free(xln* obj);

//----------------------------------------------------------------------------------------

// NUMBER
//----------------------------------------------------------------------------------------

/*
 This function normalizes the number by removing leading zeros from it.
 */
xln* xln_normalize(xln* obj);

/*
 This function converts a number obtained from a string by converting it from the modulo 10^9
 number system to the modulo 2^30 number system.
 */
xln* xln_convert(xln* obj, uint64_t a[], uint32_t n);

/*
 This function converts a number obtained from a string by converting it from the modulo 2^30
 number system to the modulo 10^9 number system.
 */
xln* xln_convert_reverse(xln* obj);

/*
 This function initializes xln with a number from the string modulo 10^9,
 normalizes it with the xln_norm function and returns a pointer to the object otherwise NULL
 */
xln* xln_init_string(xln* obj, const char* str);

/*
 This function initializes xln with a number from int,
 normalizes it with the xln_norm function and returns a pointer to the object otherwise NULL
 */
xln* xln_init_int(xln* obj, int32_t val);

/*
 This function outputs to the console a large number modulo 2^30
 */
void xln_print(xln* obj);

/*
 This function outputs to the console a large number modulo 10^9
 */
void xln_print_r(xln* obj);

//----------------------------------------------------------------------------------------

// ARITHMETIC
//----------------------------------------------------------------------------------------

/*
 This is a function that adds two large numbers element by element. If the pointer to the result is
 zero, it allocates memory for it. It returns a pointer to the sum of the two numbers or NULL in
 case of an error.
 
 left->_mem[i] + right->_mem[i] + carry <= (2^30 - 1) + (2^30 - 1) + 1 = 2^31 -1 < 2^31
 
 The addition algorithm is as follows:
 1. Assign j , carry <- 0 where j is the bit index and carry the overflow index.
 2. Add the digits modulo the system and calculate the overflow. [a[j] + b[j] + carry / base ]
 3. Increment j by one, if we have passed all digits, assign the last value of the remainder and complete the execution.
 O(log(a, 2^30))
 */
xln* xln_sum(xln* result, xln* left, xln* right);

/*
 This function subtracts one number from another with a sign. If no memory has been allocated for
 the resulting
 number, it is allocated and returns the result or NULL in case of an error.
 */
xln* xln_sub(xln* result, xln* left, xln* right);

/*
 A function for multiplying a large number by a small number smaller than the base of the system.
 If no memory has
 been allocated for the result, allocate it. Returns the result or zero in case of an error.
 */
xln* xln_smul_int(xln* result, xln* left, uint32_t right);

/*
 Slow multiplication function of two large numbers.
 Automatically allocates memory for the result if the pointer is
 zero. Returns the result or zero in case of an error. The asymptotic of the algorithm is equal to
 the product of their lengths n and m O(n*m). Suitable for small numbers.
 */
xln* xln_smul(xln* result, xln* left, xln* right);

/*
 karatsube mltp
 */
xln* xln_mul(xln* result, xln* left, xln* right);

/*
 slow div by int < base.
 */
xln* xln_sdiv_int(xln* result, xln* left, uint32_t right);

/*
 Inefficient division algorithm. Suitable for small numbers.
 
 The idea is that if the digit in the high-order is greater than half the base of the system, then
 q = (l[j+n]*base + l[j+n-1])/r[n-1] is between [
 q_true, q_true + 2]. The division process itself is the division of l[j:j+n] by r, where j =
 l.size-r.size. We literally slide over the divisor.

 Rough estimate of algorithm's complexity is O(n^2) where n is number of words in divisible.
 */
xln* xln_sdiv(xln* result, xln* left, xln* right);

/*
 An algorithm for finding the remainder modulo n based on the slow division
 algorithm for large numbers. Returns a pointer to the result or zero in case of error.
 
 Rough estimate of algorithm's complexity is O(n^2) where n is number of words in divisible.
 */
xln* xln_smod(xln* result, xln* left, xln* right);

//----------------------------------------------------------------------------------------

// COMPARISON
//----------------------------------------------------------------------------------------
 
/*
 The function compares two numbers, starting from the highest digit.
 Returns 1 if the numbers are equal otherwise zero.
 */
lnbool xln_equal(xln* left, xln* right);

/*
 The function compares two numbers, starting from the highest digit.
 Returns 1 if the numbers are not equal otherwise zero.
 */
lnbool xln_noequal(xln* left, xln* right);

/*
 The function compares two numbers starting with the highest digit.
 Returns 1 if the left number is greater than or equal to the right number, otherwise zero.
 */
lnbool xln_eqmore(xln* left, xln* right);

/*
 The function compares two numbers starting with the highest digit.
 Returns 1 if the left number is less than or equal to the right number, otherwise zero.
 */
lnbool xln_eqless(xln* left, xln* right);

/*
 The function compares two numbers starting with the highest digit.
 Returns 1 if the left number is greater than the right number otherwise zero.
 */
lnbool xln_more(xln* left, xln* right);

/*
 The function compares two numbers starting with the highest digit.
 Returns 1 if the left number is less than the right number otherwise zero.
 */
lnbool xln_less(xln* left, xln* right);

//----------------------------------------------------------------------------------------


// TEST
//----------------------------------------------------------------------------------------

void test_sum(void);

void test(void);

//----------------------------------------------------------------------------------------

#endif /* __XLONG_OBJECT_H__ */
