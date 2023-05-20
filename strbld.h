// Copyright 2023 kawaii-Code.
// Subject to the MIT License.

#ifndef STRBLD_H
#define STRBLD_H

#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

#ifndef STRBLD_CUSTOM_ALLOC
#include <stdlib.h>
#define STRBLD_MALLOC malloc
#define STRBLD_REALLOC realloc
#endif // STRBLD_CUSTOM_ALLOC

#ifndef STRBLD_ASSERT
#include <assert.h>
#define STRBLD_ASSERT assert
#endif

#ifndef SRTBLD_RESIZECOEF
#define SRTBLD_RESIZECOEF 2
#endif // SRTBLD_RESIZECOEF

#ifndef SRTBLD_DEFAULTCAP
#define SRTBLD_DEFAULTCAP 16
#endif // SRTBLD_DEFAULTCAP

typedef struct {
    size_t len;
    size_t cap;
    char *str;
} Strbld;

Strbld *strbld();
Strbld *strbld_withcap(size_t cap);
Strbld *strbld_from(const char *str);
void strbld_free(Strbld *bld);

void strbld_ensure(Strbld *bld, size_t maxlen);

void strbld_add(Strbld *bld, const char *str);
void strbld_addn(Strbld *bld, const char *str, size_t strlen);
void strbld_addc(Strbld *bld, char c);
void strbld_addi(Strbld *bld, int i);
void strbld_addf(Strbld *bld, const char *format, ...);
void strbld_addbits(Strbld *bld, int64_t i, int bitcnt);
void strbld_insert(Strbld *bld, size_t at, const char *str);
void strbld_repl(Strbld *bld, const char *old, const char *repl);

#ifdef STRBLD_IMPLEMENTATION

Strbld *strbld() {
    return strbld_withcap(SRTBLD_DEFAULTCAP);
}

Strbld *strbld_withcap(size_t capacity) {
    STRBLD_ASSERT(capacity > 0);

    char *inner = STRBLD_MALLOC(sizeof *inner * capacity);
    *inner = '\0';

    Strbld *builder = STRBLD_MALLOC(sizeof *builder);
    builder->len = 0;
    builder->cap = capacity;
    builder->str = inner;
    return builder;
}

Strbld *strbld_from(const char *string) {
    size_t length = strlen(string);
    size_t capacity = length + 1;
    char *copy = STRBLD_MALLOC(capacity * sizeof *copy);
    memcpy(copy, string, capacity);

    Strbld *builder = STRBLD_MALLOC(sizeof *builder);
    builder->len = length;
    builder->cap = capacity;
    builder->str = copy;
    return builder;
}

void strbld_free(Strbld *builder) {
    builder->len = 0;
    builder->cap = 0;
    free(builder->str);
    free(builder);
}

void strbld_ensure(Strbld *builder, size_t expected_length) {
    size_t new_capacity = builder->cap;
    while (expected_length >= new_capacity) {
        new_capacity *= SRTBLD_RESIZECOEF;
    }

    char *new_string = STRBLD_REALLOC(builder->str, new_capacity);

    builder->cap = new_capacity;
    builder->str = new_string;
}

void strbld_add(Strbld *builder, const char *string) {
    strbld_addn(builder, string, strlen(string));
}

void strbld_addn(Strbld *builder, const char *string, size_t length) {
    size_t old_length = builder->len;
    size_t new_length = old_length + length;
    strbld_ensure(builder, new_length);

    char *inner_end = builder->str + old_length;
    memcpy(inner_end, string, length + 1);

    builder->len = new_length;
}

void strbld_addrev(Strbld *builder, const char *string, size_t length) {
    size_t old_length = builder->len;
    size_t new_length = old_length + length;
    strbld_ensure(builder, new_length);

    char *inner_end = builder->str + old_length;
    const char *append_end = string + length - 1;
    while (length--) {
        *inner_end = *append_end;
        inner_end++;
        append_end--;
    }
    *inner_end = '\0';

    builder->len = new_length;
}

void strbld_addc(Strbld *builder, char c) {
    size_t old_length = builder->len;
    size_t new_length = old_length + 1;
    strbld_ensure(builder, new_length);

    char *inner = builder->str;
    inner[old_length] = c;
    inner[new_length] = '\0';
    builder->len = new_length;
}

// -2147483648
#define STRING_BUILDER_MAX_CHARS_IN_INT 11
void strbld_addi(Strbld *builder, int value) {
    char chars[STRING_BUILDER_MAX_CHARS_IN_INT];
    int i = 0;

    if (value == 0) {
        strbld_addc(builder, '0');
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

    strbld_addrev(builder, chars, i);
}

void strbld_addf(Strbld *builder, const char *format, ...) {
    va_list arg_list;
    va_start(arg_list, format);

    while (*format != '\0') {
        if (*format == '%') {
            format++;
            if (*format == 'd') {
                int value = va_arg(arg_list, int);
                strbld_addi(builder, value);
            } else if (*format == 's') {
                char *value = va_arg(arg_list, char*);
                strbld_add(builder, value);
            } else if (*format == 'c') {
                char value = va_arg(arg_list, int);
                strbld_addc(builder, value);
            } else if (*format == '%') {
                strbld_addc(builder, '%');
            }
            // TODO: Add support for more formats
        }
        else {
            // Slow
            strbld_addc(builder, *format);
        }

        format++;
    }

    va_end(arg_list);
}

void strbld_addbits(Strbld *builder, int64_t value, int bit_count) {
    STRBLD_ASSERT((bit_count > 0) && (bit_count <= 64));

    int64_t mask = (int64_t)1 << (bit_count - 1);

    while (bit_count--) {
        if (value & mask) {
            strbld_addc(builder, '1');
        } else {
            strbld_addc(builder, '0');
        }
        mask >>= 1;
    }
}

void strbld_insert(Strbld *builder, size_t insert_index, const char *inserted_string) {
    size_t old_length = builder->len;
    STRBLD_ASSERT(insert_index <= old_length);
    size_t inserted_length = strlen(inserted_string);
    size_t new_length = old_length + inserted_length;
    strbld_ensure(builder, new_length);

    char *insert_at = builder->str + insert_index;
    char *after_insert = insert_at + inserted_length;
    memmove(after_insert, insert_at, old_length - insert_index + 1);
    memcpy(insert_at, inserted_string, inserted_length);

    builder->len = new_length;
}

int strbld_count_substrings(Strbld *builder, const char *substring) {
    const char *const substring_first_character = substring;
    const char *inner = builder->str;
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

void strbld_repl(Strbld *builder, const char *old, const char *new) {
    size_t length = builder->len;
    size_t old_substring_length = strlen(old);
    size_t new_substring_length = strlen(new);
    size_t new_length = length + (new_substring_length - old_substring_length) * strbld_count_substrings(builder, old);
    strbld_ensure(builder, new_length);

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

    char *inner = builder->str;
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

    builder->len = new_length;
}

#endif // STRBLD_IMPLEMENTATION

#endif // STRBLD_H
