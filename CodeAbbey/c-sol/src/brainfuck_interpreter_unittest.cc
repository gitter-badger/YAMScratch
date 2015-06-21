#include <stdint.h>
#include <stdio.h>
#include <iomanip>

#include <stdint.h>
#include <stdio.h>
#include <iomanip>


#include <limits.h>
#include "brainfuck_interpreter.h"
#include "gtest/gtest.h"


class InterpreterTest : public testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

TEST_F(InterpreterTest, Plus) {
	printf("Plus test\n");
}

TEST_F(InterpreterTest, Minus) {
	printf("Minus Test\n");
}

TEST_F(InterpreterTest, OverflowCell) {
	printf("Overflow test\n");
}

TEST_F(InterpreterTest, UnderflowCell) {
	printf("Underflow test\n");
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
