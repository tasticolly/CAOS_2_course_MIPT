#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <elf.h>
void delete_line_break(char* file_name) {
  for (int i = 0; i < FILENAME_MAX; ++i) {
    if (file_name[i] == '\n') {
      file_name[i] = '\0';
      return;
    }
  }
}

int main() {
  char file_name[FILENAME_MAX];

  while (fgets(file_name, FILENAME_MAX, stdin) != NULL) {
    delete_line_break(file_name);
    if (access(file_name, X_OK) == 0) {

      int file = open(file_name, O_RDONLY, 0644);
      char buffer_elf[5]; //.ELF
      read(file, buffer_elf, 4);
      buffer_elf[4] = '\n';
      if (memcmp(buffer_elf, ELFMAG, SELFMAG) == 0) {
        continue;
      }
      lseek(file, 0, SEEK_SET);
      uint16_t shebang;
      read(file, &shebang, 2);
      if (shebang != 256 * '!' + '#') {  //little ending
        printf("%s\n", file_name);
        continue;
      }
      char interpreter_path[FILENAME_MAX];
      read(file, interpreter_path, FILENAME_MAX);
      delete_line_break(interpreter_path);
      if (access(interpreter_path, X_OK) == 0) {
        continue;
      }
      printf("%s\n", file_name);
    }
  }
  return 0;
}