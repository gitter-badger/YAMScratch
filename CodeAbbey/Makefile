BIN = ./bin
SRC = ./src
OBJ = ./obj

CC = gcc
CFLAGS = -Wall -Wextra --pedantic-errors -O3 -g


1 : $(BIN)/1.out

$(BIN)/1.out : $(SRC)/1.c
	$(CC) $(CFLAGS) $< -o $@


clean : 
	rm -f $(BIN)/* $(OBJ)/*