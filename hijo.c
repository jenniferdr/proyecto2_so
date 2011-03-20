#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <dirent.h>
#include <sys/stat.h>

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

void explorar(Lista *directorios,int *numBloques,char *directorio){
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
       // Ver en statbuf  si es regular o dir
       if(S_ISDIR(statbuf.st_mode)){
	 agregarNombre(directorios,sp->d_name);
       }else{
	 numBloques= numBloques+ statbuf.st_blocks;
       }
     }
   }
 }
}


main(){
  printf("hola");

exit(0);

}
