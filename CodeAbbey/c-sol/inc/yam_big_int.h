#ifndef __cplusplus
extern "C" {
#endif

#ifndef _YAM_BIG_INT_
#define _YAM_BIG_INT_ 
#include <strings.h>
#include <stdlib.h>

#include "parseCodeAbbeyInput.h"

/*we will use a vector as our big integer*/
VECTOR_INIT(char)

typedef Vector_t(char) BigInt;

#define newBigInt newVector(char)
#define BigInt_destroy(ptr) vector_destroy(char, ptr)
#define BigInt_clear(ptr) vector_clear(char, ptr)

/*Big Int is little Endian, stores least significant in lowest adress space*/

void big_int_from_str(BigInt* dest, const char* src);

char* big_int_to_str(BigInt* src, size_t* bytes_written);

int big_int_equality(BigInt* A, BigInt* B);

int big_int_less_than(BigInt* A, BigInt* B);

void big_int_add_heap(const BigInt* A, const BigInt* B, BigInt* result);

BigInt big_int_add_stack(const BigInt* A, const BigInt* B);

void compute_fibonacci(BigInt* fib_n, Vector_t(BigInt_ptr)* fib_memo);

unsigned find_fibonacci_index(BigInt* fib_n, Vector_t(BigInt_ptr)* fib_memo);

#endif

#ifndef __cplusplus
}
#endif
