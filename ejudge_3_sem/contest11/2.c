#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdlib.h>
#define MAX_COUNT 255
#define MAX_SIZE_OF_WORD 4096

int main() {
  int counter = 0;
  char word[MAX_SIZE_OF_WORD];
  for (int i = 0; i < MAX_COUNT; ++i) {
    pid_t process = fork();
    if (process < 0) {
      return -1;
    } else if (process == 0) {
      int ret_scan = scanf("%s", word);
      return ret_scan != EOF;
    } else {
      int fork_ret;
      waitpid(process, &fork_ret, 0);
      if (fork_ret == -1) {
        return -1;
      }
      counter += WEXITSTATUS(fork_ret);
    }
  }
  printf("%d", counter);
  return 0;
}