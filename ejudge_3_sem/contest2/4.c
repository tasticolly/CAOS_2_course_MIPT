#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint8_t type_of_block(uint8_t block) {
  uint8_t ans;
  if (!((block >> 7) & 1)) { ans = 0; } //0xxxxxxx
  else if (block >> 5 == 6) { ans = 1; } //110xxxxx
  else if (block >> 4 == 14) { ans = 2; } //1110xxxx
  else if (block >> 3 == 30) { ans = 3; } //11110xxx
  else if (block >> 6 == 2) { ans = 4; }  //10xxxxxx
  else { ans = 5; }
  return ans;
}

int main() {
  int c = getchar();
  int ascii_count = 0;
  int not_ascii_count = 0;

  uint8_t num_of_blocks = 0;
  uint8_t block_type = 0;

  while (c != -1) {
    block_type = type_of_block((uint8_t) c);

    switch (block_type) {
      case 0: {
        if (num_of_blocks == 0) {
          ++ascii_count;
        }
        break;
      }

      case 1: {
        num_of_blocks = 1;
        break;
      }
      case 2: {
        num_of_blocks = 2;
        break;
      }
      case 3: {
        num_of_blocks = 3;
        break;
      }
      case 4: {
        if (num_of_blocks > 0) {
          --num_of_blocks;
          if (num_of_blocks == 0) {
            ++not_ascii_count;
          }
        }
        break;
      }
      default: {
        printf("%d %d\n", ascii_count, not_ascii_count);
        exit(1);
      }
    }
    c = getchar();
  }

  printf("%d %d\n", ascii_count, not_ascii_count);
}