
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

static sigjmp_buf jmpbuf;

static void
sig_handler (int signo)
{
  siglongjmp (jmpbuf, 1);
}

int
main (void)
{
  char *p = NULL;
  volatile int ret = 0;
  struct sigaction sa;

  sa.sa_handler = sig_handler;
  sigemptyset (&sa.sa_mask);
  sa.sa_flags = SA_SIGINFO;

  if (sigaction (SIGSEGV, &sa, 0))
    {
      perror ("installing SIGSEGV handler\n");
      exit (1);
    }

  puts ("Attempting to sprintf to null ptr");
  if (setjmp (jmpbuf))
    {
      puts ("Exiting main...");
      return ret;
    }

  sprintf (p, "This should segv\n");

  return 1;
}

