#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define RESIZE_FACTOR    2
#define DEFAULT_CAPACITY 16

typedef struct {
  size_t length;
  size_t capacity;
  char *inner;
} StringBuilder_implementation;

typedef StringBuilder_implementation StringBuilder;

StringBuilder *string_builder_new() {
  size_t capacity = DEFAULT_CAPACITY;
  char *string = malloc(sizeof *string * capacity);
  *string = '\0';
  
  StringBuilder *builder = malloc(sizeof *builder);
  builder->length = 0;
  builder->capacity = capacity;
  builder->inner = string;

  return builder;
}

StringBuilder *string_builder_new_with_capacity(int capacity) {
  char *string = malloc(sizeof *string * capacity);
  *string = '\0';

  StringBuilder *builder = malloc(sizeof *builder);
  builder->length = 0;
  builder->capacity = capacity;
  builder->inner = string;

  return builder;
}

// TODO: Add constructor from string

void string_builder_free(StringBuilder *builder) {
  builder->length = 0;
  builder->capacity = 0;
  free(builder->inner);
  free(builder);
}

char *string_builder_build(StringBuilder *builder) {
  return builder->inner;
}

void string_builder_resize(StringBuilder *builder) {
  char *string = builder->inner;
  size_t new_capacity = RESIZE_FACTOR * builder->capacity;

  char *new_string = realloc(string, new_capacity);

  builder->capacity = new_capacity;
  builder->inner = new_string;
}

void string_builder_append(StringBuilder *builder, const char *append_string) {
  size_t old_length = builder->length;
  size_t capacity = builder->capacity;
  char *string = builder->inner;

  size_t add_length = strlen(append_string);
  size_t new_length = old_length + add_length;

  if (new_length >= capacity) {
    string_builder_resize(builder);
    string = builder->inner;
  }

  char *string_iterator = string + old_length;
  while (*append_string != '\0') {
    *string_iterator = *append_string;

    string_iterator++;
    append_string++;
  }
  *string_iterator = '\0';

  builder->length = new_length;
}

void string_builder_append_reversed(StringBuilder *builder, const char *append_string, size_t append_length) {
  size_t old_length = builder->length;
  size_t capacity = builder->capacity;
  char *string = builder->inner;

  size_t new_length = old_length + append_length;

  if (new_length >= capacity) {
    string_builder_resize(builder);
    string = builder->inner;
  }

  char *string_iterator = string + old_length;
  const char *append_iterator = append_string + append_length;
  while (*append_iterator != '\0') {
    *string_iterator = *append_iterator;

    string_iterator++;
    append_iterator--;
  }
  *string_iterator = '\0';

  builder->length = new_length;
}

void string_builder_append_char(StringBuilder *builder, char c) {
  size_t old_length = builder->length;
  size_t capacity = builder->capacity;
  char *string = builder->inner;

  size_t new_length = old_length + 1;

  if (new_length >= capacity) {
    string_builder_resize(builder);
    string = builder->inner;
  }

  string[old_length] = c;
  string[new_length] = '\0';
  builder->length = new_length;
}

// -2147483648
#define MAX_CHARS_IN_INT 11

void string_builder_append_int(StringBuilder *builder, int value) {
  char chars[MAX_CHARS_IN_INT + 1];
  int i = 1;

  if (value < 0) {
    value = -value;

    while (value > 0) {
      chars[i++] = '0' + value % 10;
      value /= 10;
    }

    chars[i++] = '-';
  } else {
    while (value > 0) {
      chars[i++] = '0' + value % 10;
      value /= 10;
    }
  }
  chars[0] = '\0';

  string_builder_append_reversed(builder, chars, i - 1);
}

void string_builder_append_format(StringBuilder *builder, const char *format, ...) {
  va_list arg_list;
  va_start(arg_list, format);

  while (*format != '\0') {
    if (*format == '%') {
      format++;
      if (*format == 'd') {
        int value = va_arg(arg_list, int);
        string_builder_append_int(builder, value);
      } else if (*format == 's') {
        char *value = va_arg(arg_list, char*);
        string_builder_append(builder, value);
      } else if (*format == 'c') {
        char value = va_arg(arg_list, int);
        string_builder_append_char(builder, value);
      }
    }
    else {
      // Slow
      string_builder_append_char(builder, *format);
    }

    format++;
  }

  va_end(arg_list);
}
