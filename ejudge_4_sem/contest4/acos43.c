#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/wait.h>

const char prog[] = "./acos43";

void SigactionHandler(int signal, siginfo_t* info, void* context)
{
  printf("%d\n", info->si_value.sival_int);
  fflush(stdout);
}

int main(int argc, char** argv)
{
  pid_t pid = fork();
  if (pid == 0) {
    execlp(prog, prog, NULL);
    return 1;
  }
  sigset_t mask;
  sigfillset(&mask);
  sigprocmask(SIG_BLOCK, &mask, NULL);
  sigaction(
      SIGRTMIN,
      &(struct sigaction){
          .sa_sigaction = SigactionHandler,
          .sa_flags = SA_RESTART | SA_SIGINFO,
          .sa_mask = mask},
      NULL);
  sigemptyset(&mask);
  sleep(1);
  union sigval s;
  s.sival_int = 40;
  sigqueue(pid, SIGRTMIN, s);
  sigsuspend(&mask);
  s.sival_int = 43;
  sigqueue(pid, SIGRTMIN, s);
  sigsuspend(&mask);
  s.sival_int = 15;
  sigqueue(pid, SIGRTMIN, s);
  sigsuspend(&mask);
  s.sival_int = 10;
  sigqueue(pid, SIGRTMIN, s);
  sigsuspend(&mask);
  s.sival_int = 0;
  sigqueue(pid, SIGRTMIN, s);
  int status;
  waitpid(pid, &status, 0);
  if (WIFEXITED(status) && !WEXITSTATUS(status)) {
    printf("good");
    fflush(stdout);
  } else if (WIFSIGNALED(status)) {
    printf("nu chutochku pizje");
  } else {
    printf("nayebnulis mi)))");
  }
}