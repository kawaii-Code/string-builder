// Copyright 2023 kawaii-Code.
// Subject to the MIT License.

#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

#ifndef STRING_BUILDER_NO_FORMAT
#include <stdio.h>
#endif // STRING_BUILDER_NO_FORMAT

#ifndef STRING_BUILDER_CUSTOM_MEMORY_MANAGEMENT
#include <stdlib.h>
#define STRING_BUILDER_MALLOC  malloc
#define STRING_BUILDER_REALLOC realloc
#define STRING_BUILDER_FREE    free
#endif // STRING_BUILDER_CUSTOM_MEMORY_MANAGEMENT

#ifndef STRING_BUILDER_ASSERT
#include <assert.h>
#define STRING_BUILDER_ASSERT assert
#endif // STRING_BUILDER_ASSERT

#ifndef STRING_BUILDER_RESIZE_FACTOR
#define STRING_BUILDER_RESIZE_FACTOR 2
#endif // STRING_BUILDER_RESIZE_FACTOR

#ifndef STRING_BUILDER_DEFAULT_CAPACITY
#define STRING_BUILDER_DEFAULT_CAPACITY 16
#endif // STRING_BUILDER_DEFAULT_CAPACITY

typedef struct {
    size_t length;
    size_t capacity;
    char  *string;
} StringBuilder;

StringBuilder string_builder_new();
StringBuilder string_builder_new_with_capacity(size_t capacity);
StringBuilder string_builder_new_from(const char *string);
void          string_builder_free(StringBuilder *builder);

void string_builder_ensure_capacity(StringBuilder *builder, size_t expected_length);
void string_builder_append(StringBuilder *builder, const char *append_string);
void string_builder_append_n(StringBuilder *builder, const char *append_string, size_t length);
void string_builder_append_char(StringBuilder *builder, char c);
void string_builder_append_int(StringBuilder *builder, int value);
void string_builder_append_bits(StringBuilder *builder, int64_t value, int bit_count);
void string_builder_append_format(StringBuilder *builder, const char *format, ...);
void string_builder_insert(StringBuilder *builder, size_t insert_index, const char *insertion);
void string_builder_replace(StringBuilder *builder, const char *string_to_replace, const char *replacement);

// Creates a new StringBuilder with length set to zero and
// capacity set to STRING_BUILDER_DEFAULT_CAPACITY.
// Allocates STRING_BUILDER_DEFAULT_CAPACITY bytes for the
// string and sets the string equal to the null terminator.
//
//
// Example:
//
// StringBuilder builder = string_builder_new();
// builder = StringBuilder{
//      length = 0,
//      capacity = STRING_BUILDER_DEFAULT_CAPACITY,
//      string = "\0",
// }
StringBuilder string_builder_new();

// Creates a new StringBuilder with length set to zero and
// capacity set to `capacity`.
// Allocates `capacity` bytes for the string and sets the 
// string equal to the null terminator.
//
//
// Example:
//
// string_builder_new_with_capacity(1024) -> StringBuilder{
//      length = 0,
//      capacity = 1024,
//      string = "\0",
// }
StringBuilder string_builder_new_with_capacity(size_t capacity);

//
// Creates a new StringBuilder with length set to strlen(`string`)
// and capacity set to strlen(`string`) + 1.
// strlen(`string`) + 1 bytes are allocated to hold the string.
// `string` is copied into the allocated memory.
//
//
// Example:
//
// StringBuilder builder = string_builder_new_from("Hello");
// builder = StringBuilder{
//      length = 5,
//      capacity = 6,
//      string = "Hello\0",
// }
StringBuilder string_builder_new_from(const char *string);

// Frees the memory allocated for the string and resets
// length and capacity. After calling string_builder_free(),
// the freed builder should not be used anymore.
//
//
// Example:
//
// StringBuilder builder = string_builder_new();
// string_builder_free(&builder);
// string_builder_append(&builder, "Hello"); /* DON'T DO THAT! */
void          string_builder_free(StringBuilder *builder);

// After calling string_builder_ensure_capacity(builder, expected_length), the builder
// is guaranteed to have enough memory allocated for string of length `expected_length`.
//
// The null terminator shouldn't be counted in `expected_length`.
//
//
// Example:
//
// StringBuilder builder = string_builder_new_with_capacity(3);
// const char *string = "Hello";
// /* The builder will now have enough memory to store "Hello". */
// string_builder_ensure_capacity(&builder, strlen(string));
void string_builder_ensure_capacity(StringBuilder *builder, size_t expected_length);

// Appends the `append_string` to the end of the string being built.
// Memory is allocated if needed.
//
//
// Example:
//
// StringBuilder builder = string_builder_new_with_capacity(4);
// string_builder_append(&builder, "hello");
// string_builder_append(&builder, " ");
// string_builder_append(&builder, "world");
//
// builder = StringBuilder{
//      length = 11,
//      capacity = ???, // Greater than length
//      string = "hello world\0",
// }
void string_builder_append(StringBuilder *builder, const char *append_string);

// Appends `length` characters from `append_string` to the end of the string
// being built. The resulting string is null terminated.
// Memory is allocated if needed.
//
//
// Example:
//
// StringBuilder builder = string_builder_new_with_capacity(1);
// string_builder_append_n(builder, "hello", 2);
// builder = StringBuilder{
//      length = 2,
//      capacity = ???, // Greater than length
//      string = "he\0",
// }
void string_builder_append_n(StringBuilder *builder, const char *append_string, size_t length);

// Appends a character `c` to the end of the built string.
// Memory is allocated if needed.
//
//
// Example:
//
// StringBuilder builder = string_builder_new();
// string_builder_append_char(builder, 'a');
// string_builder_append_char(builder, 'b');
// string_builder_append_char(builder, 'c');
// string_builder_append_char(builder, 'd');
// string_builder_append_char(builder, 'e');
// builder = StringBuilder{
//      length = 5,
//      capacity = ???, // Greater than length
//      string = "abcde\0",
// }
void string_builder_append_char(StringBuilder *builder, char c);

// Appends an integer to the end of the string being built.
// The integer is converted to it's string representation.
// No memory is allocated for integer to string conversion.
//
//
// Example:
//
// StringBuilder builder = string_builder_new();
// string_builder_append_int(builder, 123);
// builder = StringBuilder{
//      length = 3,
//      capacity = ???, // Greater than length
//      string = "123\0",
// }
void string_builder_append_int(StringBuilder *builder, int value);

// Appends a bit representation of an integer to the end of the string
// being built. Only `bit_count` bits are appended, starting from the
// low-order byte. The string representation of bits is not reversed.
// Memory is not allocated for the bit to string conversion, but
// allocated for the string being built if needed.
//
//
// Examples:
//
// StringBuilder builder = string_builder_new();
// string_builder_append_bits(builder, 49, 8);
// builder = StringBuilder{
//      length = 8,
//      capacity = ???, // Greater than length
//      string = "00110001\0",
// }
void string_builder_append_bits(StringBuilder *builder, int64_t value, int bit_count);

// Appends a formatted string to the end of the string being built.
// The formats are the same formats that are supported in `printf``
// No memory is allocated for integer to string conversion.
//
//
// Example:
//
// StringBuilder builder = string_builder_new();
// string_builder_append_format(builder, "%d * %.2f = %s", 5, 5.4, "27.0");
// builder = StringBuilder{
//      length = 15,
//      capacity = ???, // Greater than length
//      string = "5 * 5.40 = 27.0\0",
// }
void string_builder_append_format(StringBuilder *builder, const char *format, ...);

// Inserts `insertion` at `insert_index` in the string being built.
// All the string after `insert_index` is moved to accomodate the inserted
// string.
//
// Memory is allocated if needed for the string being built.
//
//
// Example:
//
// StringBuilder builder = string_builder_new();
// string_builder_append(&builder, "world")
// string_builder_insert(&builder, 0, "hello");
// string_builder_insert(&builder, 5, " ");
// builder = StringBuilder{
//      length = 11,
//      capacity = ???, // Greater than length
//      string = "hello world\0",
// }
void string_builder_insert(StringBuilder *builder, size_t insert_index, const char *insertion);

// Replaces all entries of `string_to_replace` in the string being built with
// `replacement`.
// No memory is allocated for any temporary strings / replacement needs, but
// memory is allocated if needed for the string being built.
//
//
// Example:
//
// StringBuilder builder = string_builder_new();
// string_builder_append(&builder, "aabbccaaaa");
// string_builder_replace(&builder, "aa", "d");
// string_builder_replace(&builder, "b", "ee");
// builder = StringBuilder{
//      length = 9,
//      capacity = ???, // Greater than length
//      string = "deeeeccdd",
// }
void string_builder_replace(StringBuilder *builder, const char *string_to_replace, const char *replacement);

#ifdef STRING_BUILDER_IMPLEMENTATION

StringBuilder string_builder_new() {
    return string_builder_new_with_capacity(STRING_BUILDER_DEFAULT_CAPACITY);
}

StringBuilder string_builder_new_with_capacity(size_t capacity) {
    STRING_BUILDER_ASSERT(capacity > 0);

    char *inner = STRING_BUILDER_MALLOC(capacity * sizeof *inner);
    *inner = '\0';

    StringBuilder builder;
    builder.length = 0;
    builder.capacity = capacity;
    builder.string = inner;
    return builder;
}

StringBuilder string_builder_new_from(const char *string) {
    size_t length = strlen(string);
    size_t capacity = length + 1;
    char *copy = STRING_BUILDER_MALLOC(capacity * sizeof *copy);
    memcpy(copy, string, capacity);

    StringBuilder builder;
    builder.length = length;
    builder.capacity = capacity;
    builder.string = copy;
    return builder;
}

void string_builder_free(StringBuilder *builder) {
    builder->length = 0;
    builder->capacity = 0;
    STRING_BUILDER_FREE(builder->string);
}

void string_builder_ensure_capacity(StringBuilder *builder, size_t expected_length) {
    size_t new_capacity = builder->capacity;
    while (expected_length >= new_capacity) {
        new_capacity *= STRING_BUILDER_RESIZE_FACTOR;
    }

    char *new_string = STRING_BUILDER_REALLOC(builder->string, new_capacity);

    builder->capacity = new_capacity;
    builder->string = new_string;
}

void string_builder_append(StringBuilder *builder, const char *string) {
    string_builder_append_n(builder, string, strlen(string));
}

void string_builder_append_n(StringBuilder *builder, const char *string, size_t length) {
    size_t old_length = builder->length;
    size_t new_length = old_length + length;
    string_builder_ensure_capacity(builder, new_length);

    char *inner_end = builder->string + old_length;
    memcpy(inner_end, string, length + 1);

    builder->string[new_length] = '\0';
    builder->length = new_length;
}

void string_builder_append_reversed(StringBuilder *builder, const char *string, size_t length) {
    size_t old_length = builder->length;
    size_t new_length = old_length + length;
    string_builder_ensure_capacity(builder, new_length);

    char *inner_end = builder->string + old_length;
    const char *append_end = string + length - 1;
    while (length--) {
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

    char *inner = builder->string;
    inner[old_length] = c;
    inner[new_length] = '\0';
    builder->length = new_length;
}

// -2147483648
#define STRING_BUILDER_MAX_CHARS_IN_INT 11
void string_builder_append_int(StringBuilder *builder, int value) {
    char chars[STRING_BUILDER_MAX_CHARS_IN_INT];
    int i = 0;

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

    string_builder_append_reversed(builder, chars, i);
}

#ifndef STRING_BUILDER_NO_FORMAT
void string_builder_append_format(StringBuilder *builder, const char *format, ...) {
    va_list arg_list;
    size_t old_length = builder->length;

    va_start(arg_list, format);
    size_t appended_length = vsnprintf(NULL, 0, format, arg_list);
    size_t new_length = old_length + appended_length;
    string_builder_ensure_capacity(builder, new_length);
    va_end(arg_list);

    char *string_end = builder->string + old_length;
    va_start(arg_list, format);
    vsnprintf(string_end, appended_length + 1, format, arg_list);
    va_end(arg_list);

    builder->length = new_length;
}
#endif // STRING_BUILDER_NO_FORMAT

void string_builder_append_bits(StringBuilder *builder, int64_t value, int bit_count) {
    STRING_BUILDER_ASSERT((bit_count > 0) && (bit_count <= 64));

    int64_t mask = (int64_t)1 << (bit_count - 1);

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
    STRING_BUILDER_ASSERT(insert_index <= old_length);
    size_t inserted_length = strlen(inserted_string);
    size_t new_length = old_length + inserted_length;
    string_builder_ensure_capacity(builder, new_length);

    char *insert_at = builder->string + insert_index;
    char *after_insert = insert_at + inserted_length;
    memmove(after_insert, insert_at, old_length - insert_index + 1);
    memcpy(insert_at, inserted_string, inserted_length);

    builder->length = new_length;
}

int string_builder_count_substrings(StringBuilder *builder, const char *substring) {
    const char *const substring_first_character = substring;
    const char *inner = builder->string;
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

    char *inner = builder->string;
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

#endif // STRING_BUILDER_IMPLEMENTATION

#endif // STRING_BUILDER_H
