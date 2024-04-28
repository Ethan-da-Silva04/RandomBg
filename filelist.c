
#include "filelist.h"
#include "string.h"
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

FileList file_list_new() {
	FileList self;
	self.files = NULL;
	self.size = 0;
	self.capacity = 0;
	return self;
}

static void file_list_realloc(FileList *self, size_t n) {
	self->capacity = n;
	self->files = realloc(self->files, self->capacity * sizeof(String));
}

static void file_list_add_file(FileList *self, const String* path, const char *name) {
	if (self->capacity < self->size + 1) {
		file_list_realloc(self, self->capacity * 2 + 1);
	}

	String full_file = string_clone(path);
	string_append_raw(&full_file, name);

	self->files[self->size++] = full_file;
}

int file_list_from_dir_helper(String *path, FileList *self, bool (*filter_pred)(const char*), bool recursive) {
	DIR *dp = opendir(string_as_raw(path));
	if (dp == NULL) {
		fprintf(stderr, "Failed opening directory %s\n", string_as_raw(path));
		return -1;
	}

	struct dirent *ep;     
	while ((ep = readdir (dp)) != NULL) {
		if (recursive && ep->d_type == DT_DIR) {
			string_append_raw(path, ep->d_name);
			string_append_char(path, '/');
			file_list_from_dir_helper(path, self, filter_pred, recursive);
			string_shrink_back(path, strlen(ep->d_name) + 1);
		}

		if (filter_pred(ep->d_name)) {
			file_list_add_file(self, path, ep->d_name);
		}
	}

	closedir (dp);
	return 0;
}

FileList file_list_from_dir(const String *path, bool (*filter_pred)(const char*), bool recursive) {
	FileList self;
	self.size = 0;
	self.files = NULL;
	self.capacity = 0;

	String path_clone = string_clone(path);

	if (string_back(path) != '/') {
		string_append_char(&path_clone, '/');
	}

	file_list_from_dir_helper(&path_clone, &self, filter_pred, recursive);
	string_free(&path_clone);

	return self;
}

void file_list_free(FileList *self) {
	for (size_t i = 0; i < self->size; i++) {
		string_free(&self->files[i]);
	}
	free(self->files);
}
