CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRC = main.c server.c router.c handler.c sysinfo.c
OBJ = $(SRC:.c=.o)

all: syspulse

syspulse: $(OBJ)
	$(CC) $(CFLAGS) -o syspulse $(OBJ)

clean:
	rm -f syspulse $(OBJ)

