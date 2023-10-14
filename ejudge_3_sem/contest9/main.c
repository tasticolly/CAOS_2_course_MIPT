#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

void zero_terminate(char* str) {
  for (int i = 0;; ++i) {
    if (str[i] == '\n') {
      str[i] = '\0';
      break;
    }
  }
}

int main() {
  struct stat attrs;
  char* file_name = calloc(PATH_MAX, sizeof(char));
  char elf_bytes[4] = {0x7f, 'E', 'L', 'F'};
  char shebang[2] = {'#', '!'};
  char* read_bytes = calloc(6, sizeof(char));
  char* read_path = calloc(PATH_MAX, sizeof(char));
  int fd, cnt;
  while (fgets(file_name, PATH_MAX, stdin) != NULL) {
    zero_terminate(file_name);
    if (access(file_name, F_OK | X_OK) == 0) {
      fd = open(file_name, O_RDONLY);
      if (fd >= 0 && read(fd, &read_bytes, 4) == 4
          && memcmp(read_bytes, elf_bytes, sizeof elf_bytes) == 0) {
        close(fd);
        continue;
      }
      if (lseek(fd, 0, SEEK_SET) >= 0
          && read(fd, &read_bytes + 4, 2) == 2
          && memcmp(read_bytes + 4, shebang, sizeof shebang) == 0
          && lseek(fd, 2, SEEK_SET) >= 0
          && read(fd, &read_path, PATH_MAX) >= 0) {
        zero_terminate(read_path);
        if (access(read_path, F_OK | X_OK) == 0) {
          close(fd);
          continue;
        }
      }
    }
    printf("%s\n", file_name);
    close(fd);
  }
  free(read_path);
  free(read_bytes);
  free(file_name);
  return 0;
}