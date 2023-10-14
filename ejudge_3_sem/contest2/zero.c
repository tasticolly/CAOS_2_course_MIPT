#include <stdint.h>
#include <stdio.h>
typedef int ITYPE;
void sum(ITYPE first, ITYPE second, ITYPE* res, int* CF) {
  ITYPE bit_xor = first ^ second;
  ITYPE bit_and = first & second;
  ITYPE tmp;
  while (bit_and != 0) {
    bit_and <<= 1;
    tmp = bit_xor;
    bit_xor ^= bit_and;
    bit_and &= tmp;
  }
  *res = bit_xor;
  if (first > bit_xor || second > bit_xor) {
    *CF = 1;
  } else {
    *CF = 0;
  }
}

int main() {
    ITYPE a;
    ITYPE b;
    scanf("%d", &a);
    scanf("%d", &b);
    int c = 0;
    int* CF = &c;
    ITYPE* res;
    res[0] = a;
    sum(a, b, res, CF);
    printf("%d\n", *res);
    printf("%d", *CF);
}
