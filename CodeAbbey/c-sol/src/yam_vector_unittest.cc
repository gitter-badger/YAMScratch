#include <stdint.h>
#include <stdio.h>
#include <iomanip>
#include <limits.h>
#include <errno.h>
#include <cstring>

#include "yam_vector.h"
#include "gtest/gtest.h"

/*dummy types for testing copy semantics*/

/*initialize all of the vector types*/

VECTOR_INIT(uint8_t)
VECTOR_INIT(uint16_t)
VECTOR_INIT(uint32_t)
VECTOR_INIT(uint64_t)


TEST(VectorTest, NewVector_uint8) {
	Vector(uint8_t)* v;
	v = newVector(uint8_t);
	EXPECT_TRUE(NULL != v);
}
TEST(VectorTest, NewVector_uint16) {
	Vector(uint16_t)* v;
	v = newVector(uint16_t);
	EXPECT_TRUE(NULL != v);
}

TEST(VectorTest, NewVector_uint32) {
	Vector(uint32_t)* v;
	v = newVector(uint32_t);
	EXPECT_TRUE(NULL != v);
}
TEST(VectorTest, NewVector_uint64) {
	Vector(uint64_t)* v;
	v = newVector(uint64_t);
	EXPECT_TRUE(NULL != v);
}

TEST(VectorTest, PushBack_uint8) {
	Vector(uint8_t)* v;
	v = newVector(uint8_t);
	//ASSERT_TRUE(NULL != v);
	//EXPECT_EQ(0, v->elms);

	uint8_t* a = new uint8_t;
	vector_push_back(uint8_t, v, *a);
	//EXPECT_EQ(1, v->items);
	//EXPECT_EQ(*a, v->items[0]);
}
