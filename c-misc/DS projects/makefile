BIN = ./bin/

INC = ./include/
CFLAGS = -Wall -g
# $< is the source  $@ is the target

#LIST = $(BIN)hw1
#LIST = $(BIN)hw2
LIST = $(BIN)hw3

all: $(LIST)

$(BIN)hw1 : ./src/hw1.cpp
	g++ -g $(INC)ellipse.cpp $< $(CFLAGS) -o $@

HW2_INC = $(INC)bowling_frame.cpp $(INC)bowling_player.cpp $(INC)bowling_game.cpp $(INC)parser_bowling.cpp

$(BIN)hw2 : ./src/hw2.cpp
	g++ $(HW2_INC) $< $(CFLAGS) -o $@

HW3_INC =

$(BIN)hw3 : ./src/main3.cpp
	g++ $(HW3_INC) $< $(CFLAGS) -o $@

HW6_INC = 

$(BIN)hw6 : ./src/hw6.cpp
	g++ $(HW6_INC) $< $(CFLAGS) -o $@

clean:
	$(RM) $(LIST)