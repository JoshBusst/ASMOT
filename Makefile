CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Ilib/mylib/include

SRC = src/main.c lib/*.c
OUT = bin/app

$(OUT): $(SRC)
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)