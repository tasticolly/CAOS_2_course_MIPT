#include <threads.h>
#include <bits/pthreadtypes.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define PTHREAD_STACK_MIN 16384
void* part_sum() {
  long long res_sum = 0;
  long long elem;
  while (scanf("%lld", &elem) != EOF) {
    res_sum += elem;
  }
  return (void*) res_sum;
}

int main(int argc, char** argv) {
  size_t num_of_threads = atoi(argv[1]);
  pthread_t* array_of_threads = (pthread_t*) calloc(num_of_threads, sizeof(pthread_t));

  pthread_attr_t attributes;
  pthread_attr_init(&attributes);
  pthread_attr_setstacksize(&attributes, PTHREAD_STACK_MIN);

  for (int i = 0; i < num_of_threads; ++i) {
    pthread_create(&array_of_threads[i], &attributes, &part_sum, NULL);
  }
  pthread_attr_destroy(&attributes);
  long long sum = 0;
  void* temp_sum;
  for (int i = 0; i < num_of_threads; ++i) {
    pthread_join(array_of_threads[i], &temp_sum);
    sum += (long long) temp_sum;
  }
  free(array_of_threads);
  printf("%lld", sum);
  return 0;
}
