
#include "string.h"
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

typedef struct {
	String * files;
	size_t size;
	size_t capacity;
} FileList;

FileList file_list_new();
FileList file_list_from_dir(const String *path, bool (*filter_pred)(const char*), bool recursive); 
void file_list_free(FileList *self);
