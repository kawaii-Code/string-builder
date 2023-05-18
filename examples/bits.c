#include <stdio.h>

#define STRING_BUILDER_IMPLEMENTATION
#include "../string_builder.h"

int main() {
    StringBuilder *builder = string_builder_new();
    string_builder_append_bits(builder, 2147483647, 32);
    printf("s = %s", builder->inner);
    string_builder_free(builder);

    printf("\n");

    builder = string_builder_new();
    string_builder_append_bits(builder, 32, 8);
    printf("s = %s", builder->inner);
    string_builder_free(builder);
}
