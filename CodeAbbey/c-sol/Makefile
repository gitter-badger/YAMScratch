BIN = bin
SRC = src
OBJ = obj
INC = inc
LDIR = lib

CC = clang
CXX = clang++
#-Werror
CFLAGS = -Wall -Wextra --pedantic-errors -g

LIBS = -L $(LDIR)

# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
GTEST_DIR =  ~/Documents/GitHub/googletest

# Where to find user code.
USER_DIR = $(GTEST_DIR)/samples

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include -isystem $(GTEST_DIR)

# Flags passed to the C++ compiler.
# 	Unused function is turned off because of the macros which implement generic types
# 	define everythin even if we only want to use one
CXXFLAGS += -g -Wall -Wextra -pthread --pedantic-errors -Wno-unused-function -Wc++-compat

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
# TESTS = $(BIN)/big_int_unittest 
TESTS =  $(BIN)/big_int_unittest.out

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
				$(GTEST_DIR)/include/gtest/internal/*.h

# House-keeping build targets.
test : $(TESTS)

# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
$(OBJ)/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
			$(GTEST_DIR)/src/gtest-all.cc -o $@

$(OBJ)/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
			$(GTEST_DIR)/src/gtest_main.cc -o $@

$(LDIR)/gtest.a : $(OBJ)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(LDIR)/gtest_main.a : $(OBJ)/gtest-all.o $(OBJ)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

#========================================
# 			BIG INT TEST
#========================================
#	depends on yam_big_int in UTILITY FUNCTIONS

$(OBJ)/yam_big_int_unittest.o : $(SRC)/yam_big_int_unittest.cc \
					  $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I $(INC) -c $< -o $@

$(BIN)/big_int_unittest.out : $(OBJ)/yam_big_int.o $(OBJ)/yam_big_int_unittest.o  \
						$(LDIR)/gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

#========================================
# 			UTILITY FUNCTIONS
#========================================

$(OBJ)/yam_big_int.o : $(SRC)/yam_big_int.c $(INC)/yam_big_int.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -x c++ -I $(INC) -c $< -o $@

$(OBJ)/yam_fibonacci.o : $(SRC)/yam_fibonacci.c $(INC)/yam_fibonacci.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -x c++ -I $(INC) -c $< -o $@
#========================================
# 		CODE ABBEY PROBLEMS SETS
#========================================

1 : $(BIN)/1.out

$(BIN)/1.out : $(SRC)/1.c
	$(CC) $(CFLAGS) $< -o $@

2 : $(BIN)/2.out

$(BIN)/2.out : $(SRC)/2.c
	$(CC) $(CFLAGS) $< -o $@

3 : $(BIN)/3.out

$(BIN)/3.out : $(SRC)/3.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

4 : $(BIN)/4.out

$(BIN)/4.out : $(SRC)/4.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

5 : $(BIN)/5.out

$(BIN)/5.out : $(SRC)/5.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

6 : $(BIN)/6.out

$(BIN)/6.out : $(SRC)/6.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

7 : $(BIN)/7.out

$(BIN)/7.out : $(SRC)/7.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

8 : $(BIN)/8.out

$(BIN)/8.out : $(SRC)/8.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

9 : $(BIN)/9.out

$(BIN)/9.out : $(SRC)/9.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

10 : $(BIN)/10.out

$(BIN)/10.out : $(SRC)/10.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

11 : $(BIN)/11.out

$(BIN)/11.out : $(SRC)/11.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

12 : $(BIN)/12.out

$(BIN)/12.out : $(SRC)/12.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

13 : $(BIN)/13.out

$(BIN)/13.out : $(SRC)/13.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

14 : $(BIN)/14.out

$(BIN)/14.out : $(SRC)/14.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

15 : $(BIN)/15.out

$(BIN)/15.out : $(SRC)/15.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

16 : $(BIN)/16.out

$(BIN)/16.out : $(SRC)/16.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

17 : $(BIN)/17.out

$(BIN)/17.out : $(SRC)/17.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

20 : $(BIN)/20.out

$(BIN)/20.out : $(SRC)/20.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

21 : $(BIN)/21.out

$(BIN)/21.out : $(SRC)/21.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

24 : $(BIN)/24.out

$(BIN)/24.out : $(SRC)/24.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

26 : $(BIN)/26.out

$(BIN)/26.out : $(SRC)/26.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

27 : $(BIN)/27.out

$(BIN)/27.out : $(SRC)/27.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

28 : $(BIN)/28.out

$(BIN)/28.out : $(SRC)/28.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

29 : $(BIN)/29.out

$(BIN)/29.out : $(SRC)/29.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

30 : $(BIN)/30.out

$(BIN)/30.out : $(SRC)/30.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

32 : $(BIN)/32.out

$(BIN)/32.out : $(SRC)/32.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

41 : $(BIN)/41.out

$(BIN)/41.out : $(SRC)/41.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

43 : $(BIN)/43.out

$(BIN)/43.out : $(SRC)/43.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

48 : $(BIN)/48.out

$(BIN)/48.out : $(SRC)/48.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@


67 : $(BIN)/67.out

$(OBJ)/67.o : $(SRC)/67.c
	$(CC) $(CFLAGS) -Wno-unused-function -I $(INC) -c $< -o $@

$(BIN)/67.out : $(OBJ)/yam_big_int.o $(OBJ)/yam_fibonacci.o $(OBJ)/67.o 
	$(CC) $(CFLAGS) $^ -o $@

81 : $(BIN)/81.out

$(BIN)/81.out : $(SRC)/81.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@
	
112 : $(BIN)/112.out

$(BIN)/112.out : $(SRC)/112.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

clean :
	rm -vf $(TESTS) $(LDIR)/gtest.a $(LDIR)/gtest_main.a $(OBJ)/*.o $(BIN)/*
