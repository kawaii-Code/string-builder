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
  char *inner = malloc(sizeof *inner * capacity);
  *inner = '\0';

  StringBuilder *builder = malloc(sizeof *builder);
  builder->length = 0;
  builder->capacity = capacity;
  builder->inner = inner;
  return builder;
}

StringBuilder *string_builder_new() {
  return string_builder_new_with_capacity(DEFAULT_CAPACITY);
}

StringBuilder *string_builder_new_from(const char *string) {
  size_t length = strlen(string);
  size_t capacity = length;
  char *copy = malloc((length + 1) * sizeof *copy);
  strcpy(copy, string);

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
  size_t new_capacity = RESIZE_FACTOR * builder->capacity;
  char *new_string = realloc(builder->inner, new_capacity);

  builder->capacity = new_capacity;
  builder->inner = new_string;
  return new_string;
}

void string_builder_ensure_capacity(StringBuilder *builder, size_t expected_length) {
  if (expected_length >= builder->capacity) {
    string_builder_resize(builder);
  } 
}

void string_builder_append(StringBuilder *builder, const char *string) {
  size_t old_length = builder->length;
  size_t new_length = old_length + strlen(string);
  string_builder_ensure_capacity(builder, new_length);

  char *inner_end = builder->inner + old_length;
  strcpy(inner_end, string);

  builder->length = new_length;
}

void string_builder_append_n(StringBuilder *builder, const char *string, size_t length) {
  size_t old_length = builder->length;
  size_t new_length = old_length + length;
  string_builder_ensure_capacity(builder, new_length);

  char *inner_end = builder->inner + old_length;
  strncpy(inner_end, string, length);
  
  *(builder->inner + new_length) = '\0';
  builder->length = new_length;
}

void string_builder_append_reversed(StringBuilder *builder, const char *string, size_t append_length) {
  size_t old_length = builder->length;
  size_t new_length = old_length + append_length;
  string_builder_ensure_capacity(builder, new_length);

  char *inner_end = builder->inner + old_length;
  const char *append_end = string + append_length;
  while (*append_end != '\0') {
    *inner_end = *append_end;
    inner_end++;
    append_end--;
  }
  *inner_end = '\0';

  builder->length = new_length;
}

void string_builder_append_char(StringBuilder *builder, char c) {
  size_t old_length = builder->length;
  size_t new_length = old_length + 1;
  string_builder_ensure_capacity(builder, new_length);

  char *inner = builder->inner;
  inner[old_length] = c;
  inner[new_length] = '\0';
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

// This is just a bad function
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
  size_t inserted_length = strlen(inserted_string);
  size_t new_length = old_length + inserted_length;
  string_builder_ensure_capacity(builder, new_length);

  char *inner = builder->inner;
  char *inner_new_end = inner + new_length;
  char *inner_reversed = inner + old_length;
  const char *const last_inserted_character = inner + insert_index + inserted_length - 1;
  while (inner_new_end > last_inserted_character) {
    *inner_new_end = *inner_reversed;
    inner_reversed--;
    inner_new_end--;
  }

  char *insert = inner + insert_index;
  while (*inserted_string != '\0') {
    *insert = *inserted_string;
    inserted_string++;
    insert++;
  }

  builder->length = new_length;
}

int string_builder_count_substrings(StringBuilder *builder, const char *substring) {
  const char *const substring_first_character = substring;
  const char *inner = builder->inner;
  int substring_count = 0;

  while (*inner != '\0') {
    if (*inner == *substring) {
      const char *const start_check_from = inner;
      while (*substring != '\0') {
        substring++;
        inner++;
        if (*inner != *substring) {
          break;
        }
      }

      if (*substring == '\0') {
        substring_count++;
      } else {
        inner = start_check_from + 1;
      }

      substring = substring_first_character;
    } else {
      inner++;
    }
  }

  return substring_count;
}

void string_builder_replace(StringBuilder *builder, const char *old, const char *new) {
  size_t length = builder->length;
  size_t old_substring_length = strlen(old);
  size_t new_substring_length = strlen(new);
  size_t new_length = length + (new_substring_length - old_substring_length) * string_builder_count_substrings(builder, old);
  string_builder_ensure_capacity(builder, new_length);

  // I don't want to allocate any memory in the function.
  // To do that, all the copying and replacing has to be done
  // in the same memory where the initial string is located.
  // For that I need to handle two different scenarios:
  //
  // When the old substring is bigger than the new one, we
  // can iterate the string from the beginning and
  // insert the new substrings as we go: this won't overwrite
  // any other string data because the new substring is smaller.
  //
  // In the other case, we must go from the end, otherwise, when
  // replacing the old substring, we would overwrite the symbols
  // that aren't in the old substring and ruin the entire string.
  //
  // A small example demonstrating the problem:
  // ```
  // string: abc
  // replace: a -> aa
  // result: aac
  // ```
  // If we aren't going from the end, we will end up with
  // 'a' being replaced by 'aa' with the second 'a' overwriting 'b'.

  char *inner = builder->inner;
  if (old_substring_length >= new_substring_length) {
    char *copy_iterator = inner;
    const char *const old_start = old;
    const char *const new_start = new;
    const char *const inner_new_end = inner + new_length;

    while (copy_iterator < inner_new_end) {
      if (*inner == *old) {
        char *const inner_start = inner;
        while (*old != '\0') {
          if (*old != *inner) {
            break;
          }
          inner++;
          old++;
        }

        if (*old == '\0') {
          while (*new != '\0') {
            *copy_iterator = *new;
            copy_iterator++;
            new++;
          }
          new = new_start;
        } else {
          inner = inner_start + 1;
          *copy_iterator = *inner_start;
          copy_iterator++;
        }
        old = old_start;
      } else {
        *copy_iterator = *inner;
        copy_iterator++;
        inner++;
      }
    }
    *copy_iterator = '\0';
  } else {
    const char *const old_end = old + old_substring_length - 1;
    const char *const new_end = new + new_substring_length - 1;
    const char *reverse_inner = inner + length - 1;
    char *copy_iterator = inner + new_length;
    *copy_iterator = '\0';
    copy_iterator--;

    for (size_t i = 0; i < length; i++) {
      if (*reverse_inner == *old_end) {
        const char *old_reversed = old_end;
        const char *const begin_from = reverse_inner;
        size_t j = old_substring_length;
        while (j) {
          if (*reverse_inner != *old_reversed) {
            break;
          }
          reverse_inner--;
          old_reversed--;
          j--;
        }

        if (j == 0) {
          const char *new_reversed = new_end;
          j = new_substring_length;
          while (j--) {
            *copy_iterator = *new_reversed;
            new_reversed--;
            copy_iterator--;
          }
        } else {
          reverse_inner = begin_from - 1;
          *copy_iterator = *begin_from;
          copy_iterator--;
        }
      } else {
        *copy_iterator = *reverse_inner;
        reverse_inner--;
        copy_iterator--;
      }
    }
  }

  builder->length = new_length;
}
