#include <error.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main(int argc, char* argv[]) {
  const char* hostname = argv[1];
  const char* scriptpath = argv[2];
  const char* localfile = argv[3];

  struct addrinfo* res_list;
  if (getaddrinfo(hostname, "8081", NULL, &res_list) != 0) {
    error(EXIT_FAILURE, errno, "Could not obtain server address");
  }
  const struct addrinfo* first_res = res_list;

  int server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock == -1) {
    error(EXIT_FAILURE, errno, "Could not create socket");
  }

  int connect_status =
      connect(server_sock, first_res->ai_addr, first_res->ai_addrlen);
  freeaddrinfo(res_list);
  if (connect_status == -1) {
    error(EXIT_FAILURE, errno, "Could not connect to the server");
  }

  int file = open(localfile, O_RDONLY);
  if (file == -1) {
    error(EXIT_FAILURE, errno, "Could not open local file");
  }

  struct stat attributes;
  if (fstat(file, &attributes) == -1) {
    error(EXIT_FAILURE, errno, "Could not get file attributes");
  }

  char text[attributes.st_size + 1];
  text[attributes.st_size] = '\0';
  if (read(file, text, attributes.st_size) == -1) {
    error(EXIT_FAILURE, errno, "Could not read local file");
  }

  const size_t request_size = 4096;
  char* request_buff = calloc(request_size, sizeof(char));
  if (request_buff == NULL) {
    error(EXIT_FAILURE, errno, "Could not allocate memory for request buffer");
  }

  const char* request_fmt =
      "POST %s HTTP/1.1\r\n"
      "Host: %s\r\n"
      "Content-Type: text/plain; charset=UTF-8\r\n"
      "Content-Length: %lu\r\n"
      "Connection: close\r\n\r\n"
      "%s";

  size_t real_size = snprintf(request_buff, request_size, request_fmt, scriptpath, hostname, attributes.st_size, text);
  if (send(server_sock, request_buff, real_size, 0) == -1) {
    error(EXIT_FAILURE, errno, "Could not send request to server");
  }

  free(request_buff);

  const size_t response_size = 1024;
  char* response_buff = calloc(response_size, sizeof(char));
  if (response_buff == NULL) {
    error(EXIT_FAILURE, errno, "Could not allocate memory for response buffer");
  }

  FILE* sock_stream = fdopen(server_sock, "r");
  if (sock_stream == NULL) {
    error(EXIT_FAILURE, errno, "Could not create stream from socket");
  }

  bool headers_received = false;
  while (fgets(response_buff, response_size, sock_stream) != NULL) {
    if (strcmp(response_buff, "\n") == 0
        || strcmp(response_buff, "\r\n") == 0) {
      headers_received = true;
      continue;
    }

    if (headers_received) {
      printf("%s", response_buff);
    }
  }

  free(response_buff);

  if (fclose(sock_stream) == EOF) {
    error(EXIT_FAILURE, errno, "Could not close socket stream");
  }

  close(server_sock);

  return EXIT_SUCCESS;
}