#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
void fill_matrix(int** matrix, int size) {
  int border = size - 1;
  int current_num = size + 1;
  int row = size - 1;
  int line = 0;
  while (current_num < size * size + 1) {
    for (int i = 0; i < border; ++i) {
      matrix[++line][row] = current_num++;
    }
    for (int i = 0; i < border; ++i) {
      matrix[line][--row] = current_num++;
    }
    for (int i = 0; i < border - 1; ++i) {
      matrix[--line][row] = current_num++;
    }
    for (int i = 0; i < border - 1; ++i) {
      matrix[line][++row] = current_num++;
    }
    border -= 2;
  }
}

int** create_matrix(int size) {
  int** matrix = calloc(size, sizeof(int*));
  for (int i = 0; i < size; ++i) {
    matrix[i] = calloc(size, sizeof(int));
    matrix[0][i] = i + 1;
  }
  return matrix;
}

void delete_matrix(int** matrix, int size) {
  for (int i = 0; i < size; ++i) {
    free(matrix[i]);
  }
  free(matrix);
}

int main(int argc, char** argv) {
  int ret = 0;
  char* file_name = argv[1];
  int size_of_cell = strtol(argv[3], NULL, 10);
  int size_of_matrix = strtol(argv[2], NULL, 10);
  int** matrix = create_matrix(size_of_matrix);
  fill_matrix(matrix, size_of_matrix);
  int input = open(file_name, O_RDWR | O_CREAT, 0664);
  lseek(input, size_of_matrix * size_of_matrix * size_of_cell + size_of_matrix - 2, SEEK_SET);
  write(input, "\0", 1);
  char* ptr = mmap(
      NULL, size_of_matrix * size_of_matrix * size_of_cell + size_of_matrix - 1, PROT_READ | PROT_WRITE, MAP_SHARED, input, 0);
  if (MAP_FAILED == ptr) {
    return 1;
  }

  

  for (int i = 0; i < size_of_matrix; ++i) {
    if (i != size_of_matrix - 1) {
      ptr[(i + 1) * size_of_matrix * size_of_cell + i] = '\n';
    }


    for (int j = 0; j < size_of_matrix; ++j) {
      char num[size_of_cell];
      if (snprintf(num, size_of_cell, "%d", matrix[i][j]) < 0) {
        break;
      }
      int len_of_number = strlen(num);

      int index = size_of_matrix * size_of_cell * i + size_of_cell * j + i;
      for (int m = 0; m < size_of_cell - len_of_number; m++) {
        ptr[index] = ' ';
        index += 1;
      }
      for (int m = 0; m < len_of_number; m++) {
        ptr[index] = num[m];
        index += 1;
      }
    }

  }
  munmap(ptr, size_of_matrix * size_of_matrix * size_of_cell + size_of_matrix);


  close(input);
  delete_matrix(matrix, size_of_matrix);
  return ret;
}
