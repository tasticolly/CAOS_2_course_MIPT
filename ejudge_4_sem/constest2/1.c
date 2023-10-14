#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
const int stack_min = 16384;

typedef struct {
  uint64_t lower_border;
  uint64_t upper_border;
  uint32_t primary_num;
  pthread_mutex_t* mutex_p;
  uint64_t current_primary;
  uint32_t count_of_prime;
  bool is_printed;
  pthread_cond_t* condition;
} arguments;

bool is_prime(uint64_t num) {
  if (num <= 1) {
    return false;
  }
  int del = 2;
  while (del * del <= num) {
    if (num % del == 0) {
      return false;
    }
    ++del;
  }
  return true;
}

void* routine(void* arg) {
  arguments* current = (arguments*) arg;
  pthread_mutex_lock(current->mutex_p);
  for (uint64_t i = current->lower_border; i <= current->upper_border && current->count_of_prime < current->primary_num;
       ++i) {
    if (is_prime(i)) {
      current->is_printed = false;
      ++current->count_of_prime;
      current->current_primary = i;
      pthread_cond_signal(current->condition);
      while (!current->is_printed) {
        pthread_cond_wait(current->condition, current->mutex_p);
      }
    }
  }
  pthread_mutex_unlock(current->mutex_p);
  return NULL;
}

int main(int argc, char** argv) {
  uint64_t lower_border = atoi(argv[1]);
  uint64_t upper_border = atoi(argv[2]);
  uint32_t primary_num = atoi(argv[3]);

  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_t condition;
  pthread_cond_init(&condition, NULL);

  arguments args = {lower_border, upper_border, primary_num, &mutex, 0, 0, true, &condition};

  pthread_attr_t attributes;
  pthread_attr_init(&attributes);
  pthread_attr_setstacksize(&attributes, stack_min);
  pthread_t pthread;
  pthread_create(&pthread, &attributes, &routine, &args);


  pthread_mutex_lock(&mutex);
  while (args.count_of_prime < primary_num) {
    while (args.is_printed) {
      pthread_cond_wait(&condition, &mutex);
    }
    printf("%lu ", args.current_primary);
    args.is_printed = true;
    pthread_cond_signal(&condition);
  }
  pthread_mutex_unlock(&mutex);

  pthread_join(pthread, NULL);
  pthread_attr_destroy(&attributes);
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&condition);
}
