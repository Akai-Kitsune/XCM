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

// Version 0.5.2

// last version id = fe40b391313d71fac94889fc86e249ad7f07bf86

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
        
        // fill the number with zeros.
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
        obj->_mem = realloc(obj->_mem, new_size*sizeof(uint32_t));
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
    if(obj != NULL && obj->_mem != NULL){
        free(obj->_mem);
        free(obj);
    }
    else {
        error("EATANE", -4);
    }
}

/*
 The function takes two pointers to numbers with multiple precision, the first pointer to where we copy, the second to what we copy.
 If the right pointer is NULL, the function will null the left pointer as well.
 */
void xln_copy(xln* left, xln* right){
    if(left != NULL && right != NULL){
        left->_current = right->_current; left->_sign = right->_sign;
        left->_sign = right->_sign;
        if(left->_size < right->_size){
            left = xln_realloc(left, right->_size);
        }
        
        memmove(left->_mem, right->_mem, right->_size*sizeof(uint32_t));
    }
    else{
        left = NULL;
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
        int32_t i;
#       define pre_base 1000000000
        obj->_current = 1;
        for(i = 0; i < n; ++i){
            carry = 0;
            for(int32_t j = n-1; j >= 0; --j){
                current = a[j] + (carry*pre_base);
                a[j] = current >> _XLN_SHIFT;
                carry = (current % (_XLN_BASE));
            }
            obj->_mem[i] = (uint32_t)carry;
            obj->_current += 1;
        }
        obj->_current += 1;
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
        // n = obj->_current + 1
        uint64_t n = obj->_current;
        uint64_t a[n];
        int32_t i;
        //memcpy(a, obj->_mem, sizeof(obj->_mem));
        for(int i = 0; i < n; ++i){
            a[i] = obj->_mem[i];
            //printf("a= %llu\n", i);
        }
#       define pre_base 1000000000
        obj->_current = 1;
        // i <= n
        for(i = 0; i < n; ++i){
            carry = 0;
            for(int64_t j = n-1; j >= 0; --j){
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
        uint64_t a[size_str];
        char tmp[(size_str)];
        
        memmove(tmp, str, strlen(str)*sizeof(char));
        for(int64_t i = (int64_t)(size_str); i > 0; i -= 9, ++j){
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
 This function is a safe wrapper for xln_sum, in case its operands can be negative.
 It takes three pointers: result, left and right operands, respectively. If the pointer to the result is zero, it returns NULL.
 */
xln* xln_ssum(xln* result, xln* left, xln* right){
    if(result != NULL && left != NULL && right != NULL){
        if(left->_sign == -1 && right->_sign == -1){
            result->_sign = -1;
            return xln_sum(result, left, right);
        }
        else if(left->_sign == -1 && right->_sign == 1){
            return xln_sub(result, right, left);
        }
        else if(left->_sign == 1 && right->_sign == -1){
            return xln_sub(result, left, right);
        }
        else{
            return xln_sum(result, left, right);
        }
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
    
    if(result != NULL && left != NULL && right != NULL){
        
        int32_t i = 0;
        int64_t carry = 0;
        
        if(left->_current == right->_current){
            
            i = left->_current;
            
            while((--i >= 0) && (left->_mem[i] == right->_mem[i])){}
            
            if(i < 0){
                result->_mem[0] = 0;
                result->_current = 1;
                result->_sign = 1;
                return result;
            }
            
            if(left->_mem[i] < right->_mem[i]){
                result->_sign = -1;
                return xln_sub(result, right, left);
            }
            
            left->_current = right->_current = i+1;
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
        
        return xln_normalize(result);
    }
    else{
        error("ATANE", -4);
        return NULL;
    }
}

/*
 This function is a safe wrapper for xln_sum, in case its operands can be negative.
 It takes three pointers: result, left and right operands, respectively. If the pointer to the result is zero, it returns NULL.
 */
xln* xln_ssub(xln* result, xln* left, xln* right){
    if(result != NULL && left != NULL && right != NULL){
        
        if(left->_sign == -1){
            // -a -(-b) = b - a
            if(right->_sign == -1){
                result->_sign = -1;
                return xln_sub(result, right, left);
            } // -a - b
            else{
                result->_sign = -1;
                return xln_sum(result, left, right);
            }
        }
        else {
            // a - (-b) = a + b
            if(right->_sign == -1){
                result->_sign = 1;
                return xln_sum(result, left, right);
            }
            else{ // a - b
                return xln_sub(result, left, right);
            }
        }
        
        return NULL;
    }
    else {
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
        
        uint32_t max_size = (left->_current + right->_current + 0x1);
        
        if(result == NULL){
            result = xln_alloc((uint32_t)max_size);
        }
        
        if(result->_size < max_size){
            xln_realloc(result, max_size);
        }
        
        
        
        for(uint32_t i = 0; i <= result->_size; ++i){
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
    
        result->_sign = left->_sign * right->_sign;
        result->_current = max_size;
        return xln_normalize(result);
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
        
        if(right == 0){
            return result;
        }
        
        if(result == NULL){
            static xln* res;
            res = xln_alloc(left->_current);
            result = res;
        }
        
        uint32_t carry = 0;
        uint64_t current;
        for(int32_t i = (int32_t)left->_current-1; i >= 0; --i){
            current = left->_mem[i] + (carry*1ll<<_XLN_SHIFT);
            result->_mem[i] = (uint32_t)(current / right);
            carry = current % right;
        }
        result->_current = left->_current;
        
        xln_normalize(result);
        return result;
    }
    else{
        error("ATANE\n", -4);
        return NULL;
    }
}

/*
 slow mod by right < base
 */
uint32_t xln_smod_int(xln* left, uint32_t right){
    if(left != NULL){
 
        if(right == 0){
            return 0;
        }
        
        uint32_t carry = 0;
        uint64_t current;
        for(int32_t i = (int32_t)left->_current-1; i >= 0; --i){
            current = left->_mem[i] + (carry*1ll <<_XLN_SHIFT);
            carry = current % (right);
        }

        return carry;
    }
    else{
        error("ATANE\n", -4);
        return 0;
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
        
        if(right->_current == 1){
            if(right->_mem[0] == 0){
                error("div by zero", -7);
                return NULL;
            }
            else{
                return xln_sdiv_int(result, left, right->_mem[0]);
            }
        }
            
        /*
         Create the necessary variables. Allocate memory to back up the original numbers, because they will change in the process.
         */
        
        int64_t carry = 0;
        uint64_t current, d, q, r;
        int32_t i, j, m, n, k;
        
        xln* tr = xln_alloc(right->_size),
        *bl = xln_alloc(left->_size),
        *br = xln_alloc(right->_size);
        
        xln_copy(tr, right); xln_copy(bl, left); xln_copy(br, right);
 
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
            //memcpy(tr, right->_mem, right->_current*sizeof(uint32_t));
            xln_copy(tr, right);

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
                    
                    //memcpy(right->_mem, tr, right->_current*sizeof(uint32_t));
                    xln_copy(right, tr);
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
            //memcpy(right->_mem, tr, right->_current*sizeof(uint32_t));
            xln_copy(right, tr);
            
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
        xln_copy(left, bl);
        xln_copy(right, br);

        // Free tmp memory
        xln_free(tr); xln_free(bl); xln_free(br);
        
        result->_current = n+m;
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
        
        if(right->_current == 1){
            result->_current = 1;
            *(result->_mem) = xln_smod_int(left, *(right->_mem));
            return result;
        }
        
        /*
         Create the necessary variables. Allocate memory to back up the original numbers, because they will change in the process.
         */
        
        int64_t carry = 0;
        uint64_t current, d, q, r;
        int32_t i, j, m, n, k;
        //uint32_t *tr, *bl, *br;
        xln* tr = xln_alloc(right->_size), *bl = xln_alloc(left->_size), *br = xln_alloc(right->_size);
        
//        tr = (uint32_t*)malloc(sizeof(uint32_t)*right->_current);
//        bl = (uint32_t*)malloc(sizeof(uint32_t)*left->_current);
//        br = (uint32_t*)malloc(sizeof(uint32_t)*right->_current);
//        memcpy(bl, left->_mem, left->_size*sizeof(uint32_t));
//        memcpy(br, right->_mem, right->_size*sizeof(uint32_t));
 
        xln_copy(tr, right); xln_copy(bl, left); xln_copy(br, right);
        
        m = left->_current - right->_current - 1;
        n = right->_current;
        d = (_XLN_BASE)/(1+right->_mem[n-1]);
    
        if(right->_mem[n-1] < (_XLN_BASE/2)){
                        
            carry = 0;
            for(j = 0; j < left->_current; ++j){
                current = (left->_mem[j]*1ull*d) + carry;
                left->_mem[j] = current & _XLN_MASK;
                carry = current >> _XLN_SHIFT;
            }
            left->_mem[j] = (uint32_t)carry;
            ++left->_current;
            
            carry = 0;
            for(j = 0; j < right->_current; ++j){
                current = (right->_mem[j]*1ull*d) + carry;
                right->_mem[j] = current & _XLN_MASK;
                carry = current >> _XLN_SHIFT;
            }
            if(carry != 0){
                right->_mem[j] = (uint32_t)carry;
                right->_current += 1;
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
            //memcpy(tr, right->_mem, right->_current*sizeof(uint32_t));
            xln_copy(tr, right);

            // Calculate q*r
            //xln_smul_int(right, right, q);
            carry = 0;
            for(i = 0; i < n; ++i){
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
                    left->_mem[n+1] = 1;
                    
                    //memcpy(right->_mem, tr, right->_current*sizeof(uint32_t));
                    xln_copy(right, tr);
                    break;
                }
            }
            
            // If the sign is positive, then subtract the numbers.
            if(left->_sign == 1){

                uint32_t cur;
                carry = 0;
                for(i = 0, k = j; i <= n && k <= j+n; ++i, ++k){
                    cur = ((uint32_t)left->_mem[k] - (uint32_t)right->_mem[i] - (uint32_t)carry);
                    left->_mem[k] = cur & _XLN_MASK;
                    carry = (cur >> _XLN_SHIFT) & 1;
                }
                
                for(;k<=j+n; ++k){
                    cur = ((uint32_t)left->_mem[k] - (uint32_t)carry);
                    left->_mem[k] = cur & _XLN_MASK;
                    carry = (cur >> _XLN_SHIFT) & 1;
                }
                
                
            }
            
            // return true value in right.
            //memcpy(right->_mem, tr, right->_size*sizeof(uint32_t));
            xln_copy(right, tr);
            
            // Assign the found value.
            result->_mem[j] = (uint32_t)q;
            
            // If the sign is negative, we must compensate for the addition. Decrease the found value by one.
            
            if(left->_sign == -1){
                //xln_sum(left, left, right);
                error("warning sign = -1\n", 1);
                result->_mem[j] -= 1;
                carry = 0;
                for(i = 0, k = j; i < n && k <= j+n; ++i, ++k){
                    carry = left->_mem[k] + right->_mem[i] + carry;
                    left->_mem[k] = carry & _XLN_MASK;
                    carry = (uint64_t)carry / (1<<30);
                }
                for(;k <= j+n; ++i, ++k){
                    carry = left->_mem[k] + carry;
                    left->_mem[k] = carry & _XLN_MASK;
                    carry = (uint64_t)carry / (1<<30);
                }
                left->_sign = 1;
            }
            
        }
        
        //carry
        xln_normalize(left);

        xln_sdiv_int(result, left, (uint32_t)d);

//      Return the original values. And normalize the answer.
//      memmove(left->_mem, bl, left->_size*sizeof(uint32_t));
//      memmove(right->_mem, br, right->_size*sizeof(uint32_t));
        xln_copy(left, bl);
        xln_copy(right, br);

        // Free tmp memory
        xln_free(tr); xln_free(bl); xln_free(br);
        
        return xln_normalize(result);
    }
    else{
        error("ATANE", -4);
        return NULL;
    }
}

/*
 Extended_enclidean_algoritm for right < base
 */
uint32_t xln_gcdInt(xln*left, uint32_t right){
    if(left != NULL){
        
        if(right == 0){
            return 0;
        }
        
        uint32_t t = xln_smod_int(left, right), c;
        
        while(t != 0){
            c = t;
            t = right % t;
            right = c;
        }
        
        return right;
    }
    else {
        error("ATANE", -4);
        return 0;
    }
}

/*
 Function for finding the greatest common divisor of two numbers when the right number is less than the base of the system and two numbers such that left*x + right*y = gcd(left, right)
 */
void** xln_egcd(xln*left, xln* right){
    if(left != NULL){
        //Next
        void **ptr = (void**)malloc(3*sizeof(void));
        
        xln *u1 = xln_alloc(left->_size), *u2 = xln_alloc(left->_size), *u3 = xln_alloc(left->_size);
        xln *v1 = xln_alloc(left->_size), *v2 = xln_alloc(left->_size), *v3 = xln_alloc(right->_size);
        xln *t1 = xln_alloc(left->_size), *t2 = xln_alloc(left->_size), *t3 = xln_alloc(right->_size);
        xln *q = xln_alloc(left->_size), *n = xln_alloc(left->_size);
        
        xln_init_string(u1, "1"); xln_init_string(u2, "0");
        xln_init_string(v1, "0"); xln_init_string(v2, "1");
        
        xln_copy(u3, left); xln_copy(v3, right);
        
        while(v3->_current >= 1 && v3->_mem[0] != 0){
            
           // xln_print(q);
#ifdef EGCDDEBUG
            xln_print(u1);
            xln_print(u2);
            xln_print(u3);

            xln_print(v1);
            xln_print(v2);
            xln_print(v3);
#endif
            printf("END ITERATION\n");
            
            xln_sdiv(q, u3, v3);

            xln_smul(n, v1, q);
            xln_ssub(t1, u1, n);

            xln_smul(n, v2, q);
            xln_ssub(t2, u2, n);

            xln_smul(n, v3, q);
            xln_ssub(t3, u3, n);

            xln_copy(u1, v1);
            xln_copy(u2, v2);
            xln_copy(u3, v3);
            
            xln_copy(v1, t1);
            xln_copy(v2, t2);
            xln_copy(v3, t3);

        }
        
        ptr[0] = u1;
        ptr[1] = u2;
        ptr[2] = u3;
        
        xln_free(v1); xln_free(v2); xln_free(v3);
        xln_free(t1); xln_free(t2); xln_free(t3);
        xln_free(q); xln_free(n);
        
        
        
        return ptr;
    }
    else {
        error("ATANE", -4);
        return NULL;
    }
}

/*
 Extended_enclidean_algoritm
 */
xln* xln_gcd(xln* res, xln*left, xln* right){
    if(left != NULL && right != NULL && res != NULL){
        
        // If right == 0, return left.
        if(right->_current == 1 && right->_mem[0] == 0){
            xln_copy(res, left);
            return res;
        } // If right == 1, return 1.
        else if(right->_current == 1 && right->_mem[0] == 0){
            res->_current = 1;
            res->_mem[0] = 1;
            return res;
        }

        // Create copies of the numbers to work with them without change.
        xln *t = xln_alloc(left->_size), *r = xln_alloc(right->_size);
        xln_copy(r, right);
        
        //Zero iteration to get rid of the case where left = k*right
        xln_smod(t, left, r);
        
        //The basic loop, is a modification of the gcd algorithm for numbers of multiple precision.
        while(t->_current >= 1 && t->_mem[0] != 0){
            xln_copy(res, t);
            xln_smod(t, r, res);
            xln_copy(r, res);
        }

        xln_copy(res, r);
        
        xln_free(t); xln_free(r);
        
        return res;
    
    }
    else {
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
        else if(left->_sign == right->_sign){
            for(int32_t i = left->_current-1; i >= 0; --i){
                if(*(left->_mem + i) != *(right->_mem + i)){
                    return LN_FALSE;
                }
            }
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
        if((left->_current > right->_current) || (left->_sign > right->_sign)){
            return LN_TRUE;
        }
        else if(left->_current < right->_current || (left->_sign < right->_sign)){
            return LN_FALSE;
        }
        else {
            for(int32_t i = left->_current-1; i >= 0; --i){
                if(*(left->_mem + i) <= *(right->_mem + i)){
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
        if(left->_current < right->_current || (left->_sign < right->_sign)){
            return LN_TRUE;
        }
        else if(left->_current > right->_current || (left->_sign > right->_sign)){
            return LN_FALSE;
        }
        else {
            for(int32_t i = left->_current-1; i >= 0; --i){
                if(*(left->_mem + i) >= *(right->_mem + i)){
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

void test_gcdInt(void){
    xln* obj = xln_alloc(10);
    xln_init_string(obj,  "1329227995784915872903807060280344576");

    printf("gcd = %u\n", xln_gcdInt(obj, 12));
    
    xln_free(obj);
}

void test_gcd(void){
    xln* obj = xln_alloc(30);
    xln* obj2 = xln_alloc(30);
    xln* res = xln_alloc(61);
    xln_init_string(obj,  "9085729004256841718308438420015608007687001344443112760818356862794610647930864113878808725615792656835384509495872239647213856410356729267494460946312905410849");



    xln_init_string(obj2, "1108899372780783641306111715875094966436017167649879524402769841278887580501366697712424694256005093589248451503068397608001");
    xln_gcd(res, obj, obj2);
    xln_convert_reverse(res);
    xln_printr(res);
    
    xln_free(obj);
    xln_free(obj2);
    xln_free(res);
}

void test_large_number(void){
    xln* obj = xln_alloc(600);
    xln* obj2 = xln_alloc(600);
    xln* res = xln_alloc(1200);
    xln_init_string(obj,  xln_2_16384);
    printf("%d\n", obj->_current);
    xln_init_string(obj2, xln_2_16384);
    xln_smul(res, obj, obj2);
    printf("%u\n", xln_smod_int(res, 1073676287));
    //xln_convert_reverse(obj);
    //xln_printr(obj);
    
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

void test_egcd(void){
    xln* obj = xln_alloc(6);
    xln* obj2 = xln_alloc(4);
    
    xln_init_string(obj,  "58247982750105740");

    xln_init_string(obj2, "0");
    
    void **ptr = xln_egcd(obj, obj2);

    xln_convert_reverse(ptr[0]);
    xln_convert_reverse(ptr[1]);
    xln_convert_reverse(ptr[2]);

    xln_printr(ptr[0]);
    xln_printr(ptr[1]);
    xln_printr(ptr[2]);
    
    xln_free(obj);
    xln_free(obj2);
    
    xln_free(ptr[0]);
    xln_free(ptr[1]);
    xln_free(ptr[2]);
    free(ptr);

}


//----------------------------------------------------------------------------------------


