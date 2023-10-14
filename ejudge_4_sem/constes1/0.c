#include <threads.h>
#include <bits/pthreadtypes.h>
#include <stdio.h>
#include <pthread.h>

void* reverse_seq() {
  int seq_element;
  while (scanf("%d", &seq_element) != EOF) {
    pthread_t new_thread;
    pthread_create(&new_thread, NULL, &reverse_seq ,NULL);
    pthread_join(new_thread,NULL);
    printf("%d ", seq_element);
  }
}

int main() {
  pthread_t root_thread;
  pthread_create(&root_thread, NULL, &reverse_seq, NULL);
  pthread_join(root_thread,NULL);
}
