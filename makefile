# constants
EXE_NAME=proj1
SRC_DIR=./src
BIN_DIR=./bin
INCLUDE=-I$(SRC_DIR)
WARNINGS=-Wall
STANDARD=-std=c++11
OPT=-O2
ADGG=-MMD
SRC_EXT=.cpp
OBJ_EXT=.o
CC=g++

$(BIN_DIR)/$(EXE_NAME):

debug: CFLAGS+=-g
debug: OPT=
debug: $(BIN_DIR)/$(EXE_NAME)
	gdb $(BIN_DIR)/$(EXE_NAME)

CFLAGS=$(OPT) $(INCLUDE) $(WARNINGS) $(STANDARD) $(ADGG)
LFLAGS=$(CFLAGS)

CPPFILES=$(wildcard $(SRC_DIR)/*$(SRC_EXT))
OBJFILES=$(addprefix $(BIN_DIR)/,$(notdir $(CPPFILES:$(SRC_EXT)=$(OBJ_EXT))))

$(BIN_DIR)/$(EXE_NAME): $(OBJFILES)
	$(CC) $(LFLAGS) -o $@ $^

$(BIN_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%$(SRC_EXT)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm $(BIN_DIR)/*$(OBJ_EXT)

clean_all: clean
	rm $(BIN_DIR)/$(EXE_NAME)

-include $(OBJFILES:$(OBJ_EXT)=.d)
