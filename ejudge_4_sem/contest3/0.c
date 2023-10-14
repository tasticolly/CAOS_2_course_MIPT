#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
size_t read_from_channel(int* pipe_fd) {
  char buffer[4096];
  ssize_t bytes_read;
  size_t total_read = 0;
  while (1) {
    bytes_read = read(pipe_fd[0], buffer, sizeof(buffer));
    if (bytes_read <= 0) {
      return total_read;
    }
    total_read += bytes_read;
  }
}

int main(int argc, char** argv) {
  const char* cmd = argv[1];
  const char* input = argv[2];
  int input_fd = open(input, O_RDWR);
  if (input_fd == -1) {
    return 1;
  }
  int pipe_fd[2];
  pipe(pipe_fd);

  pid_t pid = fork();
  if (pid == -1) {
    return 1;
  }
  if (pid == 0) {
    dup2(input_fd, STDIN_FILENO);
    dup2(pipe_fd[1], STDOUT_FILENO);
    close(input_fd);
    close(pipe_fd[1]);
    close(pipe_fd[0]);
    execl(cmd, cmd, NULL);
    exit(1);
  }
  close(pipe_fd[1]);
  printf("%ld", read_from_channel(pipe_fd));
  wait(NULL);
  return 0;
}
