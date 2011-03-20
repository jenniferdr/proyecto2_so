#include <stdio.h>
#include <signal.h>

int delay;

void childHandler();

/****************** Rutina principal ****************************************/
int  main(int argc, char * argv[]) {
  int pid;

  if (argc != 3) {
    printf("Error en argumentos..\n");
    printf("Usage: %s   \n", argv[0]);
    exit(0);
  }
  signal(SIGCHLD, childHandler); // se instala el nuevo manejador
  pid = fork();
  if (pid == 0) {
    execvp(argv[2],&argv[2]);
    perror("Error en exec");
  }
  else {
    delay=atoi(argv[1]);
    sleep(delay);
    printf("El hijo %d se excedio en tiempo...debe morir..\n",pid);
    kill(pid,SIGINT); //mata al hijo
  }
}
/****************** Manejador de la senal SIGALRM **************************/

void childHandler() {
  int childPid, childStatus;

  childPid = wait(&childStatus);

  printf("El hijo %d termino en menos de %d segundos...\n",childPid,delay);
  exit(0);
}
