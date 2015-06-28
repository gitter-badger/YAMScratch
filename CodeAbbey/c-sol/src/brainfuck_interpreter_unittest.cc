#include <stdint.h>
#include <stdio.h>
#include <iomanip>
#include <limits.h>
#include <errno.h>
#include <cstring>

#include "brainfuck_interpreter.h"
#include "gtest/gtest.h"

/*use overloading in C++ to allow inferring of type and call appropriate C function*/
signed buffer_eval(char* src, size_t nbytes, struct TapeNode* cursor, 
				   Vector_t(long)* stack, FILE* in_st, FILE* out_st) {
	return _eval_buffer(src, nbytes, cursor, stack, in_st, out_st);
}

signed buffer_eval(char* src, size_t nbytes, struct TapeNodeDebug* cursor, 
				   Vector_t(long)* stack, FILE* in_st, FILE* out_st) {
	return _eval_buffer_debug(src, nbytes, cursor, stack, in_st, out_st);
}

signed tape_destroy(struct TapeNode* cursor) {
	return _destroy_tape(cursor);
}

signed tape_destroy(struct TapeNodeDebug* cursor) {
	return _destroy_tape_debug(cursor);
}


template <typename T>
class InterpreterTest : public testing::Test
{
protected:
	signed (*buffer_eval)(char* , size_t, T*, Vector_t(long)*, FILE*, FILE*);
	virtual void SetUp() {}
	virtual void TearDown() {}
};

typedef ::testing::Types<struct TapeNode, struct TapeNodeDebug> MyTypes;
TYPED_TEST_CASE(InterpreterTest, MyTypes);

TYPED_TEST(InterpreterTest, Plus) {
	size_t ii;
	signed rc;
	size_t buff_len;
	errno = 0;
	TypeParam* CellZero;
	CellZero = (TypeParam*)calloc(1, sizeof(TypeParam));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	buff_len = 5;
	char in_buff[5];
	for(ii = 0; ii < buff_len; ++ii) {
		in_buff[ii] = '+';
	}
	rc = buffer_eval(in_buff, buff_len, CellZero, NULL, stdin, stdout);
	EXPECT_EQ(buff_len, CellZero->cell);
	EXPECT_EQ(NULL, CellZero->next);
	EXPECT_EQ(NULL, CellZero->prev);
	free(CellZero);
}

TYPED_TEST(InterpreterTest, Minus) {
	size_t ii;
	signed rc;
	size_t buff_len;
	errno = 0;
	TypeParam* CellZero;
	CellZero = (TypeParam*)calloc(1, sizeof(TypeParam));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	buff_len = 5;
	char in_buff[5];
	for(ii = 0; ii < buff_len; ++ii) {
		in_buff[ii] = '-';
	}
	rc = buffer_eval(in_buff, buff_len, CellZero, NULL, stdin, stdout);
	EXPECT_EQ(0, rc);
	EXPECT_EQ((signed)buff_len * (-1), CellZero->cell);
	EXPECT_EQ(NULL, CellZero->next);
	EXPECT_EQ(NULL, CellZero->prev);
	free(CellZero);
}


TYPED_TEST(InterpreterTest, OverflowCell) {
	size_t ii;
	signed rc;
	size_t buff_len;
	errno = 0;
	TypeParam* CellZero;
	CellZero = (TypeParam*)calloc(1, sizeof(TypeParam));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	buff_len = 1;
	char in_buff[1];
	for(ii = 0; ii < buff_len; ++ii) {
		in_buff[ii] = '+';
	}
	CellZero->cell = LONG_MAX;
	rc = buffer_eval(in_buff, buff_len, CellZero, NULL, stdin, stdout);
	EXPECT_EQ(0, rc);
	EXPECT_EQ(LONG_MIN, CellZero->cell);
	EXPECT_EQ(NULL, CellZero->next);
	EXPECT_EQ(NULL, CellZero->prev);
	free(CellZero);
}

TYPED_TEST(InterpreterTest, UnderflowCell) {
	size_t ii;
	signed rc;
	size_t buff_len;
	errno = 0;
	TypeParam* CellZero;
	CellZero = (TypeParam*)calloc(1, sizeof(TypeParam));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	buff_len = 1;
	char in_buff[1];
	for(ii = 0; ii < buff_len; ++ii) {
		in_buff[ii] = '-';
	}
	CellZero->cell = LONG_MIN;
	rc = buffer_eval(in_buff, buff_len, CellZero, NULL, stdin, stdout);
	EXPECT_EQ(0, rc);
	EXPECT_EQ(LONG_MAX, CellZero->cell);
	EXPECT_EQ(NULL, CellZero->next);
	EXPECT_EQ(NULL, CellZero->prev);
	free(CellZero);
}

TYPED_TEST(InterpreterTest, MoveLeftAlreadyAllocated) {
	signed rc;
	size_t buff_len;
	errno = 0;
	TypeParam* tape, * CellZero, * CellOne;
	tape = (TypeParam*)calloc(2, sizeof(TypeParam));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	CellZero = tape + 0;
	CellOne = tape + 1;
	CellZero->next = CellOne;
	CellOne->prev = CellZero;
	buff_len = 3;
	char in_buff[3] = {'-','<', '+'};
	rc = buffer_eval(in_buff, buff_len, CellOne, NULL, stdin, stdout);
	EXPECT_EQ(0, rc);
	EXPECT_EQ(-1, CellOne->cell);
	EXPECT_EQ(1, CellZero->cell);
	free(tape);
}

TYPED_TEST(InterpreterTest, MoveRightAlreadyAllocated) {
	signed rc;
	size_t buff_len;
	errno = 0;
	TypeParam* tape, * CellZero, * CellOne;
	tape = (TypeParam*)calloc(2, sizeof(TypeParam));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	CellZero = tape;
	CellOne = (tape+1);
	CellZero->next = CellOne;
	CellOne->prev = CellZero;
	buff_len = 3;
	char in_buff[3] = {'-','>', '+'};
	rc = buffer_eval(in_buff, buff_len, tape, NULL, stdin, stdout);
	EXPECT_EQ(0, rc);
	EXPECT_EQ(-1, CellZero->cell);
	EXPECT_EQ(1, CellOne->cell);
	free(tape);
}

TYPED_TEST(InterpreterTest, MoveLeftNotAllocated) {
	signed rc;
	size_t buff_len;
	/*output here is not expected be be that large (<100MB)so we are 
	* justified by keeping in memory*/
	FILE* test_output;
	errno = 0;
	test_output = tmpfile();
	if(test_output == NULL) {
		/*we failed to open an input*/
		perror("failed to create a tmpfile");
		FAIL();
	}
	TypeParam* tape;
	tape = (TypeParam*)calloc(1, sizeof(TypeParam));
	buff_len = 1000000;
	char* in_buff;
	in_buff = (char*)malloc(sizeof(char)*buff_len);
	errno = 0;
	in_buff = (char*)memset(in_buff, '<', buff_len);

	rc = buffer_eval(in_buff, buff_len, tape, NULL, stdin, test_output);
	TypeParam* right, * left;
	right = tape;
	left = tape;
	/*move to end of tape*/
	for(;;) {
		if(left->prev != NULL) {
			left = left->prev;
		} else {
			break;
		}
	}
	fprintf(stdout,"Allocated %ld nodes to left of a[0]\n",buff_len);
	rc = tape_destroy(left);
	/*make sure clean up of tape succeded*/
	EXPECT_EQ(0, rc);
	fclose(test_output);
	free(in_buff);
}

TYPED_TEST(InterpreterTest, MoveRightNotAllocated) {
	signed rc;
	size_t buff_len;
	/*output here is not expected be be that large (<100MB)so we are 
	* justified by keeping in memory*/
	FILE* test_output;
	errno = 0;
	test_output = tmpfile();
	if(test_output == NULL) {
		/*we failed to open an input*/
		perror("failed to create a tmpfile");
		FAIL();
	}
	TypeParam* tape;
	tape = (TypeParam*)calloc(1, sizeof(TypeParam));
	buff_len = 1000000;
	char* in_buff;
	in_buff = (char*)malloc(sizeof(char)*buff_len);
	errno = 0;
	in_buff = (char*)memset(in_buff, '>', buff_len);

	rc = buffer_eval(in_buff, buff_len, tape, NULL, stdin, test_output);
	EXPECT_EQ(0, rc);
	TypeParam* right, * left;
	right = tape;
	left = tape;
	/*move to end of tape*/
	for(;;) {
		if(right->next != NULL) {
			right = right->next;
		} else {
			break;
		}
	}
	fprintf(stdout,"Allocated %ld nodes to right of a[0]\n",buff_len);
	fclose(test_output);
	free(in_buff);
	rc = tape_destroy(left);
	/*make sure clean up of tape succeded*/
	EXPECT_EQ(0, rc);
}

TYPED_TEST(InterpreterTest, MoveLeftIndexIncrementingTest) {
	signed rc;
	size_t buff_len;
	errno = 0;
	TypeParam* tape;
	tape = (TypeParam*)calloc(5, sizeof(TypeParam));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	/*link up the list*/
	(tape+0)->next = (tape+1);
	(tape+1)->prev = (tape+0);
	(tape+1)->next = (tape+2);
	(tape+2)->prev = (tape+1);
	(tape+2)->next = (tape+3);
	(tape+3)->prev = (tape+2);
	(tape+3)->next = (tape+4);
	(tape+4)->prev = (tape+3);
	char in_buff[5] = {'<','<','<','<','+'};	buff_len = 5;
	rc = buffer_eval(in_buff, buff_len, (tape+4), NULL, stdin, stdout);
	EXPECT_EQ(0, rc);
	/*checking that we transferred as far as was expected*/
	EXPECT_EQ(1, (tape+0)->cell);
	/*the move instructions should not change value of passed over cell*/
	EXPECT_EQ(0, (tape+1)->cell);
	EXPECT_EQ(0, (tape+2)->cell);
	EXPECT_EQ(0, (tape+3)->cell);
	EXPECT_EQ(0, (tape+4)->cell);
	free(tape);
}

TYPED_TEST(InterpreterTest, MoveRightIndexIncrementingTest) {
	signed rc;
	size_t buff_len;
	errno = 0;
	TypeParam* tape;
	tape = (TypeParam*)calloc(5, sizeof(TypeParam));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	/*link up the list*/
	(tape+0)->next = (tape+1);
	(tape+1)->prev = (tape+0);
	(tape+1)->next = (tape+2);
	(tape+2)->prev = (tape+1);
	(tape+2)->next = (tape+3);
	(tape+3)->prev = (tape+2);
	(tape+3)->next = (tape+4);
	(tape+4)->prev = (tape+3);
	char in_buff[5] = {'>','>','>','>','+'};
	buff_len = 5;
	rc = buffer_eval(in_buff, buff_len, tape, NULL, stdin, stdout);
	EXPECT_EQ(0, rc);
	/*checking that we transferred as far as was expected*/
	EXPECT_EQ(1, (tape+4)->cell);
	/*the move instructions should not change value of passed over cell*/
	EXPECT_EQ(0, (tape+1)->cell);
	EXPECT_EQ(0, (tape+2)->cell);
	EXPECT_EQ(0, (tape+3)->cell);
	EXPECT_EQ(0, (tape+0)->cell);
	free(tape);
}

TYPED_TEST(InterpreterTest, StackPush) {
	signed rc;
	size_t buff_len;
	Vector_t(long)* stack;
	stack = newVector(long);
	TypeParam* CellZero;
	CellZero = (TypeParam*)calloc(1, sizeof(TypeParam));
	buff_len = 6;
	char in_buff[6] = {'+','+','#','#','#','#'};
	rc = buffer_eval(in_buff, buff_len, CellZero, stack, stdin, stdout);
	EXPECT_EQ(0, rc);
	EXPECT_EQ(4, stack->elms);
	EXPECT_EQ(2, stack->items[0]);
	EXPECT_EQ(2, stack->items[1]);
	EXPECT_EQ(2, stack->items[2]);
	EXPECT_EQ(2, stack->items[3]);
	free(CellZero);
	vector_destroy(long, stack);
}

TYPED_TEST(InterpreterTest, StackPop) {
	signed rc;
	size_t buff_len;
	Vector_t(long)* stack;
	stack = newVector(long);
	TypeParam* CellZero;
	CellZero = (TypeParam*)calloc(1, sizeof(TypeParam));
	buff_len = 1;
	char in_buff[1];
	in_buff[0] = '$';
	vector_push_back(long, stack, 0);
	vector_push_back(long, stack, LONG_MAX);
	vector_push_back(long, stack, LONG_MIN);
	EXPECT_EQ(3, stack->elms);
	rc = buffer_eval(in_buff, buff_len, CellZero, stack, stdin, stdout);
	EXPECT_EQ(LONG_MIN, CellZero->cell);
	EXPECT_EQ(0, rc);
	EXPECT_EQ(2, stack->elms);
	rc = buffer_eval(in_buff, buff_len, CellZero, stack, stdin, stdout);
	EXPECT_EQ(LONG_MAX, CellZero->cell);
	EXPECT_EQ(0, rc);
	EXPECT_EQ(1, stack->elms);
	rc = buffer_eval(in_buff, buff_len, CellZero, stack, stdin, stdout);
	EXPECT_EQ(0, CellZero->cell);
	EXPECT_EQ(0, rc);
	EXPECT_EQ(0, stack->elms);
	rc = buffer_eval(in_buff, buff_len, CellZero, stack, stdin, stdout);
	/*-6 is the return code for failed to pop stack, stack is empty*/
	EXPECT_EQ(-6, rc);
	EXPECT_EQ(0, stack->elms);

	free(CellZero);
	vector_destroy(long, stack);
}

TYPED_TEST(InterpreterTest, BufferOvershootTest) {
	signed rc;
	size_t buff_len;
	size_t ii;
	/*we will create a buffer of commands but artificially
	shorten it so that going past end actually has valid commands*/
	errno = 0;
	TypeParam* CellZero;
	CellZero = (TypeParam*)calloc(1, sizeof(TypeParam));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	buff_len = 3;
	char in_buff[3];
	for(ii = 0; ii < buff_len; ++ii) {
		in_buff[ii] = '+';
	}
	rc = buffer_eval(in_buff, buff_len-1, CellZero, NULL, stdin, stdout);
	EXPECT_EQ(0, rc);
	EXPECT_EQ(2, CellZero->cell);
	EXPECT_EQ(NULL, CellZero->next);
	EXPECT_EQ(NULL, CellZero->prev);
	free(CellZero);
}

/*test the input facilites from stdin and stdout using some other method*/
TYPED_TEST(InterpreterTest, CharacterInputTest) {
	signed rc;
	size_t buff_len;
	size_t ii;
	FILE* test_input, * test_output;
	errno = 0;
	test_input = tmpfile();
	if(test_input == NULL) {
		/*we failed to open an input*/
		perror("failed to create input tmpfile");
		FAIL();
	}
	test_output = tmpfile();
	if(test_output == NULL) {
		/*we failed to open an input*/
		perror("failed to create output tmpfile");
		FAIL();
	}		
	/*send all ascii characters to the file*/
	for(ii = 1; ii < 128; ++ii) {
		fprintf(test_input, "%c", (char)ii);
	}
	rewind(test_input);
	/*construct test program*/
	errno = 0;
	TypeParam* CellZero;
	CellZero = (TypeParam*)calloc(1, sizeof(TypeParam));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	buff_len = 1;
	char in_buff[1];
	in_buff[0] = ',';
	/*read in each character from file*/
	for(ii = 1; ii < 128; ++ii) {
		rc = buffer_eval(in_buff, buff_len, CellZero, NULL, test_input, test_output);
		EXPECT_EQ(0, rc);
		EXPECT_EQ(ii, CellZero->cell);
	}
	/*now try to read past the input of the file*/
	rc = buffer_eval(in_buff, buff_len, CellZero, NULL, test_input, stdout);
	EXPECT_EQ(0, rc);
	EXPECT_EQ(-1, CellZero->cell);
	free(CellZero);
	fclose(test_input);
	fclose(test_output);
}

TYPED_TEST(InterpreterTest, PositiveIntegerInputTest) {
	signed rc;
	size_t buff_len;
	FILE* test_input;
	errno = 0;
	test_input = tmpfile();
	if(test_input == NULL) {
		/*we failed to open an input*/
		perror("failed to create a tmpfile");
		FAIL();
	}
	signed long A, B, C;
	A = 0;
	B = 37;
	C = LONG_MAX;
	/*send some positive integers to the file*/
	fprintf(test_input, "%ld\n%ld\n%ld\n", A, B, C);
	rewind(test_input);
	errno = 0;
	TypeParam* tape;
	tape = (TypeParam*)calloc(3, sizeof(TypeParam));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	/*link up the list*/
	(tape+0)->next = (tape+1);
	(tape+1)->prev = (tape+0);
	(tape+1)->next = (tape+2);
	(tape+2)->prev = (tape+1);
	char in_buff[5] = {';','>',';','>',';'};
	buff_len = 5;
	rc = buffer_eval(in_buff, buff_len, tape, NULL, test_input, stdout);
	EXPECT_EQ(0, rc);
	EXPECT_EQ(A, tape->cell);
	EXPECT_EQ(B, (tape+1)->cell);
	EXPECT_EQ(C, (tape+2)->cell);
	free(tape);
	fclose(test_input);
}

TYPED_TEST(InterpreterTest, NegativeIntegerInputTest) {
	signed rc;
	size_t buff_len;
	FILE* test_input;
	errno = 0;
	test_input = tmpfile();
	if(test_input == NULL) {
		/*we failed to open an input*/
		perror("failed to create a tmpfile");
		FAIL();
	}
	signed long A, B, C;
	A = -1;
	B = -377;
	C = LONG_MIN;
	/*send some positive integers to the file*/
	fprintf(test_input, "%ld\n%ld\n%ld\n", A, B, C);
	rewind(test_input);
	errno = 0;
	TypeParam* tape;
	tape = (TypeParam*)calloc(3, sizeof(TypeParam));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	/*link up the list*/
	(tape+0)->next = (tape+1);
	(tape+1)->prev = (tape+0);
	(tape+1)->next = (tape+2);
	(tape+2)->prev = (tape+1);
	char in_buff[5] = {';','>',';','>',';'};
	buff_len = 5;
	rc = buffer_eval(in_buff, buff_len, tape, NULL, test_input, stdout);
	EXPECT_EQ(0, rc);
	EXPECT_EQ(A, tape->cell);
	EXPECT_EQ(B, (tape+1)->cell);
	EXPECT_EQ(C, (tape+2)->cell);
	free(tape);
	fclose(test_input);
}

TYPED_TEST(InterpreterTest, DanglingRightBracketTest) {
	signed rc;
	size_t buff_len;
	errno = 0;
	TypeParam* CellZero;
	CellZero = (TypeParam*)calloc(1, sizeof(TypeParam));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	char in_buff[5] = {'+','+',']','+','+'};
	buff_len = 5;

	rc = buffer_eval(in_buff, buff_len, CellZero, NULL, stdin, stdout);
	EXPECT_EQ(-9, rc);
	EXPECT_EQ(2, CellZero->cell);
	/*now repeat with the first character being the dangling*/
	in_buff[0] = ']';
	in_buff[2] = '+';
	/*leave a nonzero value in the cell so we continue to*/
	rc = buffer_eval(in_buff, buff_len, CellZero, NULL, stdin, stdout);
	EXPECT_EQ(-9, rc);
	EXPECT_EQ(2, CellZero->cell);
	free(CellZero);
}

TYPED_TEST(InterpreterTest, RighBracketJumpIfZeroTest) {
	signed rc;
	size_t buff_len;
	errno = 0;
	TypeParam* CellZero;
	CellZero = (TypeParam*)calloc(1, sizeof(TypeParam));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	char in_buff[2] = {']','+'};
	buff_len = 2;
	EXPECT_EQ(0, CellZero->cell);
	rc = buffer_eval(in_buff, buff_len, CellZero, NULL, stdin, stdout);
	EXPECT_EQ(0, rc);
	EXPECT_EQ(1, CellZero->cell);
	free(CellZero);
}

TYPED_TEST(InterpreterTest, LeftBracketJumpIfNotZeroTest) {
	signed rc;
	size_t buff_len;
	errno = 0;
	TypeParam* CellZero;
	CellZero = (TypeParam*)calloc(1, sizeof(TypeParam));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	char in_buff[3] = {'+','[','+'};
	buff_len = 3;
	EXPECT_EQ(0, CellZero->cell);
	rc = buffer_eval(in_buff, buff_len, CellZero, NULL, stdin, stdout);
	EXPECT_EQ(0, rc);
	EXPECT_EQ(2, CellZero->cell);
	free(CellZero);
}

TYPED_TEST(InterpreterTest, DanglingLeftBracketTest) {
	signed rc;
	size_t buff_len;
	errno = 0;
	TypeParam* CellZero;
	CellZero = (TypeParam*)calloc(1, sizeof(TypeParam));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	char in_buff[3] = {'[','+','+'};
	buff_len = 3;
	rc = buffer_eval(in_buff, buff_len, CellZero, NULL, stdin, stdout);
	EXPECT_EQ(-8, rc);
	EXPECT_EQ(0, CellZero->cell);
	/*now repeart with the end of buffer as a dangling*/
	in_buff[0] = '[';
	in_buff[1] = '[';
	in_buff[2] = '[';
	rc = buffer_eval(in_buff, buff_len, CellZero, NULL, stdin, stdout);
	EXPECT_EQ(-8, rc);
	EXPECT_EQ(0, CellZero->cell);

	free(CellZero);
}

TYPED_TEST(InterpreterTest, BracketLoopingTest) {
	signed rc;
	size_t buff_len;
	errno = 0;
	TypeParam* CellZero;
	CellZero = (TypeParam*)calloc(1, sizeof(TypeParam));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	/*simplest loop*/
	char in_buff[9] = {'+','+','+','+','[','-',']','+', '+'};
	buff_len = 9;

	rc = buffer_eval(in_buff, buff_len, CellZero, NULL, stdin, stdout);
	EXPECT_EQ(0, rc);
	EXPECT_EQ(2, CellZero->cell);
	free(CellZero);
}

TYPED_TEST(InterpreterTest, LoopNestingTest) {
	signed rc;
	size_t buff_len;
	errno = 0;
	TypeParam* CellZero;
	CellZero = (TypeParam*)calloc(1, sizeof(TypeParam));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	/*simplest loop*/
	const char* in_buff;
	in_buff = "+[[-]+[[-]+[-]]]++";
	buff_len = 18;

	rc = buffer_eval(const_cast<char*>(in_buff), buff_len, CellZero, NULL, stdin, stdout);
	EXPECT_EQ(0, rc);
	EXPECT_EQ(2, CellZero->cell);
	free(CellZero);
}
