#include <sys/socket.h>
#include <bits/pthreadtypes.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

const int flag = 200;
const int upper_border = 100;
const int lower_border = 0;

struct arg {
  int socket_fd;
  int diff;
};

void* make_diff(void* argument) {
  int number;
  int socket_fd = ((struct arg*) argument)->socket_fd;
  int difference = ((struct arg*) argument)->diff;

  read((int) (long long) socket_fd, &number, sizeof(number));
  while (number <= upper_border && number != lower_border) {
    number += difference;
    if (!(number <= upper_border && number != lower_border)) {
      if (number != flag + difference) {
        printf("%d ", number);
      }
      number = flag;
      write((int) (long long) socket_fd, &number, sizeof(number));
      break;
    }
    printf("%d ", number);
    write((int) (long long) socket_fd, &number, sizeof(number));
    read((int) (long long) socket_fd, &number, sizeof(number));
  }
  return NULL;
}

int main(int argc, char** argv) {
  int fd[2];
  socketpair(PF_LOCAL, SOCK_STREAM, 0, fd);
  pthread_t array_of_threads[2];
  struct arg thread_args[2];
  thread_args[0].socket_fd = fd[0];
  thread_args[0].diff = -3;

  thread_args[1].socket_fd = fd[1];
  thread_args[1].diff = 5;

  int number = atoi(argv[1]);
  write((int) (long long) fd[1], &number, sizeof(number));
  pthread_create(&array_of_threads[0], NULL, &make_diff, (void*) &thread_args[0]);
  pthread_create(&array_of_threads[1], NULL, &make_diff, (void*) (long long) &thread_args[1]);
  pthread_join(array_of_threads[0], NULL);
  pthread_join(array_of_threads[1], NULL);
  close(fd[0]);
  close(fd[1]);
  return 0;
}