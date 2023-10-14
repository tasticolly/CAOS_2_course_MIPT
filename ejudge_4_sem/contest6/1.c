#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <netinet/ip_icmp.h>
#include <memory.h>

volatile sig_atomic_t is_stopped = false;

int send_echo(int icmp_socket, const struct sockaddr_in* addr);
void recv_echo(int icmp_socket, struct icmp* buffer);

void close_socket(int socket) {
  shutdown(socket, SHUT_RDWR);
  close(socket);
  exit(1);
}

uint16_t check_sum(void* buffer, int len) {
  uint16_t result = 0;
  uint16_t* chunk = buffer;
  while (len > 1) {
    result += *chunk++;
    len -= 2;
  }
  if (len == 1) {
    result += *(unsigned char*) chunk;
  }
  result = (result >> 16) + (result & 0xFFFF);
  result += (result >> 16);
  return ~result;
}

void sigalarm_handler() {
  is_stopped = true;
}

int result(void* buffer, int len, int id) {
  int header_len = ((struct ip*) buffer)->ip_hl * 4;
  struct icmp* icmp_packet = (struct icmp*) (buffer + header_len);
  uint16_t checksum = icmp_packet->icmp_cksum;
  icmp_packet->icmp_cksum = 0;
  return icmp_packet->icmp_id == id && icmp_packet->icmp_type == ICMP_ECHOREPLY
      && checksum == check_sum((uint16_t*) icmp_packet, len - header_len);
}

int ping(int icmp_socket, struct sockaddr_in* to_addr_ptr) {
  struct icmp received_pk;
  int id = send_echo(icmp_socket, to_addr_ptr);
  recv_echo(icmp_socket, &received_pk);
  return result(&received_pk, sizeof(struct icmp), id);
}

int prepare_icmp(struct icmp* packet) {
  memset(packet, 0, sizeof(struct icmp));
  const short random_id = rand() % 100;
  packet->icmp_type = ICMP_ECHO;
  packet->icmp_seq = 0;
  packet->icmp_id = random_id;
  packet->icmp_cksum = check_sum((uint16_t*) packet, sizeof(struct icmp));
  return random_id;
}

struct sockaddr_in server_addr(const char* server_ip_str) {
  struct in_addr server_ip_addr;
  inet_aton(server_ip_str, &server_ip_addr);
  struct sockaddr_in server_addr = {
      .sin_family = AF_INET,
      .sin_addr = server_ip_addr};
  return server_addr;
}

int send_echo(int icmp_socket, const struct sockaddr_in* to_addr) {
  struct icmp icmp_pk;
  int id = prepare_icmp(&icmp_pk);

  ssize_t send_status = sendto(
      icmp_socket,
      &icmp_pk,
      sizeof(icmp_pk),
      0,
      (const struct sockaddr*) to_addr,
      sizeof(struct sockaddr_in));
  if (send_status < 0) {
    close_socket(icmp_socket);
  }
  return id;
}

void recv_echo(int icmp_socket, struct icmp* buffer) {
  ssize_t recv_status = recvfrom(
      icmp_socket,
      buffer,
      sizeof(struct icmp),
      /*flags=*/0,
      /*address=*/NULL,
      /*address_len=*/0);
  if (recv_status < 0) {
    close_socket(icmp_socket);
  }
}

int main(int argc, char* argv[]) {
  const char* server_ip_str = argv[1];
  int timeout = atoi(argv[2]);
  int interval = atoi(argv[3]);
  struct sigaction sigalarm_action = {.sa_sigaction = &sigalarm_handler};
  sigalarm_action.sa_flags = SA_RESTART;
  sigaction(SIGALRM, &sigalarm_action, NULL);

  int icmp_socket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  struct sockaddr_in to_addr = server_addr(server_ip_str);
  int number_of_received = 0;

  alarm(timeout);

  while (!is_stopped) {
    number_of_received += ping(icmp_socket, &to_addr);
    usleep(interval);
  }
  printf("%d", number_of_received);

  shutdown(icmp_socket, SHUT_RDWR);
  close(icmp_socket);
}