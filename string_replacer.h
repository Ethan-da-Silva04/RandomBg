#pragma once

#include "string.h"
#include <string.h>

typedef struct {
	String current;
} StringReplacer;

// takes ownership of source
StringReplacer string_replacer_from(String source);

StringReplacer string_replacer_replace(StringReplacer* self, const char* find_pattern, const String*replace_pattern);

void string_replacer_free(StringReplacer *self);
