#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

#define SIZE_OF_BUFFER  2 * 1024 * 1024
int main(int argc, char** argv) {
  char expression[SIZE_OF_BUFFER] = "print(";
  fgets(expression + 6, SIZE_OF_BUFFER, stdin);
  for (int i = 0; i < SIZE_OF_BUFFER; ++i) {
    if (expression[i] == '\n' || expression[i] == '\0') {
      expression[i] = ')';
      expression[i + 1] = '\0';
      break;
    }
  }
  execlp("python3", "python3", "-c", expression, NULL);
}