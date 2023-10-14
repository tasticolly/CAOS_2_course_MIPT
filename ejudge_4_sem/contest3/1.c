#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  const char* cmd1 = argv[1];
  const char* cmd2 = argv[2];

  int pipe_fd[2];
  pipe(pipe_fd);

  pid_t pid_1 = fork();
  if (pid_1 == -1) {
    return 1;
  }
  if (pid_1 == 0) {
    dup2(pipe_fd[1], STDOUT_FILENO);
    close(pipe_fd[1]);
    close(pipe_fd[0]);
    execlp(cmd1, cmd1, NULL);
    return 1;
  }
  close(pipe_fd[1]);
  pid_t pid_2 = fork();
  if (pid_2 == 0) {
    dup2(pipe_fd[0], STDIN_FILENO);
    close(pipe_fd[1]);
    close(pipe_fd[0]);
    execlp(cmd2, cmd2, NULL);
    return 1;
  }
  close(pipe_fd[1]);
  close(pipe_fd[0]);
  waitpid(pid_1, NULL, 0);
  waitpid(pid_2, NULL, 0);
  return 0;
}
