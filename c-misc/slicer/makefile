
BIN = ./bin/
SDIR = ./src/
IDIR = ./inc/
ODIR = ./src/obj/

LIST=$(BIN)slicer
CC = gcc
CFLAGS= -Wall -I$(IDIR) --pedantic-errors --std=gnu99

_DEPS = badVertexVector.h slicerBinaryReader.h slicerConstants.h slicerTypes.h
DEPS = $(patsubst %, $(IDIR)%, $(_DEPS))

_OBJ = slicerBinaryReader.o
OBJ = $(patsubst %, $(ODIR)%, $(_OBJ))

all: $(LIST)

$(BIN)slicer : $(ODIR)slicer.o $(OBJ)
	$(CC) $< $(CFLAGS) -o $@ $(LIBS)

$(ODIR)slicer.o : $(SDIR)slicer.c $(DEPS) $(OBJ)
	$(CC) -c $(CFLAGS) $< -o $@

$(ODIR)slicerBinaryReader.o : $(SDIR)slicerBinaryReader.c $(DEPS)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(RM) $(LIST) $(ODIR)*.o
