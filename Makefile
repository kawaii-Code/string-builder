TARGET = example
SRC = example.c string_builder.c

all: $(SRC)
	gcc $(SRC) -o $(TARGET) -Wall -Wextra -Wpedantic

run: all
	./$(TARGET)
