BIN = ./bin
OBJ = ./obj
SRC = ./src
INC = ./include
CFLAGS = -Wall -g --pedantic-errors -Wextra
# $< is the source  $@ is the target

#LIST = $(BIN)/hw1
#LIST = $(BIN)/hw2
#LIST = $(BIN)/hw3
LIST = $(BIN)/hw6

all: $(LIST)

$(BIN)/hw1 : ./src/hw1.cpp
	g++ -g $(INC)ellipse.cpp $< $(CFLAGS) -o $@

HW2_INC = $(INC)/bowling_frame.cpp $(INC)/bowling_player.cpp $(INC)/bowling_game.cpp $(INC)/parser_bowling.cpp

$(BIN)/hw2 : ./src/hw2.cpp
	g++ $(HW2_INC) $< $(CFLAGS) -o $@

HW3_INC =

$(BIN)/hw3 : $(SRC)/main3.cpp
	g++ $(HW3_INC) $< $(CFLAGS) -o $@



#======================================================

#compile and link with this step
$(BIN)/hw6 : $(OBJ)/hw6.o $(OBJ)/carcassonne_placeTile.o
	g++ $(CFLAGS) $^ $(CFLAGS) -o $@


HW6_INC = -I $(INC)/
#compiles all of the object files here
$(OBJ)/hw6.o : $(SRC)/hw6.cpp
	g++ $(CFLAGS) $(HW6_INC) -c $< -o $@


$(OBJ)/carcassonne_placeTile.o : $(SRC)/carcassonne_placeTile.cpp
	g++ $(CFLAGS) $(HW6_INC) -c $< -o $@

clean:
	$(RM) $(LIST) $(OBJ)/*