#include <stdio.h>

#define STRING_BUILDER_IMPLEMENTATION
#include "../string_builder.h"

int main() {
    StringBuilder b = string_builder_new();
    StringBuilder *builder = &b;

    string_builder_append_n(builder, "Hello, World!", 5);

    printf("%s\n", builder->string); // "Hello"

    string_builder_free(builder);
}
