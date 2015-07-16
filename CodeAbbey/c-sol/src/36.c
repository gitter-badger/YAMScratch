#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

#define STORAGE_TYPE long
/*#define BPI (8 * sizeof(STORAGE_TYPE)) */
#define BPI 64
#define NULL_CHECK(ptr, msg) 	\
	if(ptr == NULL) {			\
		perror(msg);			\
		exit(-1);				\
	}

#define GET_BIT(ptr, bit_index) \
	(((ptr)[((bit_index) / BPI)] & (((STORAGE_TYPE)1)<<((bit_index) % BPI))) ? 1: 0)

#define SET_BIT(ptr, bit_index) \
	ptr[(bit_index / BPI)] |= (((STORAGE_TYPE)1)<<(bit_index % BPI))

#define CLEAR_BIT(ptr, bit_index) \
	ptr[(bit_index / BPI)] &= ~(((STORAGE_TYPE)1)<<(bit_index % BPI))

int main(int argc, char const *argv[])
{
	/*4 digit mastermind guesser*/
	unsigned long N;
	int rc;
	unsigned ii, jj, kk, ll, C, R, d0, d1, d2, d3, secret, bin_index;
	errno = 0;
	rc = scanf(" %lu ", &N);
	if(rc != 1) {
		perror("failed to read input");
		exit(-1);
	}
	/*create a bit arrray of length*/
	unsigned long size_needed;
	size_needed = 10000/(8*sizeof(STORAGE_TYPE))+1;
	STORAGE_TYPE* result, * keep;
	result = (STORAGE_TYPE*)malloc(size_needed * sizeof(STORAGE_TYPE)); 
	NULL_CHECK(result, "failed to allocate result bit array");
	/*set all indices to true*/
	memset(result, 1, size_needed* sizeof(STORAGE_TYPE));
	printf("before %x\n", result[0] );
	STORAGE_TYPE foobar;
	for(ii = 0; ii < BPI; ++ii) {
		foobar = 1<<(ii%BPI);
		//printf("mask = %lx, %lx\n", foobar);
		printf("bit %u = %lu\n", ii, GET_BIT(result, ii));
	}

	for(ii = 64; ii < 64+BPI; ++ii) {
		printf("===================\n");
		printf("index %u bit %u\n", (ii)/ BPI , (ii) % BPI );

		printf("bit %u = %lu\n", ii, GET_BIT(result, ii));
		SET_BIT(result, ii);
		printf("bit %u = %lu\n", ii, GET_BIT(result, ii));
		CLEAR_BIT(result, ii);
		printf("bit %u = %lu\n", ii, GET_BIT(result, ii));
	}


	keep = (STORAGE_TYPE*)malloc(size_needed * sizeof(STORAGE_TYPE));
	NULL_CHECK(keep, "failed to allocate bit array");
	for(ii = 0; ii < N; ++ii) {
		errno = 0;
		rc = scanf(" %u %u", &C, &R);
		if(rc != 2) {
			fprintf(stderr, "input read error on line %u\n", ii +2);
			fflush(stderr);
			perror("");
			exit(-1);
		}
		/*sanity checks on input*/
		assert(C < 10000);
		assert(R < 5);
		/*if all four digits are correct we have found the secret code*/
		if(R == 4) {
			printf("%u\n", C);
			exit(0);
		}
		/*separate the digits 1234 -> d0 = 1, d1 = 2, ...*/
		d0 = C / 1000;
		C %= 1000;
		d1 = C /100;
		C %= 100;
		d2 = C / 10;
		d3 = C % 10;
		/*clear the keep vector*/
		memset(keep, 0, size_needed* sizeof(STORAGE_TYPE));
		if(R == 6) {
			/*operate directly on result*/
			/*clear all with same thoudsands place*/
			for(jj = d0*1000; jj < (d0+1)*1000; ++jj) {
				//printf("eliminating %u\n", jj);
				result[jj/BPI] &= ~(1<<(jj%BPI));
			}
			/*clear all with same hundreds place*/
			for(jj = 0; jj < 10; ++jj) {
				/*skip zone we have already cleared*/
				if(jj != d0) {
					bin_index = jj*1000;
					/*cross out everything with same hundreds place*/
					for(kk = bin_index+(d1*100); kk < bin_index+((d1+1)*100); ++kk) {
						//printf("eliminating %u\n", kk);
						result[kk/BPI] &= ~(1<<(kk%BPI));
					}
				}
			}
			/*clear all with same tens place*/
			for(jj = 0; jj < 10; ++jj) {
				/*skip zone we have already cleared*/
				if(jj != d0) {
					for(kk = 0; kk < 10; ++kk) {
						if(kk != d1) {
							bin_index = 1000*jj + 100*kk;
							for(ll = bin_index+(d2*10); ll < bin_index+((d2+1)*10); ++ll)
							//printf("eliminating %u\n", ll);
							result[ll/BPI] &= ~(1<<(ll%BPI));
						}
					}
				}
			}
			/*clear all with same ones place*/
			for(jj = 0; jj < 10; ++jj) {
				if(jj != d0) {
					for(kk = 0; kk < 10; ++kk) {
						if(kk != d1){
							for(ll = 0; ll < 10; ++ll) {
								if(ll != d2) {
									bin_index = 1000*jj + 100*kk + 10*ll + d3;
									printf("eliminating %u\n", bin_index);
									printf("before %lx, bit = %lu\n", result[bin_index/BPI], (result[bin_index/BPI]>>(bin_index%BPI) & 1) );
									result[bin_index/BPI] &= ~(1<<(bin_index%BPI));
									printf("after %lx, bit = %lu\n", result[bin_index/BPI], (result[bin_index/BPI]>>(bin_index%BPI) & 1));
								}
							}
						}
					}
				}
			}

		} else if(R == 1) {

		} else if (R == 2) {

		} else if (R == 3) {

		}
	}
	/*find the first non zero block, then find which bit is set*/
	STORAGE_TYPE* cursor;
	cursor = result;
	secret = 0;
	ii = 0;
	
	// while(*cursor++ == 0) {++ii;}
	// /*now result points to non zero block*/
	// kk = 0;
	// printf("value %lu\n", result[ii]);
	// for(jj = 0; jj < BPI; ++jj) {
	// 	if(result[ii] & 1<<jj) {
	// 		++kk;
	// 		secret = jj;
	// 		printf("ii %u, jj %u\n", ii, jj );
	// 		printf("valid %lu\n", jj+(ii*BPI));
	// 	}
	// }
	/*only one bit should be set*/

	free(result);
	free(keep);
	return 0;
}
