
#include <stdio.h>
#include <string.h>

#include "string.h"
#include "config.h"

void show_help() {
	puts("--path=\"...\"\n\t Set the path to look for images. Can use relative or absolute paths.");
	puts("--backend=\"...\"\n\t Set the backend to use for setting the background. Currently available options are: 'feh'");
	puts("--help\n\t Display this message.");
	puts("--p=\"...\"\n\t Set the path to look for images. Can use relative or absolute paths.");
	puts("--b=\"...\"\n\t Set the backend to use for setting the background. Currently available options are: 'feh'");
	puts("--h\n\t Display this message.");
}

void config_init_default(Config *config) {
	config->path = string_from_raw("/home/ethan/Pictures/");
	config->backend = string_from_raw("feh");
	config->recursive = false;
}

int config_init(Config *config, int argc, char **argv) {
	config_init_default(config);
	if (argc == 1) {
		show_help();
		return 0;
	}

	for (size_t i = 1; i < argc; i++) {
		String arg = string_from_raw(argv[i]);
		if (string_beginswith_raw(&arg, "--path=")) {
			config_set_path(config, argv[i] + strlen("--path="));
		} else if (string_beginswith_raw(&arg, "--p=")) {
			config_set_backend(config, argv[i] + strlen("--p="));
		} else if (string_beginswith_raw(&arg, "--backend=")) {
			config_set_backend(config, argv[i] + strlen("--backend="));
		} else if (string_beginswith_raw(&arg, "--b=")) {
			config_set_backend(config, argv[i] + strlen("--b="));
		} else if (strcmp(argv[i], "--help") == 0) {
			show_help();
			string_free(&arg);
			return 0;
		} else if (strcmp(argv[i], "--h") == 0) {
			show_help();
			string_free(&arg);
			return 0;
		} else if (strcmp(argv[i], "--recursive") == 0) {
			config_set_recursive(config, true);
		} else {
			fprintf(stderr, "Unrecognized flag '%s'\n", argv[i]);
		}

		string_free(&arg);
	}

	return 0;
}

void config_free(Config *config) {
	string_free(&config->path);
	string_free(&config->backend);
}

void config_set_recursive(Config *config, bool recursive) {
	config->recursive = recursive;
}

bool config_set_path(Config *config, const char* path) {
	config->path = string_from_raw(path);
	if (string_back(&config->path) != '/') {
		string_append_char(&config->path, '/');
	}

	return true;
}

String config_backend_command_template(const Config* config) {
	const String *backend = &config->backend;
	if (string_eq_raw(backend, "feh")) {
		return string_from_raw("feh --bg-scale <file-path>");
	}
	
	fprintf(stderr, "Unsupported backend: '%s'.\n", string_as_raw(backend));
	return string_from_raw("");
}

bool config_set_backend(Config *config, const char *backend) {
	config->backend = string_from_raw(backend);
	return true;
}

