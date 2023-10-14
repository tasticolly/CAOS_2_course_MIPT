#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

typedef struct S {
  int value;
  uint32_t next_pointer;
} Item;

int main(int argc, char* argv[]) {
  int ret = 0;
  if (argc != 2){
    return 1;
  }
  char* file_name = argv[1];
  int fd = open(file_name, O_RDONLY);
  if (fd < 0){
    ret = 1;
    goto Exit;
  }
  struct stat file_info;
  fstat(fd, &file_info);
  long size_file = file_info.st_size;
  void* ptr = mmap(NULL, size_file, PROT_READ, MAP_PRIVATE, fd, 0);
  if (MAP_FAILED == ptr) {
    ret = 1;
    goto Exit;
  }
  Item* start = ptr;
  uint32_t next_pointer = 0;
  do {
    printf("%d ", start[next_pointer / sizeof(Item)].value);
    next_pointer = start[next_pointer / sizeof(Item)].next_pointer;
  } while (next_pointer != 0);
  munmap(ptr, size_file);

  Exit:
  close(fd);
  return ret;
}