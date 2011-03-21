#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>

typedef struct Lista{
  struct Reg *first;
  struct Reg *last;
  int numRegs;
}Lista;

/* Registros para guardar los nombres
 * de los directorios a explorar
 */ 
typedef struct Reg{
  char *nombre;
  char *n_block;
  struct Reg *anterior;
  struct Reg *next; 
}Reg;


/* Funcion para crear una Lista
 * Regresa un apuntador a la lista creada
 */
struct Lista* crearLista(){
  struct Lista * newList;
    if((newList= (struct Lista*)malloc(sizeof(struct Lista)))==NULL)
      perror("No se pudo Crear la lista:");
    newList->first= NULL ;
    newList->last=NULL;
    newList->numRegs=0;
    return newList;
}

/* Funcion que agrega la cadena de caracteres "nombre"
 * en la Lista "lista".
 */ 
  void agregarNombre( Lista *lista,char *nombre){
    Reg *registro;
    
    if((registro= (Reg*)malloc(sizeof(struct Reg)))==NULL)
      perror("No se pudo agregar el nombre a la lista:");
    registro->next= NULL;
    registro->nombre= nombre;

    if(lista->first==NULL){
      lista->first= registro;
      lista->numRegs=1;
      registro->anterior=NULL;
    }else{

      lista->numRegs++;
      lista->last->next=registro;
      registro->anterior=lista->last;
    }
    lista->last=registro;
}

/* Funcion que agrega la cadena de caracteres "nombre"
 * y el numero de bloques "numB" en la Lista "lista".
 */ 
void agregarNombreBloq( Lista *lista,char *nombre,char *numB){
    Reg *registro;
    
    if((registro= (Reg*)malloc(sizeof(struct Reg)))==NULL)
      perror("No se pudo agregar el nombre a la lista:");
    registro->next= NULL;
    registro->nombre= nombre;
    registro->n_block= numB;

    if(lista->first==NULL){
      lista->first= registro;
      lista->numRegs=1;
      registro->anterior=NULL;
    }else{

      lista->numRegs++;
      lista->last->next=registro;
      registro->anterior=lista->last;
    }
    lista->last=registro;
}

/* Funcion que toma un nombre de la lista
 * y luego lo elimina de ella
 */
char* obtenerNombre(struct Lista *lista){
  char *nombre= lista->last->nombre;
  struct Reg *aux= lista->last->anterior;
  free(lista->last);
  lista->last= aux;
  lista->numRegs--;
  return nombre;
}

void atenderHijo(){
  printf("atendido");
}

void explorar(Lista *directorios,int *numBloques,char *directorio){
 DIR *dp;
 struct dirent *sp;
 struct stat statbuf;

dp= opendir(directorio);
 if(dp!=NULL){
   while((sp=readdir(dp))!=NULL){
     // por cada entrada  
     if(stat(sp->d_name,&statbuf)==-1){
       perror("Error al intentar acceder a los atributos de archivo");
       exit(1);
     }
     if (strcmp(sp->d_name,".") !=0 && (strcmp(sp->d_name,"..") !=0)){ 
       // Ver en statbuf si es regular o dir
       if(S_ISDIR(statbuf.st_mode)){
	 if (strlen(sp->d_name)>=64){
	   printf("Error: Nombre de archivo muy largo");
	   exit(1);}
	 agregarNombre(directorios,sp->d_name);
       }else{
	 numBloques= numBloques+ statbuf.st_blocks;
       }
     }
   }
 }
}

Lista *dirListos;

int main(int argc, char **argv){

struct sigaction act;

  memset (&act, '\0', sizeof(act));
  act.sa_sigaction=&atenderHijo;
  
  act.sa_flags=SA_SIGINFO;
  if(sigaction(SIGUSR2,&act,NULL)<0)
    {
      perror("Error");
      exit(1);
    }

  int n=1; // Nivel de concurrencia 
  char *direct= "./";
  int op,i,salida;

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

  int setsalida;
  setsalida= ((i=optind)<argc);

  int f;
  for(f = optind+1; f < argc; f++)
    printf ("Advertencia: Argumento ignorado: %s\n", argv[f]);

  if(setsalida) {
      //Cambiar la salida
      if((salida = open(argv[i], O_CREAT|O_TRUNC|O_WRONLY, 7644))==-1)
	perror("Error al abrir archivo");  
    }

// Explorar el directorio pasado por parametro 
 Lista *directorios= crearLista();
 int *numBloques=0;

 explorar(directorios,numBloques,direct);
 printf("%s", obtenerNombre(directorios));
 /* Arreglo que contendra los pid de cada hijo, los descriptores
  * de los pipes que utilizara para comunicarse con ellos
  * y un indicador de si esta ocupado o no. */ 
 int *pipes[n];
 int ocupados=0;

 /* Crear los trabajadores y 
  * Para cada uno crear un anillo de comunicacion */ 
 for(i=0; i<n; i++){
   int fd[2];
   int fd2[2];
   if(pipe(fd)!=0 || pipe(fd2)!=0){
     perror("Pipe:");
     exit(1);
   }

   pipes[i]= (int*) malloc(sizeof(int)*4);
   *(pipes[i]+1)= fd[1];
   *(pipes[i]+2)= fd[0];
   *(pipes[i]+3)= 0;
   pid_t hijo=fork();
   if(hijo==0){
      dup2(fd[0],0);

     dup2(fd2[1],1);
     close(fd2[0]);
     close(fd2[1]);
     close(fd[0]);
     close(fd[1]);
     execl("./hijo","./hijo",NULL);
   }else{
     wait(0);
     *pipes[i]= hijo;
     dup2(fd[1],fd[0]);
     dup2(fd2[0],fd[1]);
     close(fd2[0]);
     close(fd2[1]);
   }
 }

 // Lista que contendra a los directorios listos
 dirListos= crearLista();

 printf("hasta aqui");
 
 // while(1){

// Asignar tareas
while(directorios->numRegs!=0 || ocupados!=0){
  
  if(ocupados<n && directorios->numRegs!=0)
    {
      // iterar a ver quien esta libre
      int i;
      for(i=0;i<n && ocupados<n && directorios->numRegs>0;i++)
	{
	  if (*(pipes[i]+3)=0){
	    char * dir;
	    dir=obtenerNombre(directorios);
	    write(*(pipes[i]+2),dir, strlen(dir)+1);
	    *(pipes[i]+3)=1;
	    ocupados++;
	    printf("%d",*pipes[i]);
	    kill(SIGUSR1,*pipes[i]);
	   
	  }
	}
    }

 }
//Termina todo imprimir por consola y archivo en la variable salida
// }  
}




