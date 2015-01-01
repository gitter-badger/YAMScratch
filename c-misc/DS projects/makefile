BIN = ./bin/

INC = ./include/
CFLAGS = -Wall
# $< is the source  $@ is the target

#LIST = $(BIN)hw1
LIST = $(BIN)hw2

all: $(LIST)

$(BIN)hw1 : ./src/hw1.cpp
	g++ -g $(INC)ellipse.cpp $< $(CFLAGS) -o $@

$(BIN)hw2 : ./src/hw2.cpp
	g++ -g $(INC)bowling_frame.h $< $(CFLAGS) -o $@

clean:
	$(RM) $(LIST)