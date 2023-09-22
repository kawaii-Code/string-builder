#include <stdio.h>

#define STRING_BUILDER_IMPLEMENTATION
#include "../string_builder.h"

int main() {
    StringBuilder b = string_builder_new();
    StringBuilder *builder = &b;

    // Important: append_n should be used as a faster alternative
    // to append, when the string length is known. Using this
    // function to append less symbols than needed will lead to bugs,
    // as it doesn't copy the null byte.
    string_builder_append_n(builder, "Hello, World!", 5);

    printf("'%s'", builder->string); // 'Hello,' !!!

    string_builder_free(builder);
}
