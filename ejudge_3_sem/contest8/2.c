#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
void merge(int left, int middle, int right, int file, int part1, int part2);
void mergeSort(int left, int right, int file, int part1, int part2) {
  if (right - left == 1) {
    return;
  }
  int middle = (left + right) / 2;
  mergeSort(left, middle, file, part1, part2);
  mergeSort(middle, right, file, part1, part2);
  for (int i = left; i < middle; ++i) {
    lseek(file, sizeof(int) * i, SEEK_SET);
    int elem;
    ssize_t read_count = read(file, &elem, sizeof(elem));
    lseek(part1, sizeof(int) * (i - left), SEEK_SET);
    ssize_t write_count = write(part1, &elem, sizeof(elem));
  }
  for (int i = middle; i < right; ++i) {
    lseek(file, sizeof(int) * i, SEEK_SET);
    int elem;
    ssize_t read_count = read(file, &elem, sizeof(elem));
    lseek(part2, sizeof(int) * (i - middle), SEEK_SET);
    ssize_t write_count = write(part2, &elem, sizeof(elem));
  }
  merge(left, middle, right, file, part1, part2);
}

void merge(int left, int middle, int right, int file, int part1, int part2) {
  int iterator1 = 0;
  int iterator2 = 0;

  while (iterator1 < middle - left && iterator2 < right - middle) {
    int first, second;
    int seek_count = lseek(part1, sizeof(int) * iterator1, SEEK_SET);
    int read_count = read(part1, &first, sizeof(first));
    lseek(part2, sizeof(int) * iterator2, SEEK_SET);
    read(part2, &second, sizeof(second));
    if (first < second) {
      lseek(file, sizeof(int) * (left + iterator1 + iterator2), SEEK_SET);
      write(file, &first, sizeof(first));
      ++iterator1;
    } else {
      lseek(file, sizeof(int) * (left + iterator1 + iterator2), SEEK_SET);
      write(file, &second, sizeof(second));
      ++iterator2;
    }
  }

  while (iterator1 < middle - left) {
    int first;
    lseek(part1, sizeof(int) * iterator1, SEEK_SET);
    read(part1, &first, sizeof(first));
    lseek(file, sizeof(int) * (left + iterator1 + iterator2), SEEK_SET);
    write(file, &first, sizeof(first));
    ++iterator1;
  }
  while (iterator2 < right - middle) {
    int second;
    lseek(part2, sizeof(int) * iterator2, SEEK_SET);
    read(part2, &second, sizeof(second));
    lseek(file, sizeof(int) * (left + iterator1 + iterator2), SEEK_SET);
    write(file, &second, sizeof(second));
    ++iterator2;
  }
}

int main(int argc, char* argv[]) {

  if (argc != 2)
    return 0;
  char* file_path = argv[1];
  int ret = 0;
  int file = open(file_path, O_RDWR, 0640);
  int part1 = open("first.txt", O_RDWR | O_CREAT, 0640);
  int part2 = open("second.txt", O_RDWR | O_CREAT, 0640);
  if (file < 0 || part1 < 0 || part2 < 0) {
    ret = 1;
    goto Exit;
  }
  int size_of_file = lseek(file, 0L, SEEK_END);
  if (!size_of_file){
    return 0;
  }
  mergeSort(0, size_of_file / 4, file, part1, part2);
  Exit:
  close(file);
  close(part1);
  close(part2);
  return ret;
}