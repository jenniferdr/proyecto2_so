#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

static void Trabajo (int sig, siginfo_t *siginfo, void *context)
{
  printf ("Sending PID: %ld, UID: %ld\n",
	  (long)siginfo->si_pid, (long)siginfo->si_uid);
  
  printf("yupiii\n");
}

int main()
{
  struct sigaction act;


  act.sa_sigaction=&Trabajo;
  
  act.sa_flags=SA_SIGINFO;
  int papa;


  papa=getpid();
  printf("%d",papa);
  
  if(sigaction(SIGUSR2,&act,NULL)<0)
    {
      perror("porqueee");
      return 1;
    }
  
 
  while(1)
    {

    }

}

