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
	unsigned ii, jj;
	ii = 6;
	jj = 7;
	printf("\n Foo= %d\n", ii+jj);
}

TEST_F(BigIntTest, ToString) {
	printf("\nMade it!\n");
}

TEST_F(BigIntTest, AddOnStack) {
	printf("\nMade it!\n");
}

TEST_F(BigIntTest, AddOnHeap) {
	printf("\nMade it!\n");
}

/*=======================================*/
