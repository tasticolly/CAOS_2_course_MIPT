#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  uint64_t count_of_process = 1;
  while (1) {
    int pid = fork();
    if (pid == -1) {
      printf("%lu\n", count_of_process);
      return 0;
    }
    if (pid != 0) {
      if (waitpid(pid, NULL, 0) != -1) {
        return 1;
      }
      break;
    }
    ++count_of_process;
  }
  return 0;
}