
BIN = ./bin/
SDIR = ./src/
IDIR = ./inc/
ODIR = ./src/obj/

CC = gcc
CFLAGS= -Wall -I$(IDIR) --pedantic-errors --std=gnu99

_DEPS = badVertexVector.h slicerBinaryReader.h slicerConstants.h slicerTypes.h
DEPS = $(patsubst %, $(IDIR)%, $(_DEPS))

_OBJ = slicerBinaryReader.o
OBJ = $(patsubst %, $(ODIR)%, $(_OBJ))

all: $(BIN)slicer

$(BIN)slicer : ./src/obj/slicer.o $(OBJ)
	$(CC) $< $(OBJ) -o $@ $(CFLAGS) 

$(ODIR)slicer.o : $(SDIR)slicer.c $(DEPS)
	$(CC) -c $(CFLAGS) $< -o $@

$(ODIR)slicerBinaryReader.o : $(SDIR)slicerBinaryReader.c $(DEPS)
	$(CC) -c $(CFLAGS) $< -o $@

#prevent anything from happening to a file named clean
.FAKE : clean

clean:
	$(RM) $(BIN)slicer $(ODIR)*.o
