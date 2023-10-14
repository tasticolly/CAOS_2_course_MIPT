#include <stdio.h>
#include <unistd.h>
#include <signal.h>
//#include <assert.h>
//#include <sys/types.h>
//#include <sys/wait.h>

//#define SIGRTMIN 2

volatile sig_atomic_t sigterm_count = 0;

static void handler(int signum, siginfo_t* info, void* ucontext) {
  int received_value = info->si_value.sival_int;
  int pid = info->si_pid;
  if(received_value == 0) {
    ++sigterm_count;
    return;
  }
  sigqueue(pid, signum, (union sigval) {.sival_int = received_value - 1});
}

int main(int argc, char* argv[]) {
  sigset_t mask;
  sigfillset(&mask);
  sigdelset(&mask, SIGRTMIN);
  sigprocmask(SIG_SETMASK, &mask, NULL);

  struct sigaction sig_action = {.sa_sigaction = handler, .sa_flags = SA_SIGINFO, .sa_mask=mask};
  sigaction(SIGRTMIN, &sig_action, NULL);

//    int main_pid = getpid();
//    printf("%d\n", main_pid);
//    fflush(stdout);

  while (!sigterm_count) {
    pause();
  }

  return 0;
}