BIN = ./bin/

INC = ./include/
CFLAGS = -Wall
# $< is the source  $@ is the target

#LIST = $(BIN)hw1
#LIST = $(BIN)hw2
LIST = $(BIN)hw3

all: $(LIST)

$(BIN)hw1 : ./src/hw1.cpp
	g++ -g $(INC)ellipse.cpp $< $(CFLAGS) -o $@

HW2_INC = $(INC)bowling_frame.cpp $(INC)bowling_player.cpp $(INC)bowling_game.cpp $(INC)parser_bowling.cpp

$(BIN)hw2 : ./src/hw2.cpp
	g++ -g $(HW2_INC) $< $(CFLAGS) -o $@

HW3_INC = 

$(BIN)hw3 : ./src/hw3.cpp
	g++ -g $(HW3_INC) $< $(CFLAGS) -o $@

clean:
	$(RM) $(LIST)