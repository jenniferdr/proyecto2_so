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
void crearLista(Lista * newList){

    if((newList= (struct Lista*)malloc(sizeof(struct Reg)))==NULL)
      perror("No se pudo Crear la lista:");
    newList->first= NULL ;
    newList->last=NULL;
    newList->numRegs=0;
}


/* Funcion que agrega la cadena de caracteres "nombre"
 * en la lista "lista".
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
  return nombre;
}

int main(int argc, char **argv){

  int n=1; // Nivel de concurrencia 
  char *direct= "./";
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
      if((fd = open(argv[i], O_CREAT|O_TRUNC|O_WRONLY, 7644))==-1)
	perror("Error al abrir archivo");
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

// Como sea soy el padre y digo que vere que hay en direct 

DIR *dp;
struct dirent *sp;
struct stat statbuf;

 dp= opendir(direct);
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
       // enlistar nombre 
       printf("es directorio \n ");
     }else{
       // x= x+ statbuf.st_blocks
       printf("%d",statbuf.st_blocks);
	  }
	
      }
	

   }
 }
}





