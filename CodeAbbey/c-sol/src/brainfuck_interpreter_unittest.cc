#include <stdint.h>
#include <stdio.h>
#include <iomanip>
#include <limits.h>
#include <errno.h>

#include "brainfuck_interpreter.h"
#include "gtest/gtest.h"


class InterpreterTest : public testing::Test
{
protected:
	size_t ii;
	signed rc;
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST_F(InterpreterTest, Plus) {
	errno = 0;
	struct TapeNodeDebug* CellZero = (struct TapeNodeDebug*)calloc(1, sizeof(struct TapeNodeDebug));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	#define PLUS_BUFFER_LENGTH 5
	size_t buff_len;
	buff_len = PLUS_BUFFER_LENGTH;
	char in_buff[PLUS_BUFFER_LENGTH];
	for(ii = 0; ii < buff_len; ++ii) {
		in_buff[ii] = '+';
	}
	_eval_buffer_debug(in_buff, buff_len, CellZero, NULL, stdin, stdout);
	EXPECT_EQ(buff_len, CellZero->cell);
	EXPECT_EQ(NULL, CellZero->next);
	EXPECT_EQ(NULL, CellZero->prev);
	EXPECT_EQ(0, CellZero->index);
	free(CellZero);
}

TEST_F(InterpreterTest, Minus) {
	errno = 0;
	struct TapeNodeDebug* CellZero = (struct TapeNodeDebug*)calloc(1, sizeof(struct TapeNodeDebug));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	#define MINUS_BUFFER_LENGTH 5
	size_t buff_len;
	buff_len = MINUS_BUFFER_LENGTH;
	char in_buff[MINUS_BUFFER_LENGTH];
	for(ii = 0; ii < buff_len; ++ii) {
		in_buff[ii] = '-';
	}
	_eval_buffer_debug(in_buff, buff_len, CellZero, NULL, stdin, stdout);
	EXPECT_EQ((signed)buff_len * (-1), CellZero->cell);
	EXPECT_EQ(NULL, CellZero->next);
	EXPECT_EQ(NULL, CellZero->prev);
	EXPECT_EQ(0, CellZero->index);
	free(CellZero);
}

TEST_F(InterpreterTest, OverflowCell) {
	errno = 0;
	struct TapeNodeDebug* CellZero = (struct TapeNodeDebug*)calloc(1, sizeof(struct TapeNodeDebug));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	#define OVERFLOWCELL_BUFFER_LENGTH 1
	size_t buff_len;
	buff_len = OVERFLOWCELL_BUFFER_LENGTH;
	char in_buff[OVERFLOWCELL_BUFFER_LENGTH];
	for(ii = 0; ii < buff_len; ++ii) {
		in_buff[ii] = '+';
	}
	CellZero->cell = LONG_MAX;
	_eval_buffer_debug(in_buff, buff_len, CellZero, NULL, stdin, stdout);
	EXPECT_EQ(LONG_MIN, CellZero->cell);
	EXPECT_EQ(NULL, CellZero->next);
	EXPECT_EQ(NULL, CellZero->prev);
	EXPECT_EQ(0, CellZero->index);
	free(CellZero);
}

TEST_F(InterpreterTest, UnderflowCell) {
	errno = 0;
	struct TapeNodeDebug* CellZero = (struct TapeNodeDebug*)calloc(1, sizeof(struct TapeNodeDebug));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	#define UNDERFLOWCELL_BUFFER_LENGTH 1
	size_t buff_len;
	buff_len = UNDERFLOWCELL_BUFFER_LENGTH;
	char in_buff[UNDERFLOWCELL_BUFFER_LENGTH];
	for(ii = 0; ii < buff_len; ++ii) {
		in_buff[ii] = '-';
	}
	CellZero->cell = LONG_MIN;
	_eval_buffer_debug(in_buff, buff_len, CellZero, NULL, stdin, stdout);
	EXPECT_EQ(LONG_MAX, CellZero->cell);
	EXPECT_EQ(NULL, CellZero->next);
	EXPECT_EQ(NULL, CellZero->prev);
	EXPECT_EQ(0, CellZero->index);
	free(CellZero);
}

TEST_F(InterpreterTest, MoveLeftAlreadyAllocated) {
	printf("Move ptr left already allocated test\n");
}

TEST_F(InterpreterTest, MoveRightAlreadyAllocated) {
	printf("Move ptr right already allocated test\n");
}

TEST_F(InterpreterTest, MoveLeftNotAllocated) {
	printf("Move ptr left not allocated test \n");
}

TEST_F(InterpreterTest, MoveRightNotAllocated) {
	printf("Move ptr right not allocated test\n");
}

TEST_F(InterpreterTest, StackPush) {
	printf("Stack pushing test\n");
}

TEST_F(InterpreterTest, StackPop) {
	printf("Stack popping test\n");
}
/*test the input facilites from stdin and stdout using some other method*/
