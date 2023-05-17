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

StringBuilder *string_builder_new_with_capacity(size_t capacity) {
  char *string = malloc(sizeof *string * capacity);
  *string = '\0';

  StringBuilder *builder = malloc(sizeof *builder);
  builder->length = 0;
  builder->capacity = capacity;
  builder->inner = string;

  return builder;
}

StringBuilder *string_builder_new() {
  return string_builder_new_with_capacity(DEFAULT_CAPACITY);
}

StringBuilder *string_builder_new_from(const char *string) {
  size_t length = strlen(string);
  size_t capacity = length;
  char *copy = malloc((length + 1) * sizeof *copy);

  char *iterator = copy;
  while (*string != '\0') {
    *iterator = *string;

    string++;
    iterator++;
  }
  *iterator = '\0';

  StringBuilder *builder = malloc(sizeof *builder);
  builder->length = length;
  builder->capacity = capacity;
  builder->inner = copy;

  return builder;
}

void string_builder_free(StringBuilder *builder) {
  builder->length = 0;
  builder->capacity = 0;
  free(builder->inner);
  free(builder);
}

size_t string_builder_get_length(StringBuilder *builder) {
  return builder->length;
}

size_t string_builder_get_capacity(StringBuilder *builder) {
  return builder->capacity;
}

char *string_builder_build(StringBuilder *builder) {
  return builder->inner;
}

char *string_builder_resize(StringBuilder *builder) {
  char *string = builder->inner;
  size_t new_capacity = RESIZE_FACTOR * builder->capacity;

  char *new_string = realloc(string, new_capacity);

  builder->capacity = new_capacity;
  builder->inner = new_string;
  return new_string;
}

void string_builder_append(StringBuilder *builder, const char *append_string) {
  size_t old_length = builder->length;
  size_t capacity = builder->capacity;
  char *string = builder->inner;

  size_t add_length = strlen(append_string);
  size_t new_length = old_length + add_length;

  if (new_length >= capacity) {
    string = string_builder_resize(builder);
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
    string = string_builder_resize(builder);
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
    string = string_builder_resize(builder);
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

  if (value == 0) {
    string_builder_append_char(builder, '0');
    return;
  }
  else if (value < 0) {
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
      } else if (*format == '%') {
        string_builder_append_char(builder, '%');
      } else if (*format == 'f') {
        // double value = va_arg(arg_list, double);
        // string_builder_append_double(builder, value);
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

void string_builder_append_bits(StringBuilder *builder, int value) {
  int bit_count = 31;
  int mask = 1 << bit_count;

  while (bit_count--) {
    if (value & mask) {
      string_builder_append_char(builder, '1');
    } else {
      string_builder_append_char(builder, '0');
    }
    mask >>= 1;
  }
}

void string_builder_insert(StringBuilder *builder, size_t insert_index, const char *inserted_string) {
  size_t old_length = builder->length;
  size_t capacity = builder->capacity;
  char *string = builder->inner;

  size_t inserted_length = strlen(inserted_string);
  size_t new_length = old_length + inserted_length;

  if (new_length >= capacity) {
    string = string_builder_resize(builder);
  }

  char *copy_iterator = string + new_length;
  char *string_reversed_iterator = string + old_length;
  char *after_inserted = string + insert_index + inserted_length;
  while (copy_iterator >= after_inserted) {
    *copy_iterator = *string_reversed_iterator;

    string_reversed_iterator--;
    copy_iterator--;
  }

  char *insert_iterator = string + insert_index;
  while (*inserted_string != '\0') {
    *insert_iterator = *inserted_string;

    inserted_string++;
    insert_iterator++;
  }

  builder->length = new_length;
  builder->inner = string;
}

int string_builder_count_substrings(StringBuilder *builder, const char *substring) {
  const char *substring_begin = substring;
  const char *string = builder->inner;

  int result = 0;

  while (*string != '\0') {
    if (*string == *substring) {
      while (*substring != '\0') {
        substring++;
        string++;

        if (*string != *substring) {
          break;
        }
      }
      string--;

      if (*substring == '\0') {
        result++;
      }

      substring = substring_begin;
    }
    string++;
  }

  return result;
}

#include <stdio.h>

void string_builder_replace(StringBuilder *builder, const char *old, const char *new) {
  // TODO: Fix naming in this function (and the entire module)
  size_t length = builder->length;
  char *string = builder->inner;

  size_t old_string_length = strlen(old);
  size_t new_string_length = strlen(new);

  int substring_count = string_builder_count_substrings(builder, old);

  size_t new_length = length + (new_string_length - old_string_length) * substring_count;
  if (new_length >= builder->capacity) {
    string = string_builder_resize(builder);
  }

  // I can make two separate cases: for less and greater!
  if (old_string_length >= new_string_length) {
    char *copy_iterator = string;
    const char *const old_start = old;
    const char *const new_start = new;

    for (size_t i = 0; i < new_length; i++) {
      if (*string == *old) {
        char *const string_start = string;

        // Is this a substring?
        while (*old != '\0') {
          if (*old != *string) {
            break;
          }
          string++;
          old++;
        }

        // Is this a substring?
        if (*old == '\0') {
          // Change to strcpy
          while (*new != '\0') {
            *copy_iterator = *new;
            copy_iterator++;
            new++;
          }
        } else {
          // Not a substring, we need to copy everything that we read while checking for the substring.
          string = string_start;
        }
        old = old_start;
        new = new_start;
      }

      if (*string == '\0') {
        break;
      }

      *copy_iterator = *string;
      copy_iterator++;
      string++;
    }

    *copy_iterator = '\0';
  } else {
    const char *const old_end = old + old_string_length - 1;
    const char *const new_end = new + new_string_length - 1;
    const char *string_reverse_iterator = string + length - 1;
    char *copy_iterator = string + new_length;
    *copy_iterator = '\0';
    copy_iterator--;

    for (size_t i = 0; i < length; i++) {
      if (*string_reverse_iterator == *old_end) {
        const char *old_reversed = old_end;
        const char *const begin_from = string_reverse_iterator;
        size_t j = old_string_length;
        while (j) {
          if (*string_reverse_iterator != *old_reversed) {
            break;
          }
          string_reverse_iterator--;
          old_reversed--;
          j--;
        }

        if (j == 0) {
          const char *new_reversed = new_end;
          j = new_string_length;
          while (j--) {
            *copy_iterator = *new_reversed;
            new_reversed--;
            copy_iterator--;
          }
        } else {
          string_reverse_iterator = begin_from;
        }
      }

      *copy_iterator = *string_reverse_iterator;
      string_reverse_iterator--;
      copy_iterator--;
    }
  }

  builder->length = new_length;
}
