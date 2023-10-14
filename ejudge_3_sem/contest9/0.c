#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

void delete_line_break(char* file_name) {
  for (int i = 0; i < FILENAME_MAX; ++i) {
    if (file_name[i] == '\n'){
      file_name[i] ='\0';
      return;
    }
  }
}

int main() {
  char* file_name = calloc(FILENAME_MAX, sizeof(char));
  u_int64_t total_size_of_files = 0;
  struct stat stat_buffer;

  while (fgets(file_name, FILENAME_MAX, stdin) != NULL) {
    delete_line_break(file_name);
    ssize_t stat_ret = lstat(file_name, &stat_buffer);
    if (stat_ret != -1 && S_ISREG(stat_buffer.st_mode)) {
      total_size_of_files += stat_buffer.st_size;
    }
  }
  printf("%lu", total_size_of_files);
  free(file_name);
  return 0;
}