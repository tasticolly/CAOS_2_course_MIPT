#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <error.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

void close_socket(int fd) {
  shutdown(fd, SHUT_RDWR);
  close(fd);
}

int connect_to_server(const char* ip_address, int port_number) {
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct in_addr server_ip;
  inet_aton(ip_address, &server_ip);
  struct sockaddr_in server_addr = {.sin_family = AF_INET, .sin_port = htons(port_number), .sin_addr = server_ip};

  int connection_status = connect(socket_fd, (struct sockaddr*) &server_addr, sizeof(server_addr));
  if (connection_status != 0) {
    close_socket(socket_fd);
    error(EXIT_FAILURE, errno, "Could not connect to %s:%d", ip_address, port_number);
  }
  return socket_fd;
}

int main(int argc, char** argv) {
  const char* ip_adress = argv[1];
  int port = atoi(argv[2]);
  int server_fd = connect_to_server(ip_adress, port);
  int num;
  while (EOF != scanf("%d", &num)) {
    ssize_t send_status = send(server_fd, &num, sizeof(int), 0);
    if (send_status == -1) {
      printf("Can't send");
      return 1;
    }
    int received_number;
    ssize_t rec_status = recv(server_fd, &received_number, sizeof(int), 0);
    if (rec_status < 0) {
      printf("Can't receive");
      return 2;
    } else if (rec_status == 0) {
      break;
    }
    printf("%d\n", received_number);
  }
  close_socket(server_fd);
}
