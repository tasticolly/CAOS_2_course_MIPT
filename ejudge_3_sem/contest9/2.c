#include <stdio.h>
void processing_one_point(char* path) {
  int index_left = 0;
  int index_right = 0;
  while (*(path + index_right) != '\0') {
    if (*(path + index_right) == '/' && *(path + index_right + 1) == '.' && *(path + index_right + 2) == '/') {
      ++index_right;
      ++index_right;
    }
    *(path + index_left) = *(path + index_right);
    ++index_right;
    ++index_left;
  }
  *(path + index_left) = '\0';
}

void processing_two_point(char* path) {
  int index_left = 0;
  int index_right = 0;
  while (*(path + index_right) != '\0') {
    if (*(path + index_right) == '/' && *(path + index_right + 1) == '.' && *(path + index_right + 2) == '.'
        && *(path + index_right + 3) == '/') {
      index_right += 3;
      --index_left;
      while (*(path + index_left) != '/' && index_right != 0) {
        --index_left;
      }
    }
    *(path + index_left) = *(path + index_right);
    ++index_right;
    ++index_left;
  }
  *(path + index_left) = '\0';
}

void processing_slash(char* path) {
  int index_left = 0;
  int index_right = 0;
  while (*(path + index_right) != '\0') {
    while (*(path + index_right) == '/' && *(path + index_right + 1) == '/') {
      ++index_right;
    }
    *(path + index_left) = *(path + index_right);
    ++index_right;
    ++index_left;
  }
  *(path + index_left) = '\0';
}
void normalize_path(char* path) {
  processing_one_point(path);
  processing_slash(path);
  processing_two_point(path);
}

//char path[1000] = "./zhopa/../vlad/./idi///nahui//";


int main() {
  char path[1000] = "./zhopa/../vlad/./idi///nahui//";
  char path_file[1000] = "/zhopa/../vlad/./sosi///../../.././nahui//";

  normalize_path(path);
  normalize_path(path_file);
  normalize_path(path);
  printf("%s\n", path);
  printf("%s", path_file);
}