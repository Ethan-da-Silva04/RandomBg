
#include <stdio.h>
#include <string.h>

#include "string.h"

typedef struct {
	String path;
	String backend;
	bool recursive;
} Config;

void show_help();

void config_init_default(Config *config);
int config_init(Config *config, int argc, char **argv);

void config_set_recursive(Config *config, bool recursive);
bool config_set_path(Config *config, const char* path);
bool config_set_backend(Config *config, const char *backend);

String config_backend_command_template(const Config *config);

void config_free(Config *config);
