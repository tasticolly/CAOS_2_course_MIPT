#include <stdint.h>
#include <stdio.h>
extern int solve(int64_t A, int B, int C, int D);

int main() {
  int64_t A = 1;
  int B = -55;
  int C = 1000;
  int D = -6000;
  printf("%d",solve(A,B,C,D));
}

