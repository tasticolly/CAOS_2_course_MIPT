#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
const int BUFFER_SIZE_READ = 16;
int main(int argc, char** argv) {

  int op1;
  int op2;
  int op3;

  int read_count;
  int ret;
  if (argc < 4) {
    return 0;
  }

  char* first = argv[1];
  char* second = argv[2];
  char* third = argv[3];

  char buff[BUFFER_SIZE_READ];
  char numbers[BUFFER_SIZE_READ];
  char other_symbols[BUFFER_SIZE_READ];
  int number_point = 0;
  int other_point = 0;
  op1 = open(first, O_RDONLY, 0640);

  if (op1 == -1) {
    ret = 1;
    goto Exit;
  }
  op2 = open(second, O_WRONLY | O_CREAT, 0640);
  op3 = open(third, O_WRONLY | O_CREAT, 0640);

  if (op2 == -1 || op3 == -1) {
    ret = 2;
    goto Exit;
  }

  int write_count = 0;
  read_count = read(op1, buff, BUFFER_SIZE_READ);
  if (read_count == -1) {
    ret = 3;
    goto Exit;
  }
  while (read_count > 0) {
    for (int i = 0; i < read_count; ++i) {
      if (isdigit(buff[i])) {
        numbers[number_point++] = buff[i];

      } else {
        other_symbols[other_point++] = buff[i];
      }
    }

    write_count = write(op2, numbers, number_point);
    number_point = 0;
    if (write_count < 0) {
      ret = 3;
      goto Exit;
    }
    write_count = write(op3, other_symbols, other_point);
    other_point = 0;
    if (write_count < 0) {
      ret = 3;
      goto Exit;
    }

    read_count = read(op1, buff, BUFFER_SIZE_READ);
    if (read_count == -1) {
      ret = 3;
      goto Exit;
    }
  }

  Exit:
  close(op1);
  close(op2);
  close(op3);
  
  return ret;

}
