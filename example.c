#include <stdio.h>
#include <stddef.h>

#include "string_builder.h"

int main() {
  StringBuilder *builder = string_builder_new();

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
