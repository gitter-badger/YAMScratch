
BIN = ./bin/
SOURCE = ./source/

LIST=$(BIN)ex16

all: $(LIST)

$(BIN)% : $(SOURCE)%.c
	$(CC) $(INC) $< $(CFLAGS) -o $@ $(LIBS)

clean:
	$(RM) $(BIN)ex16