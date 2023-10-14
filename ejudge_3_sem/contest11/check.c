#include "stdio.h"
#include <pthread.h>
#include <unistd.h>
pthread_t a;
pthread_t b;

void* thread_func_a(void*) {
  sleep(1);
  pthread_join(b, NULL);
}

void* thread_func_b(void*) {
  sleep(1);
  pthread_join(a, NULL);
}

int main() {
  pthread_create(&a, NULL, thread_func_a, 0);
  pthread_create(&b, NULL, thread_func_b, 0);
}