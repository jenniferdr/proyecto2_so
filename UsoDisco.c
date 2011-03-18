#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 

int main(int argc, char **argv){

  int n=1; // Nivel de concurrencia 
  char *direct= ".";
  int op,i;

  opterr=0;
  // Obtener argumentos y validarlos  
  while((op=getopt(argc,argv,"hn:d:"))!=-1)
    switch(op){
    case 'h':
      if(argc>1)
	printf("Error de sintaxis\n");
      printf("Sintaxis: ./UsoDisco [-h] | [-n i] [-d directorio] [ salida ] \n");
      // Mostrar el resto de la ayuda
      exit(EXIT_SUCCESS);
      break;
    case'n':
      if((n=atoi(optarg))<1){
	printf("Error: El nivel de concurrencia debe ser mayor a 1\n");
	exit(EXIT_FAILURE);
      }
      break;
    case'd':
      direct=optarg;
      break;
    case '?':
      printf("Error opcion desconocida\n");
      exit(EXIT_FAILURE);
      break;
    deafult:
     exit(EXIT_FAILURE);
    }

  int redireccionar;
  redireccionar= ((i=optind)<argc);

  int f;
  for(f = optind+1; f < argc; f++)
    printf ("Advertencia: Argumento ignorado: %s\n", argv[f]);

  if(redireccionar)
    {
      //Redirecconar la salida estandar
      int fd; 
      fd = open(argv[i], O_CREAT|O_TRUNC|O_WRONLY, 7644);
      //Verificar si lo creo// 
      dup2(fd, 1); 
      close(fd); 
    }
  

  // Comenzar super codigo aqui!!

int *pipes[n];

// Crear Pipes pero esto da los descriptores de archivo donde no es...
for(i=0; i<n; i++){
	pipes[i]= (int*) malloc(sizeof(int)*2);
	if(pipe(pipes[i])!=0){
	perror("Pipe:");
	exit(1);
	}
}



}






