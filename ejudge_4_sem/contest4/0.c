#include <stdio.h>
#include <signal.h>
#include <unistd.h>

sig_atomic_t sigint_count = 0;
sig_atomic_t sigterm_count = 0;

void sigint_handler() {
  ++sigint_count;
}
void sigterm_handler() {
  ++sigterm_count;
}
int main() {

  sigset_t mask;
  sigfillset(&mask);
  sigdelset(&mask, SIGINT);
  sigdelset(&mask, SIGTERM);
  sigprocmask(SIG_SETMASK, &mask, NULL);
  struct sigaction sigint_action = {.sa_handler = sigint_handler};
  sigaction(SIGINT, &sigint_action, NULL);
  struct sigaction sigterm_action = {.sa_handler = sigterm_handler};
  sigaction(SIGTERM, &sigterm_action, NULL);


  printf("%d\n", getpid());
  fflush(stdout);

  while (sigterm_count == 0) {
    pause();
  }
  printf("%d\n", sigint_count);
}