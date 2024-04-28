#include <ctype.h>
#define _XOPEN_SOURCE 700
#define GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <dirent.h>
#include <memory.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "string.h"
#include "string_replacer.h"
#include "config.h"
#include "filelist.h"

bool is_image(const char* path) {
	String lowered = string_from_raw(path);
	string_int_transform(&lowered, tolower);
	static const char *accepted_extensions[3] = {".jpeg", ".png", ".jpg"};

	for (size_t i = 0; i < 3; i++) {
		if (string_endswith_raw(&lowered, accepted_extensions[i])) {
			string_free(&lowered);
			return true;
		}
	}

	string_free(&lowered);
	
	return false;
}


int rand_int(int fst, int snd) {
	return fst + rand() % (snd - fst);
}

int set_background(const Config *config, const String* file_choice) {
	String command = config_backend_command_template(config);
	
	StringReplacer replacer = string_replacer_from(command);
	string_replacer_replace(&replacer, "<file-path>", file_choice);
	command = replacer.current;
	puts(string_as_raw(&command));

	int result = system(string_as_raw(&command));
	
	string_free(&command);

	return result;
}

int set_bg_from_conf(const Config *config) {
	String path = string_clone(&config->path);
	FileList list = file_list_from_dir(&path, is_image, config->recursive);
	
	if (list.size == 0) {
		fprintf(stderr, "Could not find any images in '%s'.\n", string_as_raw(&path));
		file_list_free(&list);
		return -1;
	}

	int result = set_background(config, &list.files[rand_int(0, list.size - 1)]);

	file_list_free(&list);
	string_free(&path);

	return result;
}

int main (int argc, char **argv) {
	srand(time(NULL));
	Config config;
	config_init(&config, argc, argv);
	set_bg_from_conf(&config);
	config_free(&config);
	return 0;
}
