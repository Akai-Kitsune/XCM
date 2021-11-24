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

#include "xlong_obj.h"

// MEMORY
//----------------------------------------------------------------------------------------

/*
 This function allocates memory for the structure and "_size" cells to store it.
 returns a pointer to the allocated memory or NULL in case of an error
 */
xln* xln_alloc(uint32_t size){
    if(size == 0){
        error("ATTEMPT_TO_ALLOCATE_ZERO_BYTES", -3);
        return NULL;
    }
    else {
        static xln* obj;
        obj = (xln*)malloc(sizeof(xln));
        obj->_mem = (uint32_t*)malloc(size*(sizeof(uint32_t)));
        obj->_size = size;
        obj->_sign = 1;
        obj->_current = 0;
        
        // We need it?
        while(size > 0){
            obj->_mem[size-1] = 0;
            --size;
        }
        return obj;
    }
}

/*
 This function reallocates memory for the structure and "_size" cells to store it.
 returns a pointer to the allocated memory or NULL in case of an error
 */
xln* xln_realloc(xln* obj, uint32_t new_size){
    if(obj != NULL){
        obj->_mem = realloc(obj->_mem, new_size);
        return obj;
    }
    else{
        error("EATANE\n", -4);
        return NULL;
    }
}

/*
 This function frees the previously allocated memory for the object
 */
void xln_free(xln* obj){
    if(obj != NULL){
//        free(obj->_mem);
        free(obj);
    }
    else {
        error("EATANE", -4);
    }
}

//----------------------------------------------------------------------------------------

// NUMBER
//----------------------------------------------------------------------------------------

/*
 This function normalizes the number by removing leading zeros from it.
 */
xln* xln_normalize(xln* obj){
    if(obj != NULL){
        while((obj->_current > 1) && (obj->_mem[obj->_current-1] == 0)){
            obj->_current -= 1;
        }
        return obj;
    }
    else {
        error("Attempt to normalize NULL obj", 1);
        return NULL;
    }
}

/*
 This function converts a number obtained from a string by converting it from the modulo 10^9
 number system to the modulo 2^30 number system.
 */
xln* xln_convert(xln* obj, uint64_t a[], uint32_t n){
    if(obj != NULL && a != NULL){
        uint64_t current = 0;
        uint64_t carry = 0;
#       define pre_base 1000000000
        obj->_current = 0;
        for(int32_t i = 0; i < n; ++i){
            carry = 0;
            for(int32_t j = n-1; j >= 0; --j){
                //printf("in f a j = %llu\n", a[j]);
                current = a[j] + carry*pre_base;
                a[j] = current >> _XLN_SHIFT;
                carry = (current % (_XLN_BASE));
            }
            obj->_mem[i] = (uint32_t)carry;
            obj->_current += 1;
        }
        xln_normalize(obj);
        return obj;
    }
    else {
        // error ATNE
        return NULL;
    }
}

xln* xln_convert_reverse(xln* obj){
    if(obj != NULL){
        uint64_t current = 0;
        uint64_t carry = 0;
        uint32_t n = obj->_current;
        uint64_t a[n];
        //memcpy(a, obj->_mem, sizeof(obj->_mem));
        for(int i = 0; i < n; ++i){
            a[i] = obj->_mem[i];
            //printf("a= %llu\n", i);
        }
#       define pre_base 1000000000
        obj->_current = 0;
        for(int32_t i = 0; i <= n; ++i){
            carry = 0;
            for(int32_t j = n-1; j >= 0; --j){
                current = a[j] + (carry<<30);
                a[j] = (uint64_t)(current / pre_base);
                carry = (current % (pre_base));
            }
            obj->_mem[i] = (uint32_t)carry;
            obj->_current += 1;
        }

        xln_normalize(obj);
        return obj;
    }
    else {
        // error ATNE
        return NULL;
    }
}

/*
 This function initializes xln with a number from the string modulo 10^9,
 normalizes it with the xln_norm function and returns a pointer to the object otherwise NULL
 */
xln* xln_init_string(xln* obj, const char* str){
    if(str != NULL && obj != NULL){
        uint32_t size_str = (uint32_t)(strlen(str));
        int32_t j = 0;
        uint64_t a[(size_str) + 1];
        char tmp[(size_str)];
        
        for (int k = 0; k < size_str; ++k) {
            tmp[k] = *(str + k);
        }
        
        for(int32_t i = (int32_t)(size_str); i > 0; i -= 9, ++j){
            //memcpy(&tmp, i >= 9 ? (str + i - 9) : str, 9*sizeof(char));
            
            tmp[i] = '\0';
            a[j] = (uint64_t)atol(i >= 9 ? tmp + i - 9 : tmp);
        }
        return xln_convert(obj, a, j);
    }
    else if(obj == NULL){
        static xln* obj;
        obj = (xln*)malloc(sizeof(xln));
        return obj;
    }
    else{
        return NULL;
    }
}

xln* xln_init_int(xln* obj, int32_t val){
    if(obj != NULL){
        obj->_mem[0] = val >= 0 ? val : -val;
        obj->_sign = val >= 0 ? 1 : -1;
        obj->_current += 1;
        return obj;
    }
    else {
        error("Attempt to init NULL obj", -1);
        return NULL;
    }
}

/*
 This function outputs to the console a large number modulo 2^30
 */
void xln_print(xln* obj){
    if(obj != NULL){
       // uint32_t a[obj->_current];
#       ifdef _XLN_PRINT_NUM_BY_NUM
        if(obj->_sign == -1){
            printf("-");
        }
        printf("%d\n", obj->_current == 0 ? 0 : (obj->_mem[obj->_current-1]));
        for(int32_t i = obj->_current-2; i >= 0; --i){
            printf("%010d\n", (obj->_mem[i]));
        }
        printf("\n");
        
#       else
        if(obj->_sign == -1){
            printf("-");
        }
        printf("%d", obj->_current == 0 ? 0 : (obj->_mem[obj->_current-1]));
        for(int32_t i = obj->_current-2; i >= 0; --i){
            printf("%010d", (obj->_mem[i]));
        }
        printf("\n");
#       endif
    }
    else {
        error("obj = null\n", -1);
    }
}

/*
 This function outputs to the console a large number modulo 10^9
 */
void xln_printr(xln* obj){
    if(obj != NULL){
       // uint32_t a[obj->_current];
#       ifdef _XLN_PRINT_NUM_BY_NUM
        if(obj->_sign == -1){
            printf("-");
        }
        printf("%d\n", obj->_current == 0 ? 0 : (obj->_mem[obj->_current-1]));
        for(int32_t i = obj->_current-2; i >= 0; --i){
            printf("%09d\n", (obj->_mem[i]));
        }
        printf("\n");
        
#       else
        if(obj->_sign == -1){
            printf("-");
        }
        printf("%d", obj->_current == 0 ? 0 : (obj->_mem[obj->_current-1]));
        for(int32_t i = obj->_current-2; i >= 0; --i){
            printf("%09d", (obj->_mem[i]));
        }
        printf("\n");
#       endif
    }
    else {
        error("obj = null\n", -1);
    }
}
//----------------------------------------------------------------------------------------

// ARITHMETIC
//----------------------------------------------------------------------------------------

/*
 This is a function that adds two large numbers element by element. If the pointer to the result is zero, it allocates
 memory for it. It returns a pointer to the sum of the two numbers or NULL in case of an error.
 
 Carry is the Overflow Indicator, takes values from the ring of deductions modulo two
 left->_mem[i] + right->_mem[i] + carry <= (2^30 - 1) + (2^30 - 1) + 1 = 2^31 -1 < 2^31
 
 The addition algorithm is as follows:
 1. Assign j , carry <- 0 where j is the bit index and carry the overflow index.
 2. Add the digits modulo the system and calculate the overflow. [a[j] + b[j] + carry / base ]
 3. Increment j by one, if we have passed all digits, assign the last value of the remainder and complete the execution.
 */
xln* xln_sum(xln* result, xln* left, xln* right){
    if(left != NULL && right != NULL){
        if(left->_sign == -1 && right->_sign == -1){
            result->_sign = -1;
        }
        if(left->_size <= right->_size){
            uint32_t i = 0;
            uint32_t carry = 0;
            if(result == NULL){
                result = xln_alloc(right->_size+1);
            }
            for(i = 0; i < left->_current; ++i){
                carry += left->_mem[i] + right->_mem[i];
                result->_mem[i] = carry & (_XLN_MASK);
                carry >>= _XLN_SHIFT;
            }
            for(; i < right->_current; ++i){
                carry += right->_mem[i];
                result->_mem[i] = carry & (_XLN_MASK);
                carry >>= _XLN_SHIFT;
            }
            result->_current = i;
            result->_mem[i] = carry;
            return xln_normalize(result);
        }
        else {
            return xln_sum(result, right, left);
        }
        return result;
    }
    else {
        error("ATNE", -4);
        return NULL;
    }
}

/*
 This function subtracts one number from another with a sign. If no memory has been allocated for the resulting
 number, it is allocated and returns the result or NULL in case of an error.
 
 Carry is the Overflow Indicator, takes values from the -(Z/2Z)*
 left->_mem[i] - right->_mem[i] + carry <= (2^30 - 1) - 0 + 0  < 2^31
 
 The addition algorithm is as follows:
 1. Assign j , carry <- 0 where j is the bit index and carry the overflow index.
 2. Sub the digits modulo the system and calculate the overflow. [a[j] - b[j] + carry / base ]
 3. Increment j by one, if we have passed all digits, assign the last value of the remainder and complete the execution.
 */
xln* xln_sub(xln* result, xln* left, xln* right){
    if(left != NULL && right != NULL){
        int32_t i = 0;
        int64_t carry = 0;
        if(result == NULL){
            result = xln_alloc(left->_size);
        }
        
        else if(left->_size == right->_size){
            i = left->_current;
            while((--i >= 0) && (left->_mem[i] == right->_mem[i])){}
            if(i < 0){
                result->_mem[0] = 0;
                result->_current = 1;
                return result;
            }
            if(left->_mem[i] < right->_mem[i]){
                result->_sign = -1;
                return xln_sub(result, right, left);
            }
            left->_size = right->_size = i+1;
        }
        for(i = 0; i < right->_current; ++i){
            carry = (int64_t)(left->_mem[i] - right->_mem[i] - carry);
            result->_mem[i] = carry & (_XLN_MASK);
            carry = (carry >> _XLN_SHIFT) & 1;
        }
        for(; i < left->_current; ++i){
            carry = left->_mem[i] - carry;
            result->_mem[i] = carry & (_XLN_MASK);
            carry = (carry >> _XLN_SHIFT) & 1;
        }
        result->_current = i;
        result->_mem[i] = (uint32_t)carry;
        return result;
    }
    else{
        error("ATANE", -4);
        return NULL;
    }
}

/*
 A function for multiplying a large number by a small number smaller than the base of the system. If no memory has
 been allocated for the result, allocate it. Returns the result or zero in case of an error.
 */
xln* xln_smul_int(xln* result, xln* left, uint32_t right){
    if(left != NULL ){
        uint64_t max_size = (left->_current +1);
        if(result == NULL){
            result = xln_alloc((uint32_t)max_size);
            return xln_smul_int(result, left, right);
        }
        
        uint64_t carry = 0;
        uint64_t current;
        uint32_t j;
        
        for(j = 0; j < left->_current; ++j){
            current = left->_mem[j]*1ull*right + carry;
            result->_mem[j] = current & _XLN_MASK;
            carry = current >> _XLN_SHIFT;
        }
        
        if(carry != 0){
            result->_mem[j] = (uint32_t)carry;
            result->_current = left->_current + 1;
        }
        
        return xln_normalize(result);
    }
    else{
        error("ATANE\n", -4);
        return NULL;
    }
}

/*
 Slow multiplication function of two large numbers. Automatically allocates memory for the result if the pointer is
 zero. Returns the result or zero in case of an error. The asymptotic of the algorithm is equal to the product of
 their lengths n and m O(n*m). Suitable for small numbers.
 
 
 */
xln* xln_smul(xln* result, xln* left, xln* right){
    if(left != NULL && right != NULL){
        uint64_t max_size = (left->_current * right->_current);
        if(result == NULL){
            result = xln_alloc((uint32_t)max_size);
            return xln_smul(result, left, right);
        }
        
        for(uint32_t i = 0; i <= max_size; ++i){
            result->_mem[i] = 0;
        }
        
        uint64_t carry;
        uint64_t current;
        uint32_t i, j;
        for(i = 0; i < left->_current; ++i){
            carry = 0;
            for(j = 0; j < right->_current; ++j){
                current = left->_mem[i]*1ull*right->_mem[j] + result->_mem[i+j] + carry;
                result->_mem[i+j] = current & _XLN_MASK;
                carry = current >> _XLN_SHIFT;
            }
            result->_mem[i+j] = (uint32_t)carry;
        }
    
        result->_current = left->_current*right->_current;
        return result;
    }
    else{
        error("ATANE\n", -4);
        return NULL;
    }
}

/*
 karatsube mltp
  in next update
 */
xln* xln_mul(xln* result, xln* left, xln* right){
    if(left != NULL && right != NULL){
        uint64_t max_size = (left->_current * right->_current);
        if(result == NULL){
            result = xln_alloc((uint32_t)max_size);
        }
        
        
        
        return result;
    }
    else{
        error("ATANE\n", -4);
        return NULL;
    }
}

/*
 slow div by int < base.
 */
xln* xln_sdiv_int(xln* result, xln* left, uint32_t right){
    if(left != NULL){
        if(result == NULL){
            static xln* res;
            res = xln_alloc(left->_current);
            result = res;
        }
        
        uint32_t carry = 0;
        uint32_t current;
        for(int32_t i = (int32_t)left->_current-1; i >= 0; --i){
            current = left->_mem[i] + (carry<<30);
            result->_mem[i] = current / right;
            carry = current % (right);
        }
        result->_current = left->_current - 1;
        
        xln_normalize(result);
        return result;
    }
    else{
        error("ATANE\n", -4);
        return NULL;
    }
}

/*
 Inefficient division algorithm. Suitable for small numbers.
 
 The idea is that if the digit in the high-order is greater than half the base of the system, then q = (l[j+n]*base + l[j+n-1])/r[n-1] is between [
 q_true, q_true + 2]. The division process itself is the division of l[j:j+n] by r, where j = l.size-r.size. We literally slide over the divisor.

 Rough estimate of algorithm's complexity is O(n^2) where n is number of words in divisible.
 */
xln* xln_sdiv(xln* result, xln *left, xln *right){
    if(left != NULL && right != NULL){
        /*
         Create the necessary variables. Allocate memory to back up the original numbers, because they will change in the process.
         */
        
        int64_t carry = 0;
        uint64_t current, d, q, r;
        int32_t i, j, m, n, k;
        uint32_t *tr, *bl, *br;
        
        tr = (uint32_t*)malloc(sizeof(uint32_t)*right->_current);
        bl = (uint32_t*)malloc(sizeof(uint32_t)*left->_current);
        br = (uint32_t*)malloc(sizeof(uint32_t)*right->_current);
        
        memcpy(bl, left->_mem, left->_current*sizeof(uint64_t));
        memcpy(br, right->_mem, right->_current*sizeof(uint64_t));
 
        m = left->_current - right->_current - 1;
        n = right->_current;
        d = (_XLN_BASE)/(1+right->_mem[n-1]);
        
        if(right->_mem[n-1] < (_XLN_BASE/2)){
                        
            carry = 0;
            for(j = 0; j < left->_current; ++j){
                current = left->_mem[j]*1ull*d + carry;
                left->_mem[j] = current & _XLN_MASK;
                carry = current >> _XLN_SHIFT;
            }
            left->_mem[j] = (uint32_t)carry;
            ++left->_current;
            
            carry = 0;
            for(j = 0; j < right->_current; ++j){
                current = right->_mem[j]*1ull*d + carry;
                right->_mem[j] = current & _XLN_MASK;
                carry = current >> _XLN_SHIFT;
            }

            m = left->_current - right->_current - 1;
            n = right->_current;
        }
        
        // "Slide" over the divisor and divide it
        for(j = m; j >= 0; --j){
            // Calculate the approximate values of the quotient and the remainder.
            q = (((uint64_t)left->_mem[j+n]<<_XLN_SHIFT) + left->_mem[j+n-1])/(right->_mem[n-1]);
            r = (((uint64_t)left->_mem[j+n]<<_XLN_SHIFT) + left->_mem[j+n-1])%(right->_mem[n-1]);
 
            // If the condition is satisfied, then q is greater than true. Reduce it by one.
            // And repeat the test if the remainder is less than the base of the system.
            if(q*right->_mem[n-2] > (r<<_XLN_SHIFT) + left->_mem[j+n-2]){
                --q;
                r += right->_mem[n-1];
                if((r < _XLN_BASE) && (q*right->_mem[n-2] > (r<<_XLN_SHIFT) + left->_mem[j+n-2])){
                    --q;
                }
            }

            // Create back up the divisor. Сalculate l - q*r
            memcpy(tr, right->_mem, right->_current*sizeof(uint32_t));

            // Calculate q*r
            //xln_smul_int(right, right, q);
            carry = 0;
            for(i = 0; i < right->_current; ++i){
                current = (right->_mem[i]*1ull*q) + carry;
                right->_mem[i] = current & _XLN_MASK;
                carry = current >> _XLN_SHIFT;
            }
        
            right->_mem[i] = (uint32_t)carry;
            right->_current = i;

            // Check for positive l-r. If any digit is negative, then q is too high. As a result we leave l + base^(n+1);
            // Keep the sign in the variable left->_sign
            for(i = n-1, k = j+n; i >= 0 && k >= j; --i, --k){
                //printf("k=%d, i=%d\n", k, i);
                if(left->_mem[k] - right->_mem[i] < 0){
                    left->_sign = -1;
                    left->_mem[n+1] += 1;
                    
                    memcpy(right->_mem, tr, right->_current*sizeof(uint32_t));
                    break;
                }
            }
            
            // If the sign is positive, then subtract the numbers.
            if(left->_sign == 1){

                int64_t cur;
                carry = 0;
                for(i = 0, k = j; i < n && k <= j+n; ++i, ++k){
                    cur = ((int64_t)left->_mem[k] - right->_mem[i] - carry);
                    left->_mem[k] = cur & _XLN_MASK;
                    carry = (cur >> _XLN_SHIFT) & 1;
                }
                
                for(;k<=j+n; ++k){
                    cur = ((int64_t)left->_mem[k] - carry);
                    left->_mem[k] = cur & _XLN_MASK;
                    carry = (cur >> _XLN_SHIFT) & 1;
                }
                
                
            }
            
            // return true value in right.
            memcpy(right->_mem, tr, right->_current*sizeof(uint32_t));
            
            // Assign the found value.
            result->_mem[j] = (uint32_t)q;
            
            // If the sign is negative, we must compensate for the addition. Decrease the found value by one.
            if(left->_sign == -1){
                error("warning sign = -1\n", 1);
                result->_mem[j] -= 1;
                for(i = 0, k = j; i < n && k <= j+n; ++i, ++k){
                    carry = left->_mem[k] + right->_mem[i] + carry;
                    left->_mem[k] = carry & _XLN_MASK;
                    carry >>= _XLN_SHIFT;
                }
                for(;k < j+n; ++i, ++k){
                    carry = left->_mem[k] + carry;
                    left->_mem[k] = carry & _XLN_MASK;
                    carry >>= _XLN_SHIFT;
                }
                left->_sign = 1;
            }
            
        }
        
        // Return the original values. And normalize the answer.
        left->_mem = bl;
        right->_mem = br;
        result->_current = m+n;
        
        free(tr);
        free(bl);
        free(br);
        
        return xln_normalize(result);
    }
    else{
        error("ATANE\n", -4);
        return NULL;
    }
}

/*
 An algorithm for finding the remainder modulo n based on the slow division algorithm for large numbers.
 Returns a pointer to the result or zero in case of error.
 
 Rough estimate of algorithm's complexity is O(n^2) where n is number of words in divisible.
 */
xln* xln_smod(xln* result, xln* left, xln* right){
    if(left != NULL && right != NULL){
        
        
        return result;
    }
    else{
        error("ATANE", -4);
        return NULL;
    }
}
//----------------------------------------------------------------------------------------


// COMPARISON
//----------------------------------------------------------------------------------------
 
lnbool xln_equal(xln* left, xln* right){
    if(left != NULL && right != NULL){
        if(left->_current < right->_current){
            return LN_FALSE;
        }
        else if(left->_current > right->_current){
            return LN_FALSE;
        }
        else {
            for(uint32_t i = left->_current-1; i >= 0; --i){
                if(*(left->_mem + i) != *(right->_mem + i)){
                    return LN_FALSE;
                }
            }
            return LN_TRUE;
        }
    }
    else{
        error("ATANE", -4);
        return -1;
    }
}

lnbool xln_noequal(xln* left, xln* right){
    if(left != NULL && right != NULL){
        if(xln_equal(left, right) == LN_TRUE){
            return LN_FALSE;
        }
        else{
            return LN_TRUE;
        }
    }
    else{
        error("ATANE", -4);
        return -1;
    }
}

lnbool xln_eqmore(xln* left, xln* right){
    if(left != NULL && right != NULL){
        if(xln_equal(left, right) == LN_TRUE || xln_more(left, right) == LN_TRUE){
            return LN_TRUE;
        }
        else{
            return LN_FALSE;
        }
    }
    else{
        error("ATANE", -4);
        return -1;
    }
}

lnbool xln_eqless(xln* left, xln* right){
    if(left != NULL && right != NULL){
        if(left != NULL && right != NULL){
            if(xln_equal(left, right) == LN_TRUE || xln_less(left, right) == LN_TRUE){
                return LN_TRUE;
            }
            else{
                return LN_FALSE;
            }
        }
        else{
            error("ATANE", -4);
            return -1;
        }
    }
    else{
        error("ATANE", -4);
        return -1;
    }
}

lnbool xln_more(xln* left, xln* right){
    if(left != NULL && right != NULL){
        if(left->_current > right->_current){
            return LN_TRUE;
        }
        else if(left->_current < right->_current){
            return LN_FALSE;
        }
        else {
            for(uint32_t i = left->_current-1; i >= 0; --i){
                if(*(left->_mem + i) < *(right->_mem + i)){
                    return LN_FALSE;
                }
            }
            return LN_TRUE;
        }
    }
    else{
        error("ATANE", -4);
        return -1;
    }
}

lnbool xln_less(xln* left, xln* right){
    if(left != NULL && right != NULL){
        if(left->_current < right->_current){
            return LN_TRUE;
        }
        else if(left->_current > right->_current){
            return LN_FALSE;
        }
        else {
            for(uint32_t i = left->_current-1; i >= 0; --i){
                if(*(left->_mem + i) > *(right->_mem + i)){
                    return LN_FALSE;
                }
            }
            return LN_TRUE;
        }
    }
    else{
        error("ATANE", -4);
        return -1;
    }
}

//----------------------------------------------------------------------------------------

// TEST
//----------------------------------------------------------------------------------------

void test_sum(void){
    xln* obj = xln_alloc(25);
    xln* obj2 = xln_alloc(25);
    xln* res = xln_alloc(25);
    xln_init_string(obj,  "12379400381324587724312379400");
    xln_init_string(obj2, "12379400381324587724312379400");
    xln_sum(res, obj, obj2);
    xln_convert_reverse(res);
    xln_printr(res);
    
    xln_free(obj);
    xln_free(obj2);
    xln_free(res);
}

void test(void){
    xln* obj = xln_alloc(15);
    xln_init_string(obj, "1237940038132458772439760897");
    //xln_convert_reverse(obj);
    xln_print(obj);
    
    xln_free(obj);
}

//int main(void){
//    test_sum();
//    return 0;
//}

//----------------------------------------------------------------------------------------

