#include <stdio.h>

#define STRING_BUILDER_IMPLEMENTATION
#include "../string_builder.h"

int main() {
    StringBuilder b = string_builder_new();
    StringBuilder *builder = &b;

    string_builder_insert(builder, 0, "World?");
    string_builder_insert(builder, 0, "Hello");
    string_builder_insert(builder, 5, ", ");

    printf("'%s'", builder->inner); // 'Hello, World?'
}
