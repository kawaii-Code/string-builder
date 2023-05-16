#include <stdio.h>
#include <stddef.h>

#include "string_builder.h"

int main() {
  StringBuilder *builder = string_builder_new_from("Hello, World!\n");

  string_builder_append_format(builder, "%d, %d, %d\n", 32, 0, -1);
  string_builder_insert(builder, 1, "abc");

  string_builder_append_bits(builder, 32);
  string_builder_append_char(builder, '\n');
  string_builder_append_bits(builder, -2147483648);
  string_builder_append_char(builder, '\n');
  string_builder_append_bits(builder, 2147483647);
  string_builder_append_char(builder, '\n');

  string_builder_append_format(builder, "%d x %d %s:\n", 10, 10, "field");

  for (int x = 0; x < 10; x++) {
    for (int y = 0; y < 10; y++) {
      string_builder_append(builder, " * ");
    }
    string_builder_append_char(builder, '\n');
  }

  const char *result = string_builder_build(builder);
  printf("%s", result);

  string_builder_free(builder);
}
