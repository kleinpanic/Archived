# Makefile for building the Zeta Psi backend server with an obj/ dir.

CC      := gcc
CFLAGS  := -Wall -O2 -std=c99 -Iinclude
LIBS    := -lmicrohttpd -lsqlite3 -lssl -lcrypto

SRCDIR  := src
OBJDIR  := obj
TARGET  := frat_server

# Source and object lists
SRC     := $(wildcard $(SRCDIR)/*.c)
OBJ     := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

.PHONY: all clean

all: $(OBJDIR) $(TARGET)

# Ensure obj directory exists
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Link the final binary
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Compile each source into obj/*.o
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)

