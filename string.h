#pragma once

#include <stddef.h>
#include <stdbool.h>

typedef struct {
	char* __data;

	// how many characters (not including '\0') are in the string
	size_t __length;

	// how many bytes the String owns
	size_t __capacity;
} String;

String string_new();
String string_from_raw(const char* other);
String string_clone(const String* self);
String string_from_rep_char(char c, int count);

// returns how many characters (not including the null character) are in the string. Analogous to strlen, but runs in constant time.
size_t string_length(const String * self);

// Changes the capacity of string to n bytes
void string_realloc(String *self, size_t n);

void string_int_transform(String *self, int (*int_transform)(int));
void string_transform(String *self, char (*char_transform)(char));

void string_shrink_back(String *self, size_t amount);

void string_append_str(String *self, const String *other);
void string_append_raw(String *self, const char* other);
void string_append_char(String *self, char c);

char string_back(const String *self);
char string_front(const String *self);

char string_at(const String *self, size_t index);
void string_set(String *self, size_t index, char value);

bool string_beginswith(const String *self, const String *prefix);
bool string_endswith(const String *self, const String *suffix);
bool string_matches_from(const String *self, const String *str, size_t i);

bool string_beginswith_raw(const String *self, const char* prefix);
bool string_endswith_raw(const String *self, const char* suffix);
bool string_matches_from_raw(const String *self, const char *str, size_t i);

bool string_eq(const String *fst, const String *snd);
bool string_eq_raw(const String *fst, const char *snd);

// returns the string of self[begin: length - begin] put in negative one for length to get self[begin:]
String string_substr(const String* self, size_t begin, size_t length);

// returns a copy of the string as a null-terminated char*
char* string_into_raw(const String *self);

// returns the data of the string, the data is returned as immutable as the functions provided should be used to mutate the string instead
const char* string_as_raw(const String *self);

void string_free(String *self);
