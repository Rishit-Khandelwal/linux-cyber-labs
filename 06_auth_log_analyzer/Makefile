CC=gcc
CFLAGS=-Wall -Wextra -O2 -Iinclude
SRC=src/main.c src/parser.c
TARGET=auth_log_analyzer

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

