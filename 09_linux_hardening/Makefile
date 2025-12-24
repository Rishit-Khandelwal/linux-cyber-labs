CC=gcc
CFLAGS=-Wall -Wextra -O2 -Iinclude
SRC=src/main.c src/hardening.c
TARGET=linux_hardening

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

