#define MAX_LEN 4096
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

void delete_line_break(char* line) {
  for (int i = 0; i < MAX_LEN; ++i) {
    if (line[i] == '\n' || line[i] == '\0') {
      line[i] = '\0';
      return;
    }
  }
}

void process_first_line(char* first_line, char* request_method, char* script_name, char* query_string) {
  int curr_pos = 0;
  while (first_line[curr_pos] != ' ') {
    request_method[curr_pos] = first_line[curr_pos];
    ++curr_pos;
  }
  request_method[curr_pos] = '\0';
  setenv("REQUEST_METHOD", request_method, 1);

  while (first_line[curr_pos] == ' ') {
    ++curr_pos;
  }
  curr_pos += 1;
  int shift = curr_pos;
  while (first_line[curr_pos] != ' ' && first_line[curr_pos] != '?') {
    script_name[curr_pos - shift] = first_line[curr_pos];
    ++curr_pos;
  }
  script_name[curr_pos - shift] = '\0';
  setenv("SCRIPT_NAME", script_name, 1);

  if (first_line[curr_pos] == '?') {
    ++curr_pos;
    shift = curr_pos;
    while (first_line[curr_pos] != ' ') {
      query_string[curr_pos - shift] = first_line[curr_pos];
      ++curr_pos;
    }
  }
  query_string[curr_pos - shift] = '\0';
  setenv("QUERY_STRING", query_string, 1);
}

void process_second_line(char* second_line, char* http_host) {
  int curr_pos = 6;
  int shift = curr_pos;
  while (second_line[curr_pos] != ' ' && second_line[curr_pos] != '\0') {
    http_host[curr_pos - shift] = second_line[curr_pos];
    ++curr_pos;
  }
  http_host[curr_pos - shift] = '\0';
  setenv("HTTP_HOST", http_host, 1);
}

void get(char* script_name, char* query_string) {
  if (access(script_name, F_OK) != 0) {
    printf("HTTP/1.1 404 ERROR\n\n");
  } else if (access(script_name, X_OK) != 0) {
    printf("HTTP/1.1 403 ERROR\n\n");
  } else {
    printf("HTTP/1.1 200 OK\n");
    fflush(stdout);
    execl(script_name, query_string, NULL);
  }
}

int main() {
  char first_line[MAX_LEN];
  char second_line[MAX_LEN];

  fgets(first_line, MAX_LEN, stdin);
  delete_line_break(first_line);
  fgets(second_line, MAX_LEN, stdin);
  delete_line_break(second_line);
  getchar();

  char request_method[MAX_LEN];
  char script_name[MAX_LEN];
  char query_string[MAX_LEN];
  char http_host[MAX_LEN];

  process_first_line(first_line, request_method, script_name, query_string);
  process_second_line(second_line, http_host);
  get(script_name, query_string);

}