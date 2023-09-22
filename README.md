# string-builder
An stb-style library that provides efficient and convenient dynamic string construction.

This string builder was built with effeciency in mind and it tries not to depend on the standard library too much.
Under the hood it uses a simple dynamic array (also called a list or vector) to store the string.
All the functions avoid any dynamic memory allocations for anything other than to increase storage for the string built.

For those who don't like the long function names, there is a variant [strbld.h](strbld.h) with significantly less typing.

## Features
- Appending a string
- Appending a formatted string (not all format specifiers are supported)
- Replacing a substring with another string
- Inserting a string at the given index
- Other small functions, like appending a value in it's bit representation

# Example
```c
#include <stdio.h>

#define STRING_BUILDER_IMPLEMENTATION
#include "string_builder.h"

#define FIELD_SIZE 10

int main() {
    StringBuilder b = string_builder_new();
    StringBuilder *builder = &b; // Unfortunately, this boilerplate is needed.

    string_builder_append_format(builder, "Building a %dx%d field:\n", FIELD_SIZE, FIELD_SIZE);

    for (int y = 0; y < FIELD_SIZE; y++) {
        string_builder_append_char(builder, '|');
        for (int x = 0; x < FIELD_SIZE; x++) {
            string_builder_append(builder, ".#..");
        }
        string_builder_append_char(builder, '|');
        string_builder_append_char(builder, '\n');
    }

    string_builder_replace(builder, "..", ".");
    string_builder_replace(builder, ".", " ");

    string_builder_insert(builder, 0, "Hello, String Builder!\n");

    printf("%s", builder->string);

    string_builder_free(builder);
}

// OUTPUT:
// Hello, String Builder!
// Building a 10x10 field:
// | #  #  #  #  #  #  #  #  #  # |
// | #  #  #  #  #  #  #  #  #  # |
// | #  #  #  #  #  #  #  #  #  # |
// | #  #  #  #  #  #  #  #  #  # |
// | #  #  #  #  #  #  #  #  #  # |
// | #  #  #  #  #  #  #  #  #  # |
// | #  #  #  #  #  #  #  #  #  # |
// | #  #  #  #  #  #  #  #  #  # |
// | #  #  #  #  #  #  #  #  #  # |
// | #  #  #  #  #  #  #  #  #  # |
```

# Mentions

This library and it's functionality were inspired by [C#'s String Builder](https://learn.microsoft.com/en-us/dotnet/api/system.text.stringbuilder).

Some other string builders in C: 
- https://github.com/cavaliercoder/c-stringbuilder
- https://nachtimwald.com/2017/02/26/efficient-c-string-builder/
- https://codereview.stackexchange.com/questions/155286/stringbuilder-in-c
