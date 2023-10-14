#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <error.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/mman.h>
#include <netdb.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>

volatile sig_atomic_t sigterm_sigint_flag = 0;

void close_socket(int fd) {
  shutdown(fd, SHUT_RDWR);
  close(fd);
}

int accept_client(int socket_fd) {
  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(client_addr);
  int client_fd = accept(socket_fd, (struct sockaddr*) (&client_addr), &addr_len);
  if (-1 == client_fd) {
    close(socket_fd);
  }
  return client_fd;
}

int create_server(const char* ip_address, int port_number) {
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct in_addr server_ip;
  inet_aton(ip_address, &server_ip);
  struct sockaddr_in server_addr = {.sin_family = AF_INET, .sin_port = htons(port_number), .sin_addr = server_ip};

  int bind_status = bind(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));
  if (bind_status != 0) {
    close_socket(socket_fd);
    error(EXIT_FAILURE, errno, "Could not bind to %s:%d", ip_address, port_number);
  }
  return socket_fd;
}

size_t recv_client_msg(int client_fd, int socket_fd, char* buffer, size_t buffer_len) {
  int recv_status = recv(client_fd, buffer, buffer_len, 0);
  if (recv_status < 0) {
    if (errno == EINTR) {
      return recv_client_msg(client_fd, socket_fd, buffer, buffer_len);
    }
    close(client_fd);
    close(socket_fd);
    error(
        EXIT_FAILURE,
        errno,
        "Could not receive message from the client"
    );
  } else if (recv_status == 0) {
    close(client_fd);
    close(socket_fd);
    error(
        EXIT_FAILURE,
        0,
        "Empty message from the client"
    );
  }
  return recv_status;
}

void send_msg_to_client(int client_fd, int socket_fd, const char* msg, size_t msg_len) {
  if (-1 == send(client_fd, msg, msg_len, 0)) {
    if (errno == EINTR) {
      send_msg_to_client(client_fd, socket_fd, msg, msg_len);
      return;
    }
    close(client_fd);
    close(socket_fd);
    error(
        EXIT_FAILURE,
        errno,
        "Could not send message to the client"
    );
  }
}

void process_client(int socket_fd, const char* server_ipa_str, int server_port, char* dir_path) {
  int client_fd = accept_client(socket_fd);
  char buffer[16384];
  recv_client_msg(client_fd, socket_fd, buffer, sizeof(buffer));
  char file_name[1024];
  sscanf(buffer, "GET %s HTTP/1.1\r\n", file_name);
  char abs_file_path[1024];
  strcpy(abs_file_path, dir_path);
  strcat(abs_file_path, file_name);
  char response[16384];
  response[0] = '\0';
  bool file_exists = access(abs_file_path, F_OK) == 0;
  bool file_is_readable = access(abs_file_path, R_OK) == 0;
  if (!file_exists) {
    strcat(response, "HTTP/1.1 404 Not Found\r\n");
  } else if (!file_is_readable) {
    strcat(response, "HTTP/1.1 403 Forbidden\r\n");
  } else {
    strcat(response, "HTTP/1.1 200 OK\r\n");
  }
  size_t content_len;
  if (!file_exists || !file_is_readable) {
    content_len = 0;
  } else {
    struct stat file_st;
    stat(abs_file_path, &file_st);
    content_len = file_st.st_size;
  }
  char content_len_response[1024];
  sprintf(content_len_response, "Content-Length: %ld\r\n", content_len);
  strcat(response, content_len_response);
  strcat(response, "\r\n");
  if (file_exists && file_is_readable) {
    int fd = open(abs_file_path, O_RDONLY);
    char* file_contents = (char*) mmap(0, content_len, PROT_READ, MAP_PRIVATE, fd, 0);
    strncat(response, file_contents, content_len);
    munmap(file_contents, content_len);
  }
  send_msg_to_client(client_fd, socket_fd, response, strlen(response));
  close(client_fd);
}

void sigterm_sigint_handler() {
  sigterm_sigint_flag = 1;
}

int main(int argc, char** argv) {
  int port = atoi(argv[1]);
  const char* path_to_catalog = argv[2];

  sigset_t mask;
  sigfillset(&mask);
  sigdelset(&mask, SIGTERM);
  sigdelset(&mask, SIGINT);
  sigprocmask(SIG_SETMASK, &mask, NULL);

  struct sigaction sigterm_sigint_action = {.sa_handler = &sigterm_sigint_handler};
  sigaction(SIGTERM, &sigterm_sigint_action, NULL);
  sigaction(SIGINT, &sigterm_sigint_action, NULL);
  char server_ip[1024];
  strcpy(server_ip, inet_ntoa(*((struct in_addr**) gethostbyname("localhost")->h_addr_list)[0]));

  char dir_path[1024];
  strcpy(dir_path, path_to_catalog);
  if (dir_path[strlen(dir_path) - 1] != '/') {
    strcat(dir_path, "/");
  }
  int server_fd = create_server(server_ip, port);

  ssize_t listen_status = listen(server_fd, SOMAXCONN);
  if (listen_status < 0) {
    close_socket(server_fd);
  }
  while (sigterm_sigint_flag != 1) {
    process_client(server_fd, server_ip, port, dir_path);
  }
  close(server_fd);
  return 0;
}
