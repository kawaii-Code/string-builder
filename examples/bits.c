#include <stdio.h>

#define STRING_BUILDER_IMPLEMENTATION
#include "../string_builder.h"

int main() {
    StringBuilder b = string_builder_new();
    StringBuilder *builder = &b;

    string_builder_append_bits(builder, 2147483647, 32);
    printf("%s\n", builder->string); // 01111111_11111111_11111111_11111111
    string_builder_free(builder);

    b = string_builder_new();
    builder = &b;
    string_builder_append_bits(builder, 32, 8); // 00100000
    printf("%s\n", builder->string);
    string_builder_free(builder);
}
