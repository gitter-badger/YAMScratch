
BIN = ./bin/
SOURCE = ./src/
IDIR = ./inc/

LIST=$(BIN)slicer
CC = g++
CFLAGS= -Wall -I$(IDIR) --pedantic-errors

all: $(LIST)

$(BIN)% : $(SOURCE)%.c
	$(CC) $(INC) $< $(CFLAGS) -o $@ $(LIBS)

clean:
	$(RM) $(LIST)