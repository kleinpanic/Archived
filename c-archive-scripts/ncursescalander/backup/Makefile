# Compiler and Flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS = -lncurses

# Source Files
SRCS = main.c
OBJS = $(SRCS:.c=.o)

# Output Binary
TARGET = calendar

# Default Target
all: $(TARGET)

# Build Target
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Object Files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean Target
clean:
	rm -f $(OBJS) $(TARGET)

# Run Target
run: $(TARGET)
	./$(TARGET)

# Phony Targets
.PHONY: all clean run
