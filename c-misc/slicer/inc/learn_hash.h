/*My implementation of khash and murmur to learn how hash tables works

khash.h found at <https://github.com/attractivechaos/klib/blob/master/khash.h>


*/
#ifndef LEARN_HASH_
#define LEARN_HASH_ 1

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>

uint32_t learn_murmur(const char *key, uint32_t len, uint32_t seed) {
	uint32_t c1 = 0xcc9e2d51;
	uint32_t c2 = 0x1b873593;
	uint32_t r1 = 15;
	uint32_t r2 = 13;
	uint32_t m = 5;
	uint32_t n = 0xe6546b64;

	uint32_t hash = seed;
	/*pad the key to get an even 4 byte chunk*/
	const int nblocks = len/4;
	/*alias the buffer for the length*/
	const uint32_t *block_ptr = (const uint32_t *) key;

	for(int i = 0; i < nblocks; ++i) {
		uint32_t partial = block_ptr[i];
		partial *= c1;
		partial = (partial << r1) | (partial >> (32 - r1));
		partial *= c2;
		/*XOR the hash with the partial value*/
		hash ^= partial;
		/*pretty much swap the bit order*/
		hash = ((hash << r2) | hash >> (32 - r2)) * m + n;
	}
	const uint8_t *tail = (const uint8_t *) (block_ptr + (nblocks * 4);
	uint32_t leftover = 0;
	/*Here we try to place the high bits of the remainder at the low bits, 
	so that these digits have the greatest change to affect the lowe range
	in the next multiplications*/
	switch (len & 3) {
		case 3:
			leftover ^= tail[2] << 16;
		case 2:
			leftover ^= tail[1] << 8;
		case 1:
			leftover ^= tail[0];
			/*remaining bytes*/
			leftover *= c1;
			leftover = (leftover << r1) | (leftover >> (32 - r1));
			leftover *= c2;
			hash ^= leftover;
	}
	hash ^= len;
	hash ^= (hash >> 16);
	hash *= 0x85ebca6b;
	hash ^= (hash >> 13);
	hash *= 0xc2b2ae35;
	hash ^= (hash >> 16);

	return hash;
}

#endif
