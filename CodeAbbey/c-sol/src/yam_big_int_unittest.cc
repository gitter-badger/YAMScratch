#include <stdint.h>
#include <stdio.h>
#include <iomanip>


#include <limits.h>
#include "yam_big_int.h"
#include "gtest/gtest.h"


class BigIntTest : public testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST_F(BigIntTest, FromString) {
	/*This test depends on big_int_to_str working*/
	char* a, * b;
	const char* c;
	BigInt* A = newBigInt;
	size_t nbytes;
	nbytes = 0;
	/*create a string with all lower ascii characters*/
	a = (char*) malloc(128*sizeof(char));
	if(a != NULL) {
		b = a;
		unsigned ii;
		for(ii = 1; ii < 128; ++ii) {
			*b++ = ii;
		}
		*b = '\0';
		/*now create this string*/
		big_int_from_str(A, a);
		c = "0123456789";
		ASSERT_STREQ(big_int_to_str(A, &nbytes), c);
		free(a);
		BigInt_destroy(A);
	}
}

TEST_F(BigIntTest, ToString) {
	const char* a, * b;
	/*28 ones*/
	a = "1111111111111111111111111111";
	b = "1234567890123456789012345678";
	BigInt* A,* B;
	A = newBigInt;
	B = newBigInt;
	big_int_from_str(A, const_cast<char*>(a));
	big_int_from_str(B, const_cast<char*>(b));
	size_t nbytes;
	nbytes = 0;
	ASSERT_STREQ(a, big_int_to_str(A, &nbytes));
	EXPECT_EQ(28, nbytes);
	nbytes = 0;
	ASSERT_STREQ(b, big_int_to_str(B, &nbytes));
	EXPECT_EQ(28, nbytes);
	/**/
	BigInt_destroy(A);
	BigInt_destroy(B);
}

TEST_F(BigIntTest, AddOnStack) {
	printf("\nMade it!\n");
}

TEST_F(BigIntTest, AddOnHeap) {
	printf("\nMade it!\n");
}

/*=======================================*/
