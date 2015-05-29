#ifdef __cplusplus
extern "C" {
#endif

#ifndef _YAM_FIBONACCI_
#define _YAM_FIBONACCI_

#include "yam_big_int.h"

typedef BigInt* BigInt_ptr;
/*we will use a vector as our memoization object*/
VECTOR_INIT(BigInt_ptr)

Vector_t(BigInt_ptr)* init_fib_memo();

void destroy_fib_memo(Vector_t(BigInt_ptr)* p);

void fill_in_up_to_fibonacci(BigInt* fib_n, Vector_t(BigInt_ptr)* fib_memo);

signed find_fibonacci_index(BigInt* fib_n, Vector_t(BigInt_ptr)* fib_memo);

#endif

#ifdef __cplusplus
}
#endif
