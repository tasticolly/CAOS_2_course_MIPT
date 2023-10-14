#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

int main() {

  int a[5] = {7, 2, 1, 4, 15};
  for (int j = 0; j < 5; ++j) {
    for (int i = 0; i < 4; ++i) {
      printf("%c", ((char*) &a[j])[i]);
    }
  }
}