
#include <stdio.h>
#include "string_replacer.h"
#include "string.h"

StringReplacer string_replacer_from(String source) {
	StringReplacer replacer;
	replacer.current = source;
	return replacer;
}

StringReplacer string_replacer_replace(StringReplacer* self, const char* find_pattern, const String *replace_pattern) {
	String result = string_new();

	const String* fix = &self->current;

	size_t i = 0;

	while (i < string_length(fix)) {
		if (string_matches_from_raw(fix, find_pattern, i)) {
			string_append_str(&result, replace_pattern);
			i += string_length(replace_pattern) + 1;
		} else {
			string_append_char(&result, string_at(&self->current, i));
			i++;
		}
	}

	string_free(&self->current);
	self->current = result;
	return *self;
}

void string_replacer_free(StringReplacer *self) {
	string_free(&self->current);
}
