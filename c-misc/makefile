
BIN = ./bin/
SOURCE = ./source/

LIST=$(BIN)ex16 $(BIN)ex17 $(BIN)just
CC = gcc

all: $(LIST)


$(BIN)% : $(SOURCE)%.c
	$(CC) $(INC) $< $(CFLAGS) -o $@ $(LIBS)

$(BIN)just : $(SOURCE)just.cpp
	g++ $(SOURCE)ellipse.cpp $< -o $@


#$(BIN)% : $(SOURCE)%.cpp
#	g++ $(INC) $< $(CFLAGS) -o $@ $(LIBS)

clean:
	$(RM) $(LIST)