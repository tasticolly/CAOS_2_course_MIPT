#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_SIZE 4096

char HTTP_HOST[MAX_SIZE], QUERY_STRING[MAX_SIZE], REQUEST_METHOD[MAX_SIZE],
    SCRIPT_NAME[MAX_SIZE];

void input(char* first_line, char* second_line)
{
  fgets(first_line, MAX_SIZE, stdin);
  first_line[strlen(first_line) - 1] = '\0';

  fgets(second_line, MAX_SIZE, stdin);
  second_line[strlen(second_line) - 1] = '\0';

  getchar();
}

void set_env_vars(char* first_line, char* second_line)
{
  uint32_t pos;

  pos = 0;
  while (first_line[pos + 5] != '?' && first_line[pos + 5] != ' ') {
    SCRIPT_NAME[pos] = first_line[pos + 5];
    ++pos;
  }
  SCRIPT_NAME[pos] = '\0';
  setenv("SCRIPT_NAME", SCRIPT_NAME, 1);

  uint32_t shift = pos + 5;
  pos = 0;
  if (first_line[shift] == '?') {
    ++shift;
    while (first_line[pos + shift] != ' ') {
      QUERY_STRING[pos] = first_line[pos + shift];
      ++pos;
    }
  }
  QUERY_STRING[pos] = '\0';
  setenv("QUERY_STRING", QUERY_STRING, 1);

  pos = 0;
  do {
    HTTP_HOST[pos] = second_line[pos + 6];
  } while (second_line[pos++ + 6] != '\0');
  setenv("HTTP_HOST", HTTP_HOST, 1);

  pos = 0;
  while (first_line[pos] != ' ') {
    REQUEST_METHOD[pos] = first_line[pos];
    ++pos;
  }
  setenv("REQUEST_METHOD", REQUEST_METHOD, 1);
}

void show_env_vars()
{
  printf("SCRIPT_NAME: %s\n", getenv("SCRIPT_NAME"));
  printf("QUERY_STRING: %s\n", getenv("QUERY_STRING"));
  printf("HTTP_HOST: %s\n", getenv("HTTP_HOST"));
  printf("REQUEST_METHOD: %s\n", getenv("REQUEST_METHOD"));
}

int main()
{
  char first_line[MAX_SIZE], second_line[MAX_SIZE];
  input(first_line, second_line);
  //    printf("%s\n", first_line);
  //    printf("%s\n", second_line);
  set_env_vars(first_line, second_line);
  //    show_env_vars();

  if (access(SCRIPT_NAME, F_OK) != 0) {
    printf("HTTP/1.1 404 ERROR\n\n");
  } else if (access(SCRIPT_NAME, X_OK) != 0) {
    printf("HTTP/1.1 403 ERROR\n\n");
  } else {
    printf("HTTP/1.1 200 OK\n");
    fflush(stdout);
    execl(SCRIPT_NAME, QUERY_STRING, NULL);
  }

  return 0;
}