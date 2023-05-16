typedef struct StringBuilder_implementation StringBuilder;

StringBuilder *string_builder_new();
StringBuilder *string_builder_new_with_capacity(size_t capacity);
StringBuilder *string_builder_new_from(const char *string);
void string_builder_free(StringBuilder *builder);

size_t string_builder_get_length(StringBuilder *builder);
size_t string_builder_get_capacity(StringBuilder *builder);
char *string_builder_build(StringBuilder *builder);

void string_builder_append(StringBuilder *builder, const char *append_string);
void string_builder_append_char(StringBuilder *builder, char c);
void string_builder_append_int(StringBuilder *builder, int value);
void string_builder_append_format(StringBuilder *builder, const char *format_string, ...);
