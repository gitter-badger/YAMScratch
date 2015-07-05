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
