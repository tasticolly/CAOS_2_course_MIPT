#include <error.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int64_t getFileSize(const char* file_name){
  int64_t _file_size = 0;
  struct stat _fileStatbuff;
  int fd = open(file_name, O_RDONLY);
  if(fd == -1){
    _file_size = -1;
  }
  else{
    if ((fstat(fd, &_fileStatbuff) != 0) || (!S_ISREG(_fileStatbuff.st_mode))) {
      _file_size = -1;
    }
    else{
      _file_size = _fileStatbuff.st_size;
    }
    close(fd);
  }
  return _file_size;
}

int main(){
  const char* request_fmt = "/home/admin1/Desktop/concurr";
  printf("%lu", getFileSize(request_fmt));
}
