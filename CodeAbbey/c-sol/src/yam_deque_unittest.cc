#include <stdint.h>
#include <stdio.h>
#include <iomanip>
#include <limits.h>
#include <errno.h>
#include <cstring>

#include "yam_deque.h"
#include "gtest/gtest.h"

/*dummy types for testing copy semantics*/
typedef struct dummy320 {
	uint64_t data[40];
} dummy320;

typedef struct dummy512 {
	uint64_t data[64];
} dummy512;

typedef struct dummy1024 {
	uint64_t data[128];
} dummy1024;

/*initialize all of the deque types*/
DEQUE_INIT(uint8_t)
DEQUE_INIT(uint16_t)
DEQUE_INIT(uint32_t)
DEQUE_INIT(uint64_t)
DEQUE_INIT(dummy320)
DEQUE_INIT(dummy512)
DEQUE_INIT(dummy1024)

TEST(DequeTest, PageSize) {
	EXPECT_EQ(512, PAGE_SIZE(uint8_t));
	EXPECT_EQ(256, PAGE_SIZE(uint16_t));
	EXPECT_EQ(128, PAGE_SIZE(uint32_t));
	EXPECT_EQ(64, PAGE_SIZE(uint64_t));
	EXPECT_EQ(2, PAGE_SIZE(dummy320));
	EXPECT_EQ(1, PAGE_SIZE(dummy512));
	EXPECT_EQ(1, PAGE_SIZE(dummy1024));
}

TEST(DequeTest, NewDeque_uint8) {
	/*Deque(uint8_t)* v;
	v = newDeque(uint8_t);
	EXPECT_TRUE(NULL != v);
	*/
}
/*
TEST(DequeTest, NewDeque_uint16) {
	Deque(uint16_t)* v;
	v = newDeque(uint16_t);
	EXPECT_TRUE(NULL != v);
}

TEST(DequeTest, NewDeque_uint32) {
	Deque(uint32_t)* v;
	v = newDeque(uint32_t);
	EXPECT_TRUE(NULL != v);
}
TEST(DequeTest, NewDeque_uint64) {
	Deque(uint64_t)* v;
	v = newDeque(uint64_t);
	EXPECT_TRUE(NULL != v);
}

TEST(DequeTest, PushBack) {
	Deque(uint8_t)* v;
	v = newDeque(TypeParam);
	ASSERT_NE(NULL, v);
	EXPECT_EQ(0, v->elms);

	TypeParam* a = new TypeParam;
	vector_push_back(TypeParam, v, *a);
	EXPECT_EQ(1, v->items);
	EXPECT_EQ(*a, v->items[0]);
}
*/
