#include <stdio.h>

#define STRING_BUILDER_IMPLEMENTATION
#include "../string_builder.h"

int main() {
    StringBuilder b = string_builder_new_from("Hello, World!");
    StringBuilder *builder = &b;

    printf("'%s'\n", builder->string); // 'Hello, World!'

    string_builder_replace(builder, "l", "ww");
    printf("'%s'\n", builder->string);  // 'Hewwwwo, Worwwd!'

    string_builder_replace(builder, "!", "~~~"); 
    printf("'%s'\n", builder->string); // 'Hewwwwo, Worwwd~~~'

    string_builder_replace(builder, "wwww", "l");
    printf("'%s'\n", builder->string); // 'Helo, Worwwd~~~'

    string_builder_replace(builder, ",", ";");
    printf("'%s'\n", builder->string); // 'Helo; Worwwd~~~'

    string_builder_free(builder);
}
