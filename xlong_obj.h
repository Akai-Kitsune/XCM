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

// Version 0.5.5

// last version id = 00306be9e0e1ff0203436859a7893b1b220a5eef

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
#define LN_TRUE 0x1
// A type for false
#define LN_FALSE 0x0
// A type for bool
#define lnbool int8_t

// Flag to connect raw libraries in a standard way.
#define __XLN_INCLUDE_DEFOLT_LIB
// Flag to connect kVectors
#define __XLN_INCLUDE_VECTORS
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
#   define SDIVDEBUGOFF
#   define EGCDDEBUGoff
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

#ifdef __XLN_INCLUDE_VECTORS
#   include "include/Vectors.h"
#endif

#endif

/**
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

/**
 This is a tuple of data for storing various objects and passing them around. The "_mem"
 stores pointers to pointers to the object.
 */
typedef struct ln_tuple{
    void** _mem;
}ln_tuple;

// MEMORY
//----------------------------------------------------------------------------------------

/**
 This function allocates memory for the structure and "_size" cells to store it.
 returns a pointer to the allocated memory or NULL in case of an error
 */
xln* xln_alloc(uint32_t size);

/*
 This function reallocates memory for the structure and "_size" cells to store it.
 returns a pointer to the allocated memory or NULL in case of an error
 */
xln* xln_realloc(xln* obj, uint32_t new_size);

/**
 The function takes two pointers to numbers with multiple precision, the first pointer to where we copy, the second to what we copy.
 If the right pointer is NULL, the function will null the left pointer as well.
 */
void xln_copy(xln* left, xln* right);

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

/**
 This is a function that adds two large numbers element by element. If the pointer to the result is
 zero, it allocates memory for it. It returns a pointer to the sum of the two numbers or NULL in
 case of an error.
 
 left->_mem[i] + right->_mem[i] + carry <= (2^30 - 1) + (2^30 - 1) + 1 = 2^31 -1 < 2^31
 
 The addition algorithm is as follows:
 1. Assign j , carry <- 0 where j is the bit index and carry the overflow index.
 2. Add the digits modulo the system and calculate the overflow. [a[j] + b[j] + carry / base ]
 3. Increment j by one, if we have passed all digits, assign the last value of the remainder and complete the execution.
 O(log(a, 2^30))
 
 -The first argument is for the pointer to the result, the other two for the numbers, respectively.
 */
xln* xln_sum(xln* result, xln* left, xln* right);

/**
 This function is a safe wrapper for xln_sum, in case its operands can be negative.
 It takes three pointers: result, left and right operands, respectively. If the pointer to the result is zero, it returns NULL.
 */
xln* xln_ssum(xln* result, xln* left, xln* right);

/**
 This function subtracts one number from another with a sign. If no memory has been allocated for
 the resulting
 number, it is allocated and returns the result or NULL in case of an error.
 
 -The first argument is for the pointer to the result, the other two for the numbers, respectively.
 */
xln* xln_sub(xln* result, xln* left, xln* right);

/**
 This function is a safe wrapper for xln_sub, in case its operands can be negative.
 It takes three pointers: result, left and right operands, respectively. If the pointer to the result is zero, it returns NULL.
 */
xln* xln_ssub(xln* result, xln* left, xln* right);

/**
 #A function for multiplying a large number by a small number smaller than the base of the system.
 If no memory has
 been allocated for the result, allocate it. Returns the result or zero in case of an error.
 
 -The first argument is for the pointer to the result, the other two for the numbers, respectively.
 */
xln* xln_smul_int(xln* result, xln* left, uint32_t right);

/**
 Slow multiplication function of two large numbers.
 Automatically allocates memory for the result if the pointer is
 zero. Returns the result or zero in case of an error. The asymptotic of the algorithm is equal to
 the product of their lengths n and m O(n*m). Suitable for small numbers.
 
 -The first argument is for the pointer to the result, the other two for the numbers, respectively.
 */
xln* xln_smul(xln* result, xln* left, xln* right);

/**
 karatsube mltp
 
 -The first argument is for the pointer to the result, the other two for the numbers, respectively.
 */
xln* xln_mul(xln* result, xln* left, xln* right);

/**
 slow div by int < base.
 
 -The first argument is for the pointer to the result, the other two for the numbers, respectively.
 */
xln* xln_sdiv_int(xln* result, xln* left, uint32_t right);

/**
 slow mod by right < base
 
 */
uint32_t xln_smod_int(xln* left, uint32_t right);

/**
 Inefficient division algorithm. Suitable for small numbers.
 
 The idea is that if the digit in the high-order is greater than half the base of the system, then
 q = (l[j+n]*base + l[j+n-1])/r[n-1] is between [
 q_true, q_true + 2]. The division process itself is the division of l[j:j+n] by r, where j =
 l.size-r.size. We literally slide over the divisor.

 Rough estimate of algorithm's complexity is O(n^2) where n is number of words in divisible.
 
 -The first argument is for the pointer to the result, the other two for the numbers, respectively.
 */
xln* xln_sdiv(xln* result, xln* left, xln* right);

/**
 An algorithm for finding the remainder modulo n based on the slow division
 algorithm for large numbers. Returns a pointer to the result or zero in case of error.
 
 Rough estimate of algorithm's complexity is O(n^2) where n is number of words in divisible.
 
 -The first argument is for the pointer to the result, the other two for the numbers, respectively.
 */
xln* xln_smod(xln* result, xln* left, xln* right);

//----------------------------------------------------------------------------------------

// ALGORITHM
//----------------------------------------------------------------------------------------
/**
 Algorithm for finding the greatest common divisor of two numbers when the right-hand one is less than the base of the system.
 */
uint32_t xln_gcdInt(xln*left, uint32_t right);

/**
 Function for finding the greatest common divisor of two numbers when the right number is less than the base of the system and two numbers such that left*x + right*y = gcd(left, right)
 */
void** xln_egcd(xln*left, xln* right);

/**
 Algorithm for finding the greatest common divisor of two numbers, for two numbers of multiple precision.
 
 -The first argument is for the pointer to the result, the other two for the numbers, respectively.
 */
xln* xln_gcd(xln* gcd, xln*left, xln* right);

/**
 (left*result) mod right == 1
 */
xln* xln_inverseOf(xln* result, xln* left, xln* right);

//----------------------------------------------------------------------------------------

// COMPARISON
//----------------------------------------------------------------------------------------
/**
 The function compares two numbers, starting from the highest digit.
 Returns 1 if the numbers are equal otherwise zero.
 
 - The function takes two pointers and returns xln_true as the result of the comparison.
 */
lnbool xln_equal(xln* left, xln* right);

/**
 The function compares two numbers, starting from the highest digit.
 Returns 1 if the numbers are not equal otherwise zero.
 
 - The function takes two pointers and returns xln_true as the result of the comparison.
 */
lnbool xln_noequal(xln* left, xln* right);

/**
 The function compares two numbers starting with the highest digit.
 Returns 1 if the left number is greater than or equal to the right number, otherwise zero.
 
 - The function takes two pointers and returns xln_true as the result of the comparison.
 */
lnbool xln_eqmore(xln* left, xln* right);

/**
 The function compares two numbers starting with the highest digit.
 Returns 1 if the left number is less than or equal to the right number, otherwise zero.
 
 - The function takes two pointers and returns xln_true as the result of the comparison.
 */
lnbool xln_eqless(xln* left, xln* right);

/**
 The function compares two numbers starting with the highest digit.
 Returns 1 if the left number is greater than the right number otherwise zero.
 
 - The function takes two pointers and returns xln_true as the result of the comparison.
 */
lnbool xln_more(xln* left, xln* right);

/**
 The function compares two numbers starting with the highest digit.
 Returns 1 if the left number is less than the right number otherwise zero.
 
 - The function takes two pointers and returns xln_true as the result of the comparison. 
 */
lnbool xln_less(xln* left, xln* right);

//----------------------------------------------------------------------------------------


// TEST
//----------------------------------------------------------------------------------------

void test_large_number(void);

void test(void);

void test_egcd(const char *str1, const char *str2);

void test_div(const char *str1, const char *str2);

void test_inverse(const char *str1, const char *str2);

//----------------------------------------------------------------------------------------

#define xln_2_4096 "1044388881413152506691752710716624382579964249047383780384233483283953907971557456848826811934997558340890106714439262837987573438185793607263236087851365277945956976543709998340361590134383718314428070011855946226376318839397712745672334684344586617496807908705803704071284048740118609114467977783598029006686938976881787785946905630190260940599579453432823469303026696443059025015972399867714215541693835559885291486318237914434496734087811872639496475100189041349008417061675093668333850551032972088269550769983616369411933015213796825837188091833656751221318492846368125550225998300412344784862595674492194617023806505913245610825731835380087608622102834270197698202313169017678006675195485079921636419370285375124784014907159135459982790513399611551794271106831134090584272884279791554849782954323534517065223269061394905987693002122963395687782878948440616007412945674919823050571642377154816321380631045902916136926708342856440730447899971901781465763473223850267253059899795996090799469201774624817718449867455659250178329070473119433165550807568221846571746373296884912819520317457002440926616910874148385078411929804522981857338977648103126085903001302413467189726673216491511131602920781738033436090243804708340403154190336"

#define xln_2_16384 "1189731495357231765085759326628007130763444687096510237472674821233261358180483686904488595472612039915115437484839309258897667381308687426274524698341565006080871634366004897522143251619531446845952345709482135847036647464830984784714280967845614138476044338404886122905286855313236158695999885790106357018120815363320780964323712757164290613406875202417365323950267880089067517372270610835647545755780793431622213451903817859630690311343850657539360649645193283178291767658965405285113556134369793281725888015908414675289832538063419234888599898980623114025121674472051872439321323198402942705341366951274739014593816898288994445173400364617928377138074411345791848573595077170437644191743889644885377684738322240608239079061399475675334739784016491742621485229014847672335977897158397334226349734811441653077758250988926030894789604676153104257260141806823027588003441951455327701598071281589597169413965608439504983171255062282026626200048042149808200002060993433681237623857880627479727072877482838438705048034164633337013385405998040701908662387301605018188262573723766279240798931717708807901740265407930976419648877869604017517691938687988088008944251258826969688364194133945780157844364946052713655454906327187428531895100278695119323496808703630436193927592692344820812834297364478686862064169042458555136532055050508189891866846863799917647547291371573500701015197559097453040033031520683518216494195636696077748110598284901343611469214274121810495077979275556645164983850062051066517084647369464036640569339464837172183352956873912042640003611618789278195710052094562761306703551840330110645101995435167626688669627763820604342480357906415354212732946756073006907088870496125050068156659252761297664065498347492661798824062312210409274584565587264846417650160123175874034726261957289081466197651553830744424709698634753627770356227126145052549125229448040149114795681359875968512808575244271871455454084894986155020794806980939215658055319165641681105966454159951476908583129721503298816585142073061480888021769818338417129396878371459575846052583142928447249703698548125295775920936450022651427249949580708203966082847550921891152133321048011973883636577825533325988852156325439335021315312134081390451021255363707903495916963125924201167877190108935255914539488216897117943269373608639074472792751116715127106396425081353553137213552890539802602978645319795100976432939091924660228878912900654210118287298298707382159717184569540515403029173307292454391789568674219640761451173600617752186991913366837033887201582071625868247133104513315097274713442728340606642890406496636104443217752811227470029162858093727701049646499540220983981932786613204254226464243689610107429923197638681545837561773535568984536053627234424277105760924864023781629665526314910906960488073475217005121136311870439925762508666032566213750416695719919674223210606724721373471234021613540712188239909701971943944347480314217903886317767779921539892177334344368907550318800833546852344370327089284147501640589448482001254237386680074457341910933774891959681016516069106149905572425810895586938833067490204900368624166301968553005687040285095450484840073528643826570403767157286512380255109954518857013476588189300004138849715883139866071547574816476727635116435462804401112711392529180570794193422686818353212799068972247697191474268157912195973794192807298886952361100880264258801320928040011928153970801130741339550003299015924978259936974358726286143980520112454369271114083747919007803406596321353417004068869443405472140675963640997405009225803505672726465095506267339268892424364561897661906898424186770491035344080399248327097911712881140170384182058601614758284200750183500329358499691864066590539660709069537381601887679046657759654588001937117771344698326428792622894338016112445533539447087462049763409147542099248815521395929388007711172017894897793706604273480985161028815458787911160979113422433557549170905442026397275695283207305331845419990749347810524006194197200591652147867193696254337864981603833146354201700628817947177518115217674352016511172347727727075220056177748218928597158346744541337107358427757919660562583883823262178961691787226118865632764934288772405859754877759869235530653929937901193611669007472354746360764601872442031379944139824366828698790212922996174192728625891720057612509349100482545964152046477925114446500732164109099345259799455690095576788686397487061948854749024863607921857834205793797188834779656273479112388585706424836379072355410286787018527401653934219888361061949671961055068686961468019035629749424086587195041004404915266476272761070511568387063401264136517237211409916458796347624949215904533937210937520465798300175408017538862312719042361037129338896586028150046596078872444365564480545689033575955702988396719744528212984142578483954005084264327730840985420021409069485412320805268520094146798876110414583170390473982488899228091818213934288295679717369943152460447027290669964066816"

#endif /* __XLONG_OBJECT_H__ */


