#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>


extern void sum(size_t N, const int *A, const int *B, int *R);

int main(){
    size_t N = 10;
    int* A = malloc(N * sizeof(int));
    int* B = malloc(N * sizeof(int));
    int* R = malloc(N * sizeof(int));

    // printf("%lu %lu %lu",(size_t) A, (size_t) B, (size_t) R);
    for (int i = 0; i < N; ++i){
        A[i]= i;
        B[i] = -2;
    }
    sum(N, A, B, R);
    for (int i = 0; i < N; ++i){
        printf("%d ",R[i]);
    }
}