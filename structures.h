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
 * Regresa un apuntador a la lista creada 12
 */
extern void crearLista(Lista * newList);

/* Funcion que agrega la cadena de caracteres "nombre"
 * en la lista "lista".
 */ 
extern  void agregarNombre( Lista *lista,char *nombre);

/* Funcion que toma un nombre de la lista
 * y luego lo elimina de ella
 */
char* obtenerNombre(struct Lista *lista);
