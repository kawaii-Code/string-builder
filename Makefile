SRC = examples/*.c

all: directories examples

directories:
	mkdir -p "bin"

examples: $(SRC)
	gcc examples/append.c -o bin/append
	gcc examples/format.c -o bin/format
	gcc examples/insert.c -o bin/insert
	gcc examples/replace.c -o bin/replace
	gcc examples/main_example.c -o bin/main_example
