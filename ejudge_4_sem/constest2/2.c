#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h>
const int stack_min = 16384;

typedef struct Item {
  _Atomic (struct Item*) prev;
  int64_t value;
} item_t;

typedef struct {
  item_t** tail;
  int64_t index;
  uint32_t num_of_numbers;
} arguments;

void atomic_push(int64_t new_value, item_t** tail) {
  item_t* new_node = (item_t*) malloc(sizeof(item_t));
  new_node->value = new_value;
  while (true) {
    item_t** old_tail = tail;
    new_node->prev = *old_tail;
    if (atomic_compare_exchange_strong(tail, old_tail, new_node)) {
      printf("%lu ", new_value);
      return;
    }
  }
}

void* routine(void* arg) {
  arguments* args = (arguments*) arg;
  for (int64_t i = args->index * args->num_of_numbers; i < (args->index + 1) * args->num_of_numbers; ++i) {
    atomic_push(i, args->tail);
    sched_yield();
  }
  return NULL;
}

item_t* init_list() {
  item_t* head = (item_t*) malloc(sizeof(item_t));
  head->prev = NULL;
  return head;
}

void clear_list(item_t* tail) {
  item_t* prev;
  while (tail != NULL) {
    prev = tail;
    tail = tail->prev;
    free(prev);
  }
}
int main(int argc, char** argv) {
  int64_t num_of_threads = atoi(argv[1]);
  int64_t k = atoi(argv[2]);

  pthread_attr_t attributes;
  pthread_attr_init(&attributes);
  pthread_attr_setstacksize(&attributes, stack_min);
  pthread_t* array_of_pthread = (pthread_t*) malloc(sizeof(pthread_t) * num_of_threads);
  arguments* array_of_args = (arguments*) malloc(sizeof(arguments) * num_of_threads);
  item_t* head = init_list();
  for (int i = 0; i < num_of_threads; ++i) {
    array_of_args[i].tail = &head;
    array_of_args[i].index = i;
    array_of_args[i].num_of_numbers = k;
    pthread_create(array_of_pthread + i, &attributes, &routine, array_of_args + i);
  }
  for (int i = 0; i < num_of_threads; ++i) {
    pthread_join(array_of_pthread[i], NULL);
  }
  free(array_of_pthread);
  pthread_attr_destroy(&attributes);
  clear_list(*array_of_args[0].tail);
  free(array_of_args);
}
