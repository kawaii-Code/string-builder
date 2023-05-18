CC = gcc
SRC = examples/*.c

all: directories examples

directories:
	mkdir -p "bin"

examples: $(SRC)
	$(CC) examples/append.c -o bin/append
	$(CC) examples/format.c -o bin/format
	$(CC) examples/insert.c -o bin/insert
	$(CC) examples/replace.c -o bin/replace
	$(CC) examples/bits.c -o bin/bits
	$(CC) examples/main_example.c -o bin/main_example
