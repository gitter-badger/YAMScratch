#include <stdint.h>
#include <stdio.h>
#include <iomanip>


#include <limits.h>
#include "yam_big_int.h"
#include "gtest/gtest.h"


class BigIntTest : public testing::Test
{
protected:

	virtual void SetUp() {
	}

	virtual void TearDown() {
	}

};

TEST_F(BigIntTest, FromString) {
	char* a, * b, * c, * d;
	/*28 ones*/
	a = "1111111111111111111111111111"
	b = "2222222222222222222222222222"
	
	/*expected behavior is only to extract digits, ignoring all other characters*/

}

TEST_F(BigIntTest, ToString) {
	char* a, * b, * c, * d;
	/*28 ones*/
	a = "1111111111111111111111111111"
	b = "1234567890123456789012345678"
	BigInt A, B;
	big_int_from_str(&A, a);
	big_int_from_str(&B, b);
	size_t nbytes;
	nbytes = 0;
	ASSERT_STREQ(a, big_int_to_str(&A, &nbytes));
	EXPECT_EQ(28, nbytes);
	nbytes = 0;
	ASSERT_STREQ(b, big_int_to_str(&A, &nbytes));
	EXPECT_EQ(28, nbytes);
}

TEST_F(BigIntTest, AddOnStack) {
	printf("\nMade it!\n");
}

TEST_F(BigIntTest, AddOnHeap) {
	printf("\nMade it!\n");
}

/*=======================================*/
