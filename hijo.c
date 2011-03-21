#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <dirent.h>
#include <sys/stat.h>
#include<signal.h>
#include<string.h>

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

void tareaHijo(){
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
 kill(SIGUSR2,getppid());
}

main(){
 
  struct sigaction act;
  memset (&act, '\0', sizeof(act)); 
 
  act.sa_sigaction=&tareaHijo;
  
  act.sa_flags=SA_SIGINFO;
  if(sigaction(SIGUSR1,&act,NULL)<0)
    {
      perror("Error");
      
    }
 
  // while(1){
  //}  
 
  
}
