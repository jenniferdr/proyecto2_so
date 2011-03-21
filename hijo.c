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

void tareaHijo(int sig){
  int fd= open("tarea hijo",O_CREAT,666); 
 int n;
  char directorio[64];
  Lista *directorios;
  scanf( "%s", directorio );
  int numBloques=0;

DIR *dp;
 struct dirent *sp;
 struct stat statbuf;

dp= opendir(directorio);
 if(dp!=NULL){
   while(sp=readdir(dp)){
     // por cada entrada 
     if(stat(sp->d_name,&statbuf)==-1){
       perror("Error al intentar acceder a los atributos de archivo");
       exit(1);
     }
     if (strcmp(sp->d_name,".") !=0 && (strcmp(sp->d_name,"..") !=0)){ 
       // Ver en statbuf si es regular o dir
       if(S_ISDIR(statbuf.st_mode)){
	 if (strlen(sp->d_name)>=64){
	   perror("Error: Nombre de archivo muy largo");
	   exit(1);
	   /* Recordar manejar este error*/}
	  agregarNombre(directorios,sp->d_name);
       }else{
	 numBloques= numBloques+ statbuf.st_blocks;
       }
     }
   }
 }

 printf("%d\n",numBloques);
 while(directorios->numRegs>0){
   printf("%s\n",obtenerNombre(directorios));
 }
 kill(getppid(),SIGUSR1);
}

main(){
 
  struct sigaction usr_action;
  sigset_t block_mask;
  pid_t child_id;

  /* Establish the signal handler.  */
  sigfillset (&block_mask);
  usr_action.sa_handler =tareaHijo ;
  usr_action.sa_mask = block_mask;
  usr_action.sa_flags = 0;
  sigaction (SIGUSR2, &usr_action, NULL);
  
   while(1){
     
  }  
 
  
}
