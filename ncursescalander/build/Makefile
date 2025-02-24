# Makefile for ncurses calendar app

CC = gcc
CFLAGS = -Wall -Wextra -I../include
LDFLAGS = -lncurses -lsqlite3
SRC_DIR = ../src
BUILD_DIR = ../build

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

TARGET = calendar_app

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET)

.PHONY: all clean

