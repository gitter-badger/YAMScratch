BIN = ./bin/

# $< is the source  $@ is the target

$(BIN)hw1 : ./src/hw1.cpp
	g++ ./include/ellipse.cpp $< $(CFLAGS) -o $@

clean:
	$(RM) $(BIN)hw1