# constants
EXE_NAME=proj1
SRC_DIR=./src
BIN_DIR=./bin
INCLUDE=-I$(SRC_DIR)
WARNINGS=-Wall
STANDARD=-std=c++11
OPT=-O2
PROF=-pg
ADGG=-MMD
SRC_EXT=.cpp
OBJ_EXT=.o
CC=g++
MAIN=$(BIN_DIR)/$(EXE_NAME)

$(BIN_DIR)/$(EXE_NAME):

debug: CFLAGS+=-g
debug: OPT=
debug: $(MAIN)
	gdb $(MAIN)

prof: CFLAGS+=$(PROF)
prof: $(MAIN)
	$(MAIN)
	gprof $(MAIN) > $(MAIN).prof

CFLAGS=$(OPT) $(INCLUDE) $(WARNINGS) $(STANDARD) $(ADGG)
LFLAGS=$(CFLAGS)

CPPFILES=$(wildcard $(SRC_DIR)/*$(SRC_EXT))
OBJFILES=$(addprefix $(BIN_DIR)/,$(notdir $(CPPFILES:$(SRC_EXT)=$(OBJ_EXT))))

$(MAIN): $(OBJFILES)
	$(CC) $(LFLAGS) -o $@ $^

$(BIN_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%$(SRC_EXT)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm $(BIN_DIR)/*$(OBJ_EXT)

clean_all: clean
	rm $(MAIN)

-include $(OBJFILES:$(OBJ_EXT)=.d)
