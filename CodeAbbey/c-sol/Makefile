BIN = ./bin
SRC = ./src
OBJ = ./obj
INC = ./inc

CC = gcc
CFLAGS = -Wall -Wextra --pedantic-errors -O3 -g


1 : $(BIN)/1.out

$(BIN)/1.out : $(SRC)/1.c
	$(CC) $(CFLAGS) $< -o $@

2 : $(BIN)/2.out

$(BIN)/2.out : $(SRC)/2.c
	$(CC) $(CFLAGS) $< -o $@

3 : $(BIN)/3.out

$(BIN)/3.out : $(SRC)/3.c
	clang $(CFLAGS) -I $(INC) $< -o $@

clean : 
	rm -f $(BIN)/* $(OBJ)/*
	