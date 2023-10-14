#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>



const size_t MAX_PENDING_EVENTS = 65536;
volatile sig_atomic_t finished = false;

void sigterm_handler(int var) {
  finished = true;
}

static void make_non_blocking(int socket_fd) {
  int new_flags = (fcntl(socket_fd, F_GETFL) | O_NONBLOCK);
  fcntl(socket_fd, F_SETFL, new_flags);
}

void create_event_data(int fd, int epoll_fd) {
  struct epoll_event event_ready_read;
  event_ready_read.events = EPOLLIN;
  event_ready_read.data.fd = fd;
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event_ready_read);
}

void process_client(struct epoll_event* event) {
  char buffer[4096];
  int now_read;
  while ((now_read = read(event->data.fd, buffer, sizeof(buffer))) > 0) {
    for (int j = 0; j < now_read; ++j) {
      buffer[j] = toupper(buffer[j]);
    }
    write(event->data.fd, buffer, now_read);
  }
  shutdown(event->data.fd, SHUT_RDWR);
  close(event->data.fd);
}

void accept_client(int socket_fd, int epoll_fd) {
  struct sockaddr client_adress;
  unsigned int adress_length = sizeof(struct sockaddr);
  int client_fd = accept(socket_fd, &client_adress, &adress_length);
  make_non_blocking(client_fd);
  create_event_data(client_fd, epoll_fd);
}

int main(int argc, char** argv) {
  const char* server_port = argv[1];

  struct sigaction sigterm_signal = {
      .sa_handler = &sigterm_handler,
      .sa_flags = SA_RESTART
  };

  sigaction(SIGTERM, &sigterm_signal, NULL);
  struct sockaddr_in server_address = {
      .sin_family = AF_INET,
      .sin_port = htons(atoi(server_port))
  };
  inet_aton("127.0.0.1", &server_address.sin_addr);

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  make_non_blocking(socket_fd);

  if (socket_fd == -1) {
    close(socket_fd);
    exit(1);
  }

  if (bind(socket_fd, (struct sockaddr*) (&server_address), sizeof(server_address)) == -1) {
    close(socket_fd);
    exit(2);
  }

  if (listen(socket_fd, SOMAXCONN) == -1) {
    close(socket_fd);
    exit(3);
  }

  int epoll_fd = epoll_create1(0);
  create_event_data(socket_fd, epoll_fd);
  struct epoll_event events[MAX_PENDING_EVENTS];
  while (!finished) {
    int n = epoll_wait(epoll_fd, events, MAX_PENDING_EVENTS, -1);
    for (int i = 0; i < n; ++i) {
      if (events[i].data.fd != socket_fd) {
        process_client(events + i);
      } else {
        accept_client(socket_fd, epoll_fd);
      }
    }
  }
  close(socket_fd);
  close(epoll_fd);
  return 0;
}