/* Ejemplo de manejo de signals 
 * Programa un contador de decenas de segundos utilizando alarmas y
 * manejadores de se~nales. Ademas, instala un manejador para capturar
 * la se~nal SIGINT una sola vez, restableciendo el manejador por
 * defecto una vez ejecutada la primera vez.
 * Utiliza dos manejadores de sen~les, uno para SIGINT y otro para 
 * SIGALRM.
*/ 
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int dseg, min; /* decenas de segundos y minutos, resp. */

/* ControlC
 * Manejador de sen~al SIGINT
 * Cuando se ejecuta la primera vez, restablece el manejador por
 * defecto usando la llamada signal
 */
void Controlc (int sig) 
{
  printf("Protegido contra Control-C\n");
  signal(SIGINT, SIG_DFL); /* restablece el manejador por defecto */ 
 
}
/* Tick
 * Manejador de se~nal SIGALRM
 * Implementa un contador de decenas de segundos. Imprime en la
 * pantalla el tiempo transcurrido
 */
void Tick (int sig)
{
  dseg = dseg + 10;
  if (dseg == 60) {
    dseg = 0;
    min++;
  }
  printf("Van %d minutos y %d segundos\n", min, dseg);
  signal(SIGALRM, Tick); /* reinstala el manejador. No es necesario en
			    ciertos versiones de Unix */ 
  return;
}

int main (void) 
{
  /* Inicializa contadores */ 
  dseg = 0;
  min = 0;
  /* Instala manejadores de sen~ales SIGINT y SIGALRM*/ 
  signal(SIGINT, Controlc);
  signal(SIGALRM, Tick);
  /* lazo infinito */ 
  for (;;) {
    alarm(5); /* programa alarma para dentro de 10 segundos */
    pause(); /* duerme hasta que recibe una se~nal */
  }
  exit(0);
}
