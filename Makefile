CC=g++
DB=gdb

CFLAGS=-Wall -Wextra -Iinclude $(LIBS)
DEBUG_FLAGS=-g$(DB) -DDEBUG -fsanitize=address
RELEASE_FLAGS=-O2 -s -DNDEBUG

SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin
INCLUDE_DIR=include

SRCS=$(wildcard $(SRC_DIR)/*.cpp)
OBJS=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
LIBS=$(addprefix -l, raylib GL m pthread dl rt X11)

BIN=$(BIN_DIR)/xspacewar

all: debug

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

debug: CFLAGS+=$(DEBUG_FLAGS)
debug: $(BIN)

release: CFLAGS+=$(RELEASE_FLAGS)
release: clean
release: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*
