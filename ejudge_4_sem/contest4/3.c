#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

volatile sig_atomic_t value = 1;
volatile sig_atomic_t return_pid = 0;

void number_handler(int num_of_sig, siginfo_t* info, void* context) {
  value = info->si_value.sival_int;
  return_pid = info->si_pid;
}

int main() {
  sigset_t mask;
  sigfillset(&mask);
  sigdelset(&mask, SIGRTMIN);
  sigprocmask(SIG_SETMASK, &mask, NULL);

  struct sigaction sigrtmin_action = {.sa_sigaction = &number_handler};
  sigrtmin_action.sa_flags = SA_SIGINFO;
  sigaction(SIGRTMIN, &sigrtmin_action, NULL);

  while (true) {
    pause();
    if (value == 0) {
      return 0;
    }
    union sigval return_si;
    return_si.sival_int = value - 1;
    sigqueue(return_pid, SIGRTMIN, return_si);
  }
}
