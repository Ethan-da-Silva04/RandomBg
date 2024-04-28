
#include "string.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


// returns how many bytes the self is currently using (owned and in use). As an invariant this shall always be self->__length + 1, to account for the null character at the end.
static size_t get_size(const String *self) {
	return self->__length + 1;
}

size_t string_length(const String *self) {
	return self->__length;
}

String string_new() {
	String string;
	string.__data = NULL;
	string.__length = 0;
	string.__capacity = 0;
	return string;
}

String string_from_raw(const char* other) {
	String string;
	string.__length = strlen(other);
	string.__capacity = string.__length + 1;
	string.__data = malloc(string.__capacity);
	string.__data = memcpy(string.__data, other, string.__capacity);
	return string;
}

String string_clone(const String *self) {
	return string_substr(self, 0, self->__length);
}

String string_from_rep_char(char c, int count) {
	String string;
	string.__length = count;
	string.__capacity = string.__length + 1;
	string.__data = malloc(string.__capacity);

	for (size_t i = 0; i < string.__length; i++) {
		string.__data[i] = c;
	}

	string.__data[string.__length] = '\0';

	return string;
}

void string_realloc(String *self, size_t n) {
	if (n <= self->__capacity) {
		return;
	}

	self->__capacity = n;
	char *old_data = self->__data;
	self->__data = malloc(n);

	if (old_data != NULL) {
		strcpy(self->__data, old_data);
		free(old_data);
	}
}

void string_int_transform(String *self, int (*int_transform)(int)) {
	for (size_t i = 0; i < self->__length; i++) {
		string_set(self, i, int_transform(string_at(self, i)));
	}
}

void string_transform(String *self, char (*char_transform)(char)) {
	for (size_t i = 0; i < self->__length; i++) {
		string_set(self, i, char_transform(string_at(self, i)));
	}
}

void string_shrink_back(String *self, size_t amount) {
	if (self->__length == 0 || self->__length < amount) {
		self->__length = 0;
		return;
	}
	
	self->__length -= amount;
	self->__data[self->__length] = '\0';
}

void string_append_str(String *self, const String *other) {
	if (self == other) {
		String temp = string_clone(self);
		string_append_str(self, &temp);
		string_free(&temp);
		return;
	}

	if (self->__capacity < get_size(self) + other->__length) {
		string_realloc(self, self->__capacity + other->__length);
	}
	
	strcpy(self->__data + self->__length, other->__data);
	self->__length += other->__length;
}


void string_append_raw(String *self, const char* other) {
	size_t copy_length = strlen(other);
	if (self->__capacity < get_size(self) + copy_length) {
		string_realloc(self, self->__capacity + copy_length + 1);
	}

	strcpy(self->__data + self->__length, other);
	self->__length += copy_length;
}

void string_append_char(String *self, char c) {
	if (self->__capacity < get_size(self) + 1) {
		string_realloc(self, 5 + self->__capacity * 2);
	}

	self->__data[self->__length] = c;
	self->__length++;
	self->__data[self->__length] = '\0';
}

char string_back(const String *self) {
	return string_at(self, 0);
}

char string_front(const String *self) {
	return string_at(self, self->__length - 1);
}

char string_at(const String *self, size_t index) {
	return self->__data[index];
}

void string_set(String *self, size_t index, char value) {
	self->__data[index] = value;
}

bool string_beginswith(const String *self, const String *prefix) {
	return string_matches_from(self, prefix, 0);
}

bool string_endswith(const String *self, const String *suffix) {
	return string_matches_from(self, suffix, self->__length - suffix->__length);
}

bool string_matches_from(const String *self, const String *str, size_t i) {
	if (str->__length == 0) {
		return true;
	}

	if (i >= self->__length) {
		return false;
	}

	size_t j = 0;

	while (i < self->__length && j < str->__length) {
		if (string_at(self, i) != string_at(self, j)) {
			return false;
		}
		i++;
		j++;
	}

	return j == str->__length;
}

bool string_beginswith_raw(const String *self, const char* prefix) {
	return string_matches_from_raw(self, prefix, 0);
}

bool string_endswith_raw(const String *self, const char* suffix) {
	return string_matches_from_raw(self, suffix, self->__length - strlen(suffix));
}

bool string_matches_from_raw(const String *self, const char *str, size_t i) {
	size_t n = strlen(str);
	if (n == 0) {
		return true;
	}

	if (i >= self->__length) {
		return false;
	}

	size_t j = 0;

	while (i < self->__length && j < n) {
		if (string_at(self, i) != str[j]) {
			return false;
		}
		i++;
		j++;
	}

	return j == n;
}

bool string_eq(const String *fst, const String *snd) {
	return fst->__length == snd->__length && string_matches_from(fst, snd, 0);
}

bool string_eq_raw(const String *fst, const char *snd) {
	return fst->__length == strlen(snd) && string_matches_from_raw(fst, snd, 0);
}

String string_substr(const String *self, size_t begin, size_t length) {
	if (begin >= self->__length) {
		return string_from_raw("");
	}
	if (begin + length > self->__length) {
		length = self->__length - begin;
	}

	String result;
	result.__capacity = length + 1;
	result.__data = malloc(result.__capacity);
	memcpy(result.__data, self->__data + begin, length);
	result.__length = length;
	result.__data[result.__length] = '\0';
	return result;
}

char* string_into_raw(const String *self) {
	char *result = malloc(self->__length + 1);
	strcpy(result, self->__data);
	return result;
}

const char* string_as_raw(const String *self) {
	return self->__data;
}


void string_free(String *self) {
	if (self->__data != NULL) {
		free(self->__data);
	}
	self->__length = 0;
	self->__capacity = 0;
}
