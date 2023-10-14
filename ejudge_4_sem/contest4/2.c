#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>

const int BUFFER_SIZE = 4096;
sig_atomic_t fd = 0;

void file_handler(int num_of_sig) {
  fd = num_of_sig - SIGRTMIN;
}

void close_files(FILE** files, int num) {
  for (int i = 1; i < num; ++i) {
    fclose(files[i]);
  }
}

int main(int argc, char** argv) {
  sigset_t mask;
  sigfillset(&mask);
  struct sigaction actions[argc];
  for (int signal = SIGRTMIN; signal <= SIGRTMAX; ++signal) {
    sigdelset(&mask, signal);
  }
  sigprocmask(SIG_SETMASK, &mask, NULL);
  FILE* files[argc];

  for (int i = 0; i < argc; ++i) {
    if (i >= 1) {
      files[i] = fopen(argv[i], "r");
    }
    actions[i].sa_handler = &file_handler;
    sigaction(SIGRTMIN + i, actions + i, NULL);
  }

  char buffer[BUFFER_SIZE];
  while (true) {
    pause();
    if (fd == 0) {
      close_files(files, argc);
      return 0;
    }
    fgets(buffer, BUFFER_SIZE, files[fd]);
    printf("%s", buffer);
    fflush(stdout);
  }
}