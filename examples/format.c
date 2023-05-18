#include <stdio.h>

#define STRING_BUILDER_IMPLEMENTATION
#include "../string_builder.h"

int main() {
    StringBuilder *builder = string_builder_new();

    string_builder_append_format(builder, "%d + %d = %d, ", 1, 2, 1 + 2);
    string_builder_append_format(builder, "%c + %c = %s", 'a', 'b', "ab");

    printf("'%s'", builder->inner); // '1 + 2 = 3, a + b = ab'

    string_builder_free(builder);
}
