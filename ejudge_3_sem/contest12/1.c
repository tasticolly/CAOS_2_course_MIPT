#define MAX_LEN 4096

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

char* make_text_of_program(char* s, char* result) {
  char text_program_first[MAX_LEN] = "#include <stdio.h>\n\nint main() {\n  int result = (";
  char text_program_second[MAX_LEN] = ");\n  printf(\"%d\\n\", result);\n}\n\0";
  snprintf(result, MAX_LEN, "%s%s%s", text_program_first, s, text_program_second);
  return result;
}

int compile(char* s) {
  const char file_name[MAX_LEN] = "main.c";
  remove(file_name);
  int file = open(file_name, O_WRONLY | O_CREAT, 0640);
  if (file < 0) {
    return 1;
  }
  char result[MAX_LEN];
  make_text_of_program(s, result);
  write(file, result, strlen(result));
  close(file);

  int ret = execlp("gcc", "gcc", file_name, NULL);
  if (ret < 0) {
    return 2;
  }
  return 0;
}

int run(char* s) {
  return execlp("bash", "bash", "-c", s, NULL);
}

int main() {
  char expression[MAX_LEN];
  fgets(expression, MAX_LEN, stdin);
  for (int i = 0; i < MAX_LEN; ++i) {
    if (expression[i] == '\n' || expression[i] == '\0') {
      expression[i] = '\0';
      break;
    }
  }
  pid_t compile_process = fork();
  if (compile_process == 0) {
    int ret = compile(expression);
    if (ret != 0) {
      return ret;
    }
  } else if (compile_process > 0) {
    waitpid(compile_process, NULL, 0);
    int ret = run("./a.out");
    if (ret < 0) {
      return 3;
    }
  } else {
    return -1;
  }
}