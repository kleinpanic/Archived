CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# The final executable name
TARGET = terrarium

# Source files
SRCS = main.c game.c ascii_plant_data.c

# Object files
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET) *.o
	rm -f terrarium_save.dat

