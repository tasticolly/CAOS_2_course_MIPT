#include <sys/syscall.h>
#define stdin 0
#define stdout 1
const int SIZE_OF_CHUNK = 16 * 1024;
extern long syscall(long number, ...);
void _start() {
  char text[SIZE_OF_CHUNK];
  long count = SIZE_OF_CHUNK;
  while (count == SIZE_OF_CHUNK) {
    count = syscall(SYS_read, stdin, text, SIZE_OF_CHUNK);
    if (count == -1) {
      syscall(SYS_exit, 0);
    }
    syscall(SYS_write, stdout, text, count);
  }
  syscall(SYS_exit, 0);
}