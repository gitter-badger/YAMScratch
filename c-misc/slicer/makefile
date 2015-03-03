
BIN = ./bin/
SOURCE = ./src/
IDIR = ./inc/

LIST=$(BIN)slicer
CC = gcc
CFLAGS= -Wall -I$(IDIR) --pedantic-errors --std=gnu99

all: $(LIST)

$(BIN)% : $(SOURCE)%.c
	$(CC) $(INC) $< $(CFLAGS) -o $@ $(LIBS)

clean:
	$(RM) $(LIST)