#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void insert_number(int number, char* ptr, int pos_line, int pos_row, int size_of_matrix, int size_of_cell) {
  char num[size_of_cell];
  if (snprintf(num, size_of_cell, "%d", number) < 0) {
    return;
  }
  int len_of_number = strlen(num);

  int index = size_of_matrix * size_of_cell * pos_line + size_of_cell * pos_row + pos_line;
  for (int m = 0; m < size_of_cell - len_of_number; m++) {
    ptr[index] = ' ';
    index += 1;
  }
  for (int m = 0; m < len_of_number; m++) {
    ptr[index] = num[m];
    index += 1;
  }
}

void fill_first_line(char* ptr, int size_of_matrix, int size_of_cell) {
  for (int i = 0; i < size_of_matrix; ++i) {
    insert_number(i + 1, ptr, 0, i, size_of_matrix, size_of_cell);
    ptr[(i + 1) * size_of_matrix * size_of_cell + i] = '\n';
  }
}

void fill_other_part_of_matrix(char* ptr, int size_of_matrix, int size_of_cell) {
  int border = size_of_matrix - 1;
  int current_num = size_of_matrix + 1;
  int row = size_of_matrix - 1;
  int line = 0;
  while (current_num < size_of_matrix * size_of_matrix + 1) {
    for (int i = 0; i < border; ++i) {
      insert_number(current_num++, ptr, ++line, row, size_of_matrix, size_of_cell);
    }
    for (int i = 0; i < border; ++i) {
      insert_number(current_num++, ptr, line, --row, size_of_matrix, size_of_cell);
    }
    for (int i = 0; i < border - 1; ++i) {
      insert_number(current_num++, ptr, --line, row, size_of_matrix, size_of_cell);
    }
    for (int i = 0; i < border - 1; ++i) {
      insert_number(current_num++, ptr, line, ++row, size_of_matrix, size_of_cell);
    }
    border -= 2;
  }
}

int main(int argc, char** argv) {
  int ret = 0;
  char* file_name = argv[1];
  int size_of_cell = strtol(argv[3], NULL, 10);
  int size_of_matrix = strtol(argv[2], NULL, 10);

  int input = open(file_name, O_RDWR | O_CREAT, 0664);
  lseek(input, size_of_matrix * size_of_matrix * size_of_cell + size_of_matrix - 2, SEEK_SET);
  write(input, "\0", 1);
  char* ptr = mmap(
      NULL,
      size_of_matrix * size_of_matrix * size_of_cell + size_of_matrix - 1,
      PROT_READ | PROT_WRITE,
      MAP_SHARED,
      input,
      0);
  if (MAP_FAILED == ptr) {
    return 1;
  }

  fill_first_line(ptr, size_of_matrix, size_of_cell);
  fill_other_part_of_matrix(ptr, size_of_matrix, size_of_cell);
  munmap(ptr, size_of_matrix * size_of_matrix * size_of_cell + size_of_matrix);

  close(input);
  return ret;
}
