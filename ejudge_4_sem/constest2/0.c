#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
const int stack_min = 16384;

typedef struct {
  void* node_array;
  double value;
  size_t num_of_iterations;
  size_t my_index;
  pthread_mutex_t* mutex_p;
  size_t size;
} node;

void* routine(void* arg) {
  node* current = (node*) arg;
  node* list_of_values = (node*) current->node_array;
  for (int i = 0; i < current->num_of_iterations; ++i) {
    pthread_mutex_lock(current->mutex_p);
    list_of_values[current->my_index].value += 1.0;
    list_of_values[(current->my_index + current->size - 1) % current->size].value += 0.99;
    list_of_values[(current->my_index + 1) % current->size].value += 1.01;
    pthread_mutex_unlock(current->mutex_p);
  }
  return NULL;
}

int main(int argc, char** argv) {
  size_t num_of_iterations = atoi(argv[1]);
  size_t num_of_threads = atoi(argv[2]);
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);
  pthread_t* thread_array = (pthread_t*) malloc(num_of_threads * sizeof(pthread_t));
  node* node_array = (node*) malloc(num_of_threads * sizeof(node));
  for (int i = 0; i < num_of_threads; ++i) {
    node_array[i].my_index = i;
    node_array[i].mutex_p = &mutex;
    node_array[i].size = num_of_threads;
    node_array[i].num_of_iterations = num_of_iterations;
    node_array[i].value = 0;
    node_array[i].node_array = (void*) node_array;
  }

  pthread_attr_t attributes;
  pthread_attr_init(&attributes);
  pthread_attr_setstacksize(&attributes, stack_min);

  for (int i = 0; i < num_of_threads; ++i) {
    pthread_create(thread_array + i, &attributes, &routine, node_array + i);
  }

  for (int i = 0; i < num_of_threads; ++i) {
    pthread_join(thread_array[i], NULL);
  }
  for (int i = 0; i < num_of_threads; ++i) {
    printf("%.10g ", node_array[i].value);
  }
  pthread_mutex_destroy(&mutex);
  pthread_attr_destroy(&attributes);
  free(node_array);
  free(thread_array);
}
