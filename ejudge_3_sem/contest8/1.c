#define _FILE_OFFSET_BITS 64
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
struct Item {
  int value;
  uint32_t next_pointer;
};

int main(int argc, char* argv[]) {

  if (argc != 2)
    return 1;
  char* file_path = argv[1];
  int ret = 0;
  int file = open(file_path, O_RDONLY, 0640);
  if (file < 0) {
    ret = 1;
    goto Exit;
  }
  struct Item elem = {0, 0};
  do {
    lseek(file, elem.next_pointer, SEEK_SET);
    ssize_t read_count = read(file, &elem, sizeof(elem));
    if (read_count < 0) {
      ret = 1;
      goto Exit;
    }
    if (read_count == 0) {
      ret = 0;
      goto Exit;
    }
    printf("%d ", elem.value);
  } while (elem.next_pointer != 0);
  Exit:
  close(file);
  return ret;
}