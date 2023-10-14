#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

const int ALPHABET_SIZE = 26;
const int NUM_OF_DIGIT = 10;
const int COMMON_NUM_OF_SYMBOLS = 2 * ALPHABET_SIZE + NUM_OF_DIGIT;
int num_of_bit(int c) {
  int tmp = 0;
  if (c >= '0' && c < '9') {
    tmp = c - '0';
  } else if (c >= 'A' && c < 'Z') {
    tmp = c - 'A' + NUM_OF_DIGIT;
  } else if (c >= 'a' && c < 'z') {
    tmp = c - 'a' + NUM_OF_DIGIT + ALPHABET_SIZE;
  }
  return 61 - tmp;
}

void print_ans(uint64_t result) {
  bool current_char;
  for (int i = 0; i < COMMON_NUM_OF_SYMBOLS; ++i) {
    current_char = (result >> (COMMON_NUM_OF_SYMBOLS - 1 - i)) & 1;
    if (current_char) {
      if (i >= 0 && i < NUM_OF_DIGIT) {
        printf("%c", i + '0');
      } else if (i >= NUM_OF_DIGIT && i < NUM_OF_DIGIT + ALPHABET_SIZE) {
        printf("%c", i + 'A' - NUM_OF_DIGIT);
      } else {
        printf("%c", i + 'a' - NUM_OF_DIGIT - ALPHABET_SIZE);
      }
    }
  }
}

int main() {
  uint64_t result = 0;
  uint64_t current = 0;
  int c = ' ';
  int shift_bit;
  while (c != EOF) {
    c = getchar();
    switch (c) {
      case '|': {
        result |= current;
        current = 0;
        break;
      }
      case '&': {
        result &= current;
        current = 0;
        break;
      }
      case '~': {
        result = ~result;
        current = 0;
        break;
      }
      case '^': {
        result ^= current;
        current = 0;
        break;
      }
      default: {
        shift_bit = num_of_bit(c);
        if (!((current >> shift_bit) & 1)) {
          current += (uint64_t) 1 << shift_bit;
        }
        break;
      }
    }
  }
  print_ans(result);
}
