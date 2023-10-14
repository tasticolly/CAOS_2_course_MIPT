#include <stdint.h>

ITYPE sum(ITYPE first, ITYPE second) {
  ITYPE bit_xor = first ^ second;
  ITYPE bit_and = first & second;
  ITYPE tmp;

  while (bit_and != 0) {
    bit_and <<= 1;
    tmp = bit_xor;
    bit_xor ^= bit_and;
    bit_and &= tmp;
  }
  return bit_xor;
}

void mul(ITYPE first, ITYPE second, ITYPE* res, int* CF) {
  ITYPE result = 0;
  ITYPE tmp;

  while (second) {
    if (second & 1) {
      tmp = result;
      result = sum(tmp, first);

      if (tmp > result || first > result) {
        *CF = 1;
        return;
      }
    }
    tmp = first;
    first <<= 1;

    if (first < tmp) {
      *CF = 1;
      return;
    }
    second >>= 1;
  }
  *CF = 0;
  *res = result;
}