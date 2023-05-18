#include <stdio.h>
#include <stddef.h>

#define STRING_BUILDER_IMPLEMENTATION
#include "string_builder.h"

typedef struct StringBuilder StringBuilder_t;

int main() {
  StringBuilder *builder = string_builder_new();

  string_builder_ensure_capacity(builder, 10 * 10 * 10);

  for (int y = 0; y < 10; y++) {
    for (int x = 0; x < 10; x++) {
      string_builder_append(builder, " # ");
    }
    string_builder_append_char(builder, '\n');
  }

  string_builder_replace(builder, " ", ".");
  string_builder_replace(builder, "..", ".");

  string_builder_insert(builder, 0, "Field:\n");

  printf("%s", builder->inner);

  string_builder_free(builder);
}
