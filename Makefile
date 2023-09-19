CC = gcc
SRC = examples/*.c

all: directories examples

directories:
	mkdir -p "bin"

bin/main_example:
	$(CC) examples/append.c -o bin/append
	$(CC) examples/format.c -o bin/format
	$(CC) examples/insert.c -o bin/insert
	$(CC) examples/replace.c -o bin/replace
	$(CC) examples/bits.c -o bin/bits
	$(CC) examples/main_example.c -o bin/main_example

debug:
	$(CC) -g examples/append.c -o bin/append
	$(CC) -g examples/format.c -o bin/format
	$(CC) -g examples/insert.c -o bin/insert
	$(CC) -g examples/replace.c -o bin/replace
	$(CC) -g examples/bits.c -o bin/bits
	$(CC) -g examples/main_example.c -o bin/main_example

run: all
	./bin/append
	./bin/format
	./bin/insert
	./bin/replace
	./bin/bits
	./bin/main_example
