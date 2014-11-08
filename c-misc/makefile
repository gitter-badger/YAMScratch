
BIN = ./bin/
SOURCE = ./source/

LIST=$(BIN)ex16 $(BIN)ex17

all: $(LIST)

$(BIN)% : $(SOURCE)%.c
	$(CC) $(INC) $< $(CFLAGS) -o $@ $(LIBS)

clean:
	$(RM) $(LIST)