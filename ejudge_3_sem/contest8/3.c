#define _FILE_OFFSET_BITS 64

#include <windows.h>

#include <stdio.h>
#include <stdint.h>

struct Item {
  int value;
  uint32_t next_pointer;
};

int main(int argc, char* argv[]) {

  if (argc != 2)
    return 0;
  char* file_path = argv[1];
  int ret = 0;
  HANDLE file = CreateFileA(file_path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (file < 0) {
    ret = 1;
    goto Exit;
  }
  struct Item elem;
  DWORD input;
  ReadFile(file, &elem, sizeof(elem), &input, NULL);
  if (input <= 0) {
    goto Exit;
  }
  SetFilePointer(file, 0, NULL, FILE_BEGIN);
  while (elem.next_pointer != 0) {
    ReadFile(file, &elem, sizeof(elem), &input, NULL);
    if (input < 0) {
      ret = 1;
      goto Exit;
    }
    printf("%d ", elem.value);
    SetFilePointer(file, elem.next_pointer, NULL, FILE_BEGIN);
  }
  Exit:
  DeleteFileA(file_path);
  return ret;
}