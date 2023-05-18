#include <stdio.h>

#define STRING_BUILDER_IMPLEMENTATION
#include "../string_builder.h"

#define FIELD_SIZE 10

int main() {
  StringBuilder *builder = string_builder_new();

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

  printf("%s", builder->inner);

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
