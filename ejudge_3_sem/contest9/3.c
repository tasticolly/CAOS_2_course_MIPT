#include <inttypes.h>
#include <limits.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

void zero_terminate(char* str) {
  for (int i = 0;; ++i) {
    if (str[i] == '\n') {
      str[i] = '\0';
      break;
    }
  }
}

int main() {
  char file_name[PATH_MAX];
  struct stat attrs;
  while (fgets(file_name, PATH_MAX, stdin) != NULL) {
    zero_terminate(file_name);
    if (lstat(file_name, &attrs) == 0) {
      if (S_ISLNK(attrs.st_mode)) {
        char real_path_name[PATH_MAX];
        realpath(file_name, real_path_name);
        printf("%s\n", real_path_name);
      } else if (S_ISREG(attrs.st_mode)) {
        char link_file_name[PATH_MAX];
        snprintf(link_file_name, PATH_MAX + 100, "link_to_%s", file_name);
        symlink(file_name, link_file_name);
      }
    }
  }

  return 0;
}