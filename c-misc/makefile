
BIN = ./bin/
SOURCE = ./source/

LIST=$(BIN)ex16 $(BIN)ex17
CC = gcc

all: $(LIST)


$(BIN)% : $(SOURCE)%.c
	$(CC) $(INC) $< $(CFLAGS) -o $@ $(LIBS)

$(BIN)% : $(SOURCE)%.cpp
	g++ $(INC) $< $(CFLAGS) -o $@ $(LIBS)

clean:
	$(RM) $(LIST)