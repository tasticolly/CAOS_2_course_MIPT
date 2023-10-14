#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
const int BUFFER = 4096;
const int NUMBER_BUFFER = 128;

void parse_line(const char* line,
                size_t* warning_count,
                size_t* last_warning,
                size_t* error_count,
                size_t* last_error,
                const char* program_name) {
  int i;
  for (i = 0; i < strlen(program_name); ++i) {
    if (line[i] != program_name[i]) {
      return;
    }
  }
  i += 1; //read ":"

  if (!(line[i] <= '9' && line[i] >= '0')) {
    return;
  }

  char num_of_line_str[NUMBER_BUFFER];
  int j;
  for (j = 0; line[i + j] != ':'; ++j) {
    num_of_line_str[j] = line[i + j];
  }
  size_t num_of_line = atoi(num_of_line_str);
  i += j + 1;

  while (line[i] != ' ') {
    ++i;
  }

  ++i; // now we at the first symbol of word (warning or error)
  if (line[i] == 'w' && *last_warning != num_of_line) {
    ++(*warning_count);
    *last_warning = num_of_line;
  } else if (line[i] == 'e' && *last_error != num_of_line) {
    ++(*error_count);
    *last_error = num_of_line;
  }
}
void read_from_channel(int* pipe_fd, const char* program_name) {
  char buffer[BUFFER];
  dup2(pipe_fd[0], STDIN_FILENO);
  size_t warning_count = 0;
  size_t error_count = 0;
  size_t last_error = 0;
  size_t last_warning = 0;
  while (fgets(buffer, BUFFER, stdin) != NULL) {
    parse_line(buffer, &warning_count, &last_warning, &error_count, &last_error, program_name);
  }
  printf("%lu %lu", error_count, warning_count);
}

int main(int argc, char** argv) {
  const char* c_program = argv[1];

  int pipe_fd[2];
  pipe(pipe_fd);

  pid_t pid = fork();
  if (pid == -1) {
    return 1;
  }
  if (pid == 0) {
    dup2(pipe_fd[1], STDERR_FILENO);
    close(pipe_fd[1]);
    close(pipe_fd[0]);
    execlp("gcc", "gcc", c_program, NULL);
    return 1;
  }

  close(pipe_fd[1]);
  read_from_channel(pipe_fd, c_program);
  waitpid(pid, NULL, 0);
  return 0;
}
