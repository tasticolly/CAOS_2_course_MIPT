#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

sig_atomic_t value = 0;
sig_atomic_t need_exit = false;

void sigint_sigterm_handler() {
  need_exit = true;
}
void sigusr1_handler() {
  ++value;
}
void sigusr2_handler() {
  value *= -1;
}
int main() {

  sigset_t mask;
  sigfillset(&mask);
  sigdelset(&mask, SIGUSR1);
  sigdelset(&mask, SIGUSR2);
  sigdelset(&mask, SIGTERM);
  sigdelset(&mask, SIGINT);

  sigprocmask(SIG_SETMASK, &mask, NULL);
  struct sigaction sigusr1_action = {.sa_handler = &sigusr1_handler};
  sigaction(SIGUSR1, &sigusr1_action, NULL);
  struct sigaction sigusr2_action = {.sa_handler = &sigusr2_handler};
  sigaction(SIGUSR2, &sigusr2_action, NULL);
  struct sigaction sigint_sigterm_action = {.sa_handler = &sigint_sigterm_handler};
  sigaction(SIGINT, &sigint_sigterm_action, NULL);
  sigaction(SIGTERM, &sigint_sigterm_action, NULL);

  printf("%d\n", getpid());
  fflush(stdout);
  scanf("%d", &value);
  while (!need_exit) {
    pause();
    printf("%d\n", value);
    fflush(stdout);
  }
}
