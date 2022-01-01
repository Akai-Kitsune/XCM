// Version 0.9.5. -> 0.9.6 gamma 
// For Windows, Linux, macOS.
// Unstable bild.
/*
 *  08.04.2021  Update 1 -> Code refactoring.
 *  08.04.2021  Update 2 -> add function: realloc, init, print, free for intenger.
 *  09.04.2021  Update 3 -> refactoring, documenting code.
 *                      main.c :
 *                      add function more, less, equally,
 *                      no equaly, sum, sub. add macros define and macro function.
 *                      ln_errno.h no change.
 *                      bin : folder for different program builds.
 *                      add build0.0.1.exe build0.0.2.exe
 *                      build0.0.3.exe.
 *  18.04.2021  Update 4 -> main.c
 *                      add macro define for boolean type.
 *                      Now the initialization function removes the leading zeros.
 *                      Add function lni_max, lni_min, lni_equaly, lni_noequaly, lni_more,
 *                      lni_less, lni_geqslat, lni_leqslant.
 *                      bin-> add build0.0.4.exe.
 *  30.04.2021 Uptade 5 -> main.c
 *                      Add macro define
 *                      Add function mltp regural number.
 *                      Add extern function division for lni and regural number.
 *  23.09.2021 Uptade 9 -> main.c -> klong_number.h
 *                      Add function mltp for lni.
 *                      Add function devision for lni
 *                      Add lnr and lnr_ alloc, init, print, free
 *                      Add extern for lnr sum, sub, devision, mltp
*/

/*----------------------------------------------------------------------------------------
    Features Update:
    add definitios for lnr sum, sub, devision, mltp 0.2.0
    add fast mltp for lni 0.2.0
    add fourier devision for lni 0.0.3
    add fast devision for lni 0.2.0
----------------------------------------------------------------------------------------*/

#ifndef __LONG_NUMBER_H__
#define __LONG_NUMBER_H__

//CONFIGUARE
//----------------------------------------------------------------------------------------
// Use standard memory allocation
#define __LONG_NUMBER_USING_MALLOC

// Including the required libraries
#define __LONG_NUMBER_INCLUDE_DEFAULT_LIBRARIES

// Tracking the latest error
#define __LONG_NUMBER_TRACKING_ERRORS

// Defolt storage array size increase factor
#define SIZE_MULTIPLICATION 1.5

// Size at standard initialization
#define LN_DEFOLT_SIZE 3

// The coefficient of increasing the number size when reallocating memory
#define LN_DEFOLT_REALLOC 2

// LN_DEFOLT a macro reserved for executing a function with default parameters
#define LN_DEFOLT 0

// BASE of the calculus system
#define LN_BASE 1000000000

// Macro to define uint16_t from <stdint.h>
#define LN_SIZE16 uint16_t

// Macro to define uint32_t from <stdint.h>
#define LN_SIZE32 uint32_t

// Macro to define uint64_t from <stdint.h>
#define LN_SIZE64 uint64_t

// Size of variables for storing the size
#define LN_SIZE_DEFOLT LN_SIZE32

// Define for boolean type
#define LN_BOOL int8_t

// Define for true
#define LN_TRUE  1

// Define for false
#define LN_FALSE 0

// Define for int32_t
#define LN_INT32 int32_t

// Define for uint32_t
#define LN_UINT32 uint32_t

// Define for int64_t
#define LN_INT64 int64_t

// Define for uint64_t
#define LN_UINT64 uint64_t

// Define for int8_t
#define LN_INT8 int8_t

// Define for lni_alloc size
#define LN_128BITS 5
#define LN_256BITS 9
#define LN_512BITS 18
#define LN_1024BITS 35
#define LN_2048BITS 69
#define LN_4096BITS 138
#define LN_8192BITS 275
#define LN_16384BITS 549
#define LN_32768BITS 1097
#define LN_65536BITS 2193
#define LN_131072BITS 4384
#define LN_262144BITS 8769
#define LN_524288BITS 17537
#define LN_1048576BITS 35073

// Macro funtion for max two standart numbers
#define LN_MAX(X, Y) (X >= Y ? X : Y)

// Macro funtion for min two standart numbers
#define LN_MIN(X, Y) (X <= Y ? X : Y)

// Macro function for easy create ln number
#define intn(name, size, str) lni* name = lni_alloc(size);lni_init(name, str);




// Abbreviated name for long_number_intenger
//#define long_number_intenger lni        

//----------------------------------------------------------------------------------------

#if defined(__LONG_NUMBER_INCLUDE_DEFAULT_LIBRARIES)
    //For different data types.
    #include <stdint.h> 
    //For output.   
    #include <stdio.h>
    //For working with strings.     
    #include <string.h> 
    //For working with memory.  
    #include <stdlib.h> 
    //For math.
    #include <math.h>
    #if defined __LONG_NUMBER_TRACKING_ERRORS
        //For tracking errors.
        #include "ln_errno.h"
        //#include "logger.h"
    #endif
#endif 

typedef struct long_number_intenger
{
    // For sign of number
    int8_t   _sign;
    // To store a number
    int32_t* _mem;
    // Size of number.
    uint32_t _size;
    // Last index number.
    uint32_t _current_number;
}lni;

typedef struct long_number_real
{
    // Size of number.
    uint32_t _size;
    // For sign of number
    int8_t _sign;
    // For number in [0, 1)
    int32_t* _mem;
    // _mem * 10**(-exponent)
    int64_t _exponent;
    // Last index intenger number.
    uint32_t _current_number;
    
}lnr;

typedef struct ln_tuple{
    void** _mem;
}ln_tuple;

// LONG_NUMBER_INTENGER
//----------------------------------------------------------------------------------------

#ifdef __LONG_NUMBER_USING_MALLOC

// Allocate memory for the new number, total of size blocks, each 32 bits.
// Returns a pointer to the allocated memory area or NULL.
lni* lni_alloc(uint32_t size);

// Reallocate memory. Returns a pointer to the reallocated memory area or NULL.
lni* lni_realloc(lni* number, uint32_t size);

// Initializes a number from the input string(base 10), for which the memory was previously allocated.
void lni_init(lni* number, const char* str);

// Initializes a number from the input string with base, for which the memory was previously allocated.
void lni_init_from_base(lni* number, char* str);

// Merge to lni number result = left...right
lni* lni_merge(lni* result, lni* left, lni* right);

// Freeing up previously allocated memory.
void lni_free(lni* number);

#endif //__LONG_NUMBER_USING_MALLOC

// Print the number in the console.
void lni_print(lni* number);

// left == right
LN_BOOL lni_equally(lni* left, lni* right);

// left != right
LN_BOOL lni_noequally(lni* left, lni* right);

// left > right
LN_BOOL lni_more(lni* left, lni* right);

// left < right
LN_BOOL lni_less(lni* left, lni* right);

// left >= right
LN_BOOL lni_geqslant(lni* left, lni* right);

// left <= right
LN_BOOL lni_leqslant(lni* left, lni* right);

// Returns a pointer to a larger number or NULL.
lni* lni_max(lni* left, lni* right);

// Returns a pointer to a smaller number or NULL.
lni* lni_min(lni* left, lni* right);

// The addition of two numbers.
lni* lni_sum(lni* result, lni* left, lni* right);

// Subtraction of two numbers.
lni* lni_sub(lni* result, lni* left, lni* right);

// Multiplication of two numbers.
lni* lni_mltp(lni* result, lni* left, lni* right);

// Multiplying a large number by a small number. right < base
lni* lni_mltp_num(lni* result, lni* left, LN_UINT64 right);

// Division of two numbers.
lni* lni_devision(lni* result,lni* left, lni* right);

// Return 1/left 
lnr* lni_devision_reverse_fourier(lnr* result, lni* right);

// Return 1/left 
lnr* lni_devision_reverse(lnr* result, lni* right); 

// Division a large number by a small number. right < base
lni* lni_devision_num(lni* result, lni* left, LN_INT64 right);

// Division a large number by a small number. right < base return <<result, carry>>
ln_tuple* lni_devision_num_wcarry(lni * result, lni * left, LN_INT64 right);

// End long_number_intenger
//----------------------------------------------------------------------------------------

// LONG_NUMBER_REAL
//----------------------------------------------------------------------------------------
lnr* lnr_alloc(LN_UINT32 size);

lnr* lnr_realloc(lnr* number, LN_UINT32 new_size);

lnr* lnr_init(lnr* number, const char* str, int64_t exponent);

LN_INT64 fpow(LN_INT64 number, LN_INT64 exponent);

void lnr_print(lnr* number);

// left == right
LN_BOOL lnr_equally(lnr* left, lnr* right);

// left != right
LN_BOOL lnr_noequally(lnr* left, lnr* right);

// left > right
LN_BOOL lnr_more(lnr* left, lnr* right);

// left < right
LN_BOOL lnr_less(lnr* left, lnr* right);

// left >= right
LN_BOOL lnr_geqslant(lnr* left, lnr* right);

// left <= right
LN_BOOL lnr_leqslant(lnr* left, lnr* right);

// Returns a pointer to a larger number or NULL.
lni* lnr_max(lnr* left, lnr* right);

// Returns a pointer to a smaller number or NULL.
lni* lnr_min(lnr* left, lnr* right);

// Return left + right. O(n)
lnr* lnr_sum(lnr* result, lnr* left, lnr* right);

// Return left - right. O(n)
lnr* lnr_sub(lnr* result, lnr* left, lnr* right);

// Return left*right. O(n^2)
lnr* lnr_mltp(lnr* result, lnr* left, lnr* right);

// Return left*right where right < LN_BASE. O(n)
lnr* lnr_mltp_num(lnr* result, lnr* left, LN_INT32 right);

// Division of two numbers. 
lnr* lnr_devision(lnr* result,lnr* left, lnr* right);

// Return 1/left 
lnr* lnr_devision_reverse_fourier(lnr* result, lnr* right);

// Return 1/left 
lnr* lnr_devision_reverse(lnr* result, lnr* right); 

// Division a large number by a small number. right < base
lnr* lnr_devision_num(lnr* result, lnr* left, LN_INT64 right);

// Division a large number by a small number. right < base return <<result, carry>>
ln_tuple* lnr_devision_num_wcarry(lnr * result, lnr * left, LN_INT64 right);

// Tuple
//----------------------------------------------------------------------------------------
ln_tuple* ln_tuple_create(void** a);

void ln_tuple_free(ln_tuple* tpl);
//----------------------------------------------------------------------------------------

#endif
