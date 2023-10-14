#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdint.h>

void print_positions(char* pattern, char* text, int text_length) {
  int pattern_length = strlen(pattern);
  for (int first_position = 0; first_position < text_length - pattern_length + 1; ++first_position) {
    int shift = 0;
    while (text[first_position + shift] == pattern[shift] && shift < pattern_length) {
      ++shift;
    }
    if (shift == pattern_length) {
      printf("%d\n", first_position);
    }
  }
}

int main(int argc, char** argv) {
  int ret = 0;
  char* pattern = argv[2];
  int file = open(argv[1], O_RDONLY);
  if (file < 0) {
    return 1;
  }
  struct stat attributes;
  int ret_stat = fstat(file, &attributes);
  if (ret_stat < -1) {
    ret = 2;
    goto Exit;
  }
  void* mapped_point = mmap(NULL, attributes.st_size, PROT_READ, MAP_PRIVATE, file, 0);
  if (mapped_point == NULL) {
    ret = 3;
    goto Exit;
  }
  char* text = mapped_point;

  print_positions(pattern, text, attributes.st_size);
  munmap(mapped_point, attributes.st_size);

  Exit:
  close(file);
  return ret;
}