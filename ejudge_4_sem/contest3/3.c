#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>


int main(int argc, char** argv) {
  pid_t pids[argc];
  for (int i = 1; i < argc; ++i) {
    int pipe_fd[2];
    pipe(pipe_fd);

    pid_t pid = fork();
    pids[i] = pid;
    if (pid == -1) {
      return 1;
    }
    if (pid == 0) {
      if (i != argc - 1) {
        dup2(pipe_fd[1], STDOUT_FILENO);
      }
      close(pipe_fd[1]);
      close(pipe_fd[0]);
      execlp(argv[i], argv[i], NULL);
      return 1;
    }
    close(pipe_fd[1]);
    dup2(pipe_fd[0], STDIN_FILENO);
  }

  for (int i = 1; i < argc; ++i) {
    waitpid(pids[i], NULL, 0);
  }
  return 0;
}
