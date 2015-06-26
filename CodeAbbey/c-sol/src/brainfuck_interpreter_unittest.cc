#include <stdint.h>
#include <stdio.h>
#include <iomanip>
#include <limits.h>
#include <errno.h>
#include <cstring>

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
	struct TapeNodeDebug* CellZero;
	CellZero = (struct TapeNodeDebug*)calloc(1, sizeof(struct TapeNodeDebug));
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
	struct TapeNodeDebug* CellZero;
	CellZero = (struct TapeNodeDebug*)calloc(1, sizeof(struct TapeNodeDebug));
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
	struct TapeNodeDebug* CellZero;
	CellZero = (struct TapeNodeDebug*)calloc(1, sizeof(struct TapeNodeDebug));
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
	struct TapeNodeDebug* CellZero;
	CellZero = (struct TapeNodeDebug*)calloc(1, sizeof(struct TapeNodeDebug));
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
	errno = 0;
	struct TapeNodeDebug* tape;
	tape = (struct TapeNodeDebug*)calloc(2, sizeof(struct TapeNodeDebug));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	struct TapeNodeDebug* CellZero, * CellOne;
	CellZero = tape + 0;
	CellOne = tape + 1;
	CellZero->next = CellOne;
	CellOne->prev = CellZero;
	CellOne->index = 1;
	CellZero->index = 0;
	size_t buff_len;
	buff_len = 3;
	char in_buff[3] = {'-','<', '+'};
	_eval_buffer_debug(in_buff, buff_len, CellOne, NULL, stdin, stdout);
	EXPECT_EQ(-1, CellOne->cell);
	EXPECT_EQ(1, CellZero->cell);
	EXPECT_EQ(1, CellOne->index);
	EXPECT_EQ(0, CellZero->index);
	free(tape);
}


TEST_F(InterpreterTest, MoveRightAlreadyAllocated) {
	errno = 0;
	struct TapeNodeDebug* tape;
	tape = (struct TapeNodeDebug*)calloc(2, sizeof(struct TapeNodeDebug));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	struct TapeNodeDebug* CellZero, * CellOne;
	CellZero = tape;
	CellOne = (tape+1);
	CellZero->next = CellOne;
	CellOne->prev = CellZero;
	CellOne->index = 1;
	CellZero->index = 0;
	size_t buff_len;
	buff_len = 3;
	char in_buff[3] = {'-','>', '+'};
	_eval_buffer_debug(in_buff, buff_len, tape, NULL, stdin, stdout);
	EXPECT_EQ(-1, CellZero->cell);
	EXPECT_EQ(1, CellOne->cell);
	EXPECT_EQ(1, CellOne->index);
	EXPECT_EQ(0, CellZero->index);
	free(tape);
}

TEST_F(InterpreterTest, MoveLeftNotAllocated) {
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
	struct TapeNodeDebug* tape;
	tape = (struct TapeNodeDebug*)calloc(1, sizeof(struct TapeNodeDebug));
	size_t buff_len = 1000000;
	char* in_buff;
	in_buff = (char*)malloc(sizeof(char)*buff_len);
	errno = 0;
	in_buff = (char*)memset(in_buff, '<', buff_len);

	_eval_buffer_debug(in_buff, buff_len, tape, NULL, stdin, test_output);
	struct TapeNodeDebug* right, * left;
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
	EXPECT_EQ(0, right->index);
	EXPECT_EQ((-1*buff_len), left->index);
	fprintf(stdout,"Allocated %ld nodes to left of a[0]\n",buff_len);
	rc = _destroy_tape_debug(left);
	/*make sure clean up of tape succeded*/
	EXPECT_EQ(0, rc);
	fclose(test_output);
	free(in_buff);
}

TEST_F(InterpreterTest, MoveRightNotAllocated) {
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
	struct TapeNodeDebug* tape;
	tape = (struct TapeNodeDebug*)calloc(1, sizeof(struct TapeNodeDebug));
	size_t buff_len = 1000000;
	char* in_buff;
	in_buff = (char*)malloc(sizeof(char)*buff_len);
	errno = 0;
	in_buff = (char*)memset(in_buff, '>', buff_len);

	_eval_buffer_debug(in_buff, buff_len, tape, NULL, stdin, test_output);
	struct TapeNodeDebug* right, * left;
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
	EXPECT_EQ(0, left->index);
	EXPECT_EQ(buff_len, right->index);
	fprintf(stdout,"Allocated %ld nodes to right of a[0]\n",buff_len);
	rc = _destroy_tape_debug(left);
	/*make sure clean up of tape succeded*/
	EXPECT_EQ(0, rc);
	fclose(test_output);
	free(in_buff);
}

TEST_F(InterpreterTest, MoveLeftIndexIncrementingTest) {
	errno = 0;
	struct TapeNodeDebug* tape;
	tape = (struct TapeNodeDebug*)calloc(5, sizeof(struct TapeNodeDebug));
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
	/*start at end of tape with positive index to test both pos and negative indices*/
	(tape+4)->index = 2;
	char in_buff[5] = {'<','<','<','<','+'};
	size_t buff_len = 5;
	_eval_buffer_debug(in_buff, buff_len, (tape+4), NULL, stdin, stdout);
	EXPECT_EQ(2, (tape+4)->index);
	EXPECT_EQ(1, (tape+3)->index);
	EXPECT_EQ(0, (tape+2)->index);
	EXPECT_EQ(-1, (tape+1)->index);
	EXPECT_EQ(-2, (tape+0)->index);
	/*checking that we transferred as far as was expected*/
	EXPECT_EQ(1, (tape+0)->cell);
	/*the move instructions should not change value of passed over cell*/
	EXPECT_EQ(0, (tape+1)->cell);
	EXPECT_EQ(0, (tape+2)->cell);
	EXPECT_EQ(0, (tape+3)->cell);
	EXPECT_EQ(0, (tape+4)->cell);
	free(tape);
}

TEST_F(InterpreterTest, MoveRightIndexIncrementingTest) {
	errno = 0;
	struct TapeNodeDebug* tape = (struct TapeNodeDebug*)calloc(5, sizeof(struct TapeNodeDebug));
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
	/*start at end of tape with positive index to test both pos and negative indices*/
	(tape+0)->index = -2;
	char in_buff[5] = {'>','>','>','>','+'};
	size_t buff_len = 5;
	_eval_buffer_debug(in_buff, buff_len, tape, NULL, stdin, stdout);
	EXPECT_EQ(2, (tape+4)->index);
	EXPECT_EQ(1, (tape+3)->index);
	EXPECT_EQ(0, (tape+2)->index);
	EXPECT_EQ(-1, (tape+1)->index);
	EXPECT_EQ(-2, (tape+0)->index);
	/*checking that we transferred as far as was expected*/
	EXPECT_EQ(1, (tape+4)->cell);
	/*the move instructions should not change value of passed over cell*/
	EXPECT_EQ(0, (tape+1)->cell);
	EXPECT_EQ(0, (tape+2)->cell);
	EXPECT_EQ(0, (tape+3)->cell);
	EXPECT_EQ(0, (tape+0)->cell);
	free(tape);
}

TEST_F(InterpreterTest, StackPush) {
	printf("Stack pushing test\n");
}

TEST_F(InterpreterTest, StackPop) {
	printf("Stack popping test\n");
}

TEST_F(InterpreterTest, BufferOvershootTest) {
	/*we will create a buffer of commands but artificially
	shorten it so that going past end actually has valid commands*/
	errno = 0;
	struct TapeNodeDebug* CellZero = (struct TapeNodeDebug*)calloc(1, sizeof(struct TapeNodeDebug));
	if(errno != 0) {
		perror("failed to allocate tape node");
		FAIL();
	}
	size_t buff_len;
	buff_len = 3;
	char in_buff[3];
	for(ii = 0; ii < buff_len; ++ii) {
		in_buff[ii] = '+';
	}
	_eval_buffer_debug(in_buff, buff_len-1, CellZero, NULL, stdin, stdout);
	EXPECT_EQ(2, CellZero->cell);
	EXPECT_EQ(0, CellZero->index);
	EXPECT_EQ(NULL, CellZero->next);
	EXPECT_EQ(NULL, CellZero->prev);

}
/*test the input facilites from stdin and stdout using some other method*/
TEST_F(InterpreterTest, CharacterInputTest) {
	FILE* test_input;
	errno = 0;
	test_input = tmpfile();
	if(test_input == NULL) {
		/*we failed to open an input*/
		perror("failed to create a tmpfile");
		FAIL();
	}	
	/*send all ascii characters to the file*/
	for(ii = 1; ii < 128; ++ii) {
		fprintf(test_input, "%c", (char)ii);
	}
	rewind(test_input);
	/*construct test program*/
	size_t buff_len;
	buff_len = 5;



	fclose(test_input);
}

TEST_F(InterpreterTest, PositiveIntegerInputTest) {
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
	struct TapeNodeDebug* tape = (struct TapeNodeDebug*)calloc(3, sizeof(struct TapeNodeDebug));
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
	size_t buff_len = 5;
	_eval_buffer_debug(in_buff, buff_len, tape, NULL, test_input, stdout);
	EXPECT_EQ(A, tape->cell);
	EXPECT_EQ(B, (tape+1)->cell);
	EXPECT_EQ(C, (tape+2)->cell);
	free(tape);
	fclose(test_input);
}

TEST_F(InterpreterTest, NegativeIntegerInputTest) {
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
	struct TapeNodeDebug* tape = (struct TapeNodeDebug*)calloc(3, sizeof(struct TapeNodeDebug));
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
	size_t buff_len = 5;
	_eval_buffer_debug(in_buff, buff_len, tape, NULL, test_input, stdout);
	EXPECT_EQ(A, tape->cell);
	EXPECT_EQ(B, (tape+1)->cell);
	EXPECT_EQ(C, (tape+2)->cell);
	free(tape);
	fclose(test_input);
}
