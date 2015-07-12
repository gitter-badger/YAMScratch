#include "code_abbey_utility.h"

signed array_checksum(long const* array, size_t size, long* checksum) {
	*checksum = 0;
	size_t ii;
	for(ii = 0; ii < size; ++ii) {
		*checksum += array[ii];
		*checksum *= 113;
		*checksum %= 10000007;
	}
	return 0;
}

uint64_t modular_exponentiation(uint64_t A, uint64_t B, uint32_t M){
	uint64_t result, accum, mask;
	/*sanity checks*/
	if(B == 0) {
		return 1;
	}
	if(M == 0){
		return 0;
	}
	/*base case (A^1)%M*/
	accum = A % M;
	mask = 1;
	if((mask & B)) {
		result = accum;
	} else {
		result = 1;
	}
	uint32_t ii;
	for(ii = 1; ii < 64; ++ii) {
		accum *= accum;
		accum %= M;
		/*now accum is (A^ii)%M, see if it contributes*/
		if(((mask << ii) & B)) {
			result *= accum;
			result %= M;
		}
	}
	return result;
}

unsigned long linear_congruent_gen(unsigned long A, unsigned long C,
								unsigned long M, unsigned long Xcurr) {
	/*make the chance of overflow smaller*/
	A %= M;
	Xcurr %= M;
	C %= M;
	return ((A * Xcurr) % M + C ) % M;
}
