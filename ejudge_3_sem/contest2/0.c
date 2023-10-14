#include <stdint.h>
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

  *CF = (first > bit_xor || second > bit_xor) ? 1 : 0;
}