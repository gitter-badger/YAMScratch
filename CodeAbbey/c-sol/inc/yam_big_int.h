#ifdef __cplusplus
extern "C" {
#endif

#ifndef _YAM_BIG_INT_
#define _YAM_BIG_INT_ 

#include "yam_vector.h"

/*we will use a vector as our big integer*/
VECTOR_INIT(char)
typedef Vector(char) BigInt;

#define newBigInt newVector(char)
#define BigInt_destroy(ptr) vector_destroy(char, ptr)
#define BigInt_clear(ptr) vector_clear(char, ptr)
#define BigInt_resize(ptr,n) vector_resize(char, ptr, n)


/*Big Int is little Endian, stores least significant in lowest adress space*/

void big_int_from_str(BigInt* dest, char* src);

char* big_int_to_str(BigInt* src, size_t* bytes_written);

int big_int_equality(BigInt* A, BigInt* B);

int big_int_less_than(BigInt* A, BigInt* B);

void big_int_add_heap(BigInt* A, BigInt* B, BigInt* result);

BigInt big_int_add_stack(BigInt* A, BigInt* B);

#endif

#ifdef __cplusplus
}
#endif
