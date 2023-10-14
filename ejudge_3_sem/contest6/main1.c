#include <stdio.h>

extern double avx_sin(double d);
int main(){
  double d = 3.14159;
  printf("%f", avx_sin(d));
  return 0;
}