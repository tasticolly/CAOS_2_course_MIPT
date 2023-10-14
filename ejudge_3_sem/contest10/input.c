#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>

#define size 4
typedef struct S {
  int value;
  uint32_t next_pointer;
} Item;

int main() {
  Item a[size] = {{234, 8}, {1234, 16}, {12345, 24}, {1, 0}};
  for (int j = 0; j < size; ++j) {
    for (int i = 0; i < 8; ++i) {
      printf("%c", ((char*) &a[j])[i]);
    }
  }
//  printf("%s%s%s",&a,&b,&c);
}