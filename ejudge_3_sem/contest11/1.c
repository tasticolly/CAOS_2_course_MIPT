#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
int main(int argc, char** argv) {
  int n = strtol(argv[1], NULL, 10);
  for (int i = 0; i < n - 1; ++i) {
    printf("%d ", i + 1);
    fflush(stdout);
    int id = fork();
    int fork_ret;
    waitpid(id, &fork_ret, 0);
    if (fork_ret == -1) {
      return 1;
    }
    if (!id) {
      return 0;
    }
  }
  printf("%d\n", n);
}