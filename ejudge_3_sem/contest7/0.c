#include <sys/syscall.h>
extern long syscall(long number, ...);
void _start() {
  const char text[13] = "Hello, World!";
  syscall(SYS_write, 1, text, sizeof(text));
  syscall(SYS_exit, 0);
}