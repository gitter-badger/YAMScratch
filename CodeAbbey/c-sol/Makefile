BIN = ./bin
SRC = ./src
OBJ = ./obj
INC = ./inc

CC = clang
CFLAGS = -Wall -Wextra --pedantic-errors -O3 -g


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

15 : $(BIN)/15.out

$(BIN)/15.out : $(SRC)/15.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

20 : $(BIN)/20.out

$(BIN)/20.out : $(SRC)/20.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

28 : $(BIN)/28.out

$(BIN)/28.out : $(SRC)/28.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

41 : $(BIN)/41.out

$(BIN)/41.out : $(SRC)/41.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

43 : $(BIN)/43.out

$(BIN)/43.out : $(SRC)/43.c
	$(CC) $(CFLAGS) -I $(INC) $< -o $@

clean : 
	rm -f $(BIN)/* $(OBJ)/*
	