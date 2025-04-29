#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Tarea {
    int TareaID;
    char *Descripcion;
    int Duracion; // entre 10 - 100
} Tarea;

typedef struct Tnodo {
    Tarea T;
    struct Tnodo *siguiente;
} Tnodo;

// Prototipos
Tnodo *CrearListaVacia();
Tnodo *CrearNodo(Tarea tarea);
void InsertarNodo(Tnodo **start, Tnodo *nodo);
Tnodo *QuitarNodo(Tnodo **start, int id);
void MostrarTareas(Tnodo *start, const char *titulo);
Tnodo *BuscarTareaPorID(Tnodo *start, int id);
void BuscarTareaPorPalabra(Tnodo *start, char *palabra, const char *estado);

// Implementación
int main() {
    int idActual = 1000;                                                        //idActual: guarda el ID actual para asignarlo a cada tarea (empieza en 1000).
    char buffer[100];                                                           //buffer: usamos esto como espacio temporal para leer descripciones.
    int opcion;                                                                //opcion: para preguntar si el usuario quiere seguir cargando tareas.
    
    //Creamos dos listas enlazadas: una para tareas pendientes y otra para tareas realizadas
    Tnodo *pendientes = CrearListaVacia();
    Tnodo *realizadas = CrearListaVacia();

    // 1. Carga de tareas
    do {
        Tarea nueva;                                                            //Creamos una variable t de tipo Tarea
        nueva.TareaID = idActual++;                                             //Le asignamos un ID y lo incrementamos para la siguiente tarea

        printf("\nIngrese descripcion de la tarea: ");
        fflush(stdin);
        gets(buffer);                                                           //Le pedimos al usuario que ingrese la descripción y la guardamos en el buffer
        nueva.Descripcion = (char *)malloc(strlen(buffer) + 1);                 //Reservamos memoria justa para guardar la descripción en la estructura Tarea
        strcpy(nueva.Descripcion, buffer);

        nueva.Duracion = rand() % 91 + 10; // 10-100

        InsertarNodo(&pendientes, CrearNodo(nueva));                            //Creamos un nodo que contiene esa tarea y lo insertamos al inicio de la lista de pendientes

        printf("¿Desea ingresar otra tarea? (s/n): ");
        fflush(stdin);
        scanf("%d", &opcion);
        fflush(stdin);

    } while (opcion == 1);                                                      //Repetimos el proceso mientras el usuario quiera seguir cargando tareas

    // 2. Transferencia de tareas realizadas
    do {
        MostrarTareas(pendientes, "TAREAS PENDIENTES");

        printf("\nIngrese el ID de la tarea realizada (0 para finalizar): ");
        int id;
        scanf("%d", &id);
        fflush(stdin);

        if (id == 0) break;

        Tnodo *tareaRealizada = QuitarNodo(&pendientes, id);
        if (tareaRealizada) {
            InsertarNodo(&realizadas, tareaRealizada);
            printf("Tarea ID %d marcada como realizada.\n", id);
        } else {
            printf("Tarea no encontrada.\n");
        }

    } while (1);

    // 3. Listar tareas
    printf("\n--- LISTADO COMPLETO DE TAREAS ---\n");
    MostrarTareas(pendientes, "TAREAS PENDIENTES");
    MostrarTareas(realizadas, "TAREAS REALIZADAS");

    // 4. Búsqueda por ID o palabra clave
    printf("\n¿Desea buscar una tarea por ID [0] o por palabra clave [1]? ");
    int tipoBusqueda;
    scanf("%d", &tipoBusqueda);
    fflush(stdin);

    if (tipoBusqueda == 0) {
        int id;
        printf("Ingrese el ID de la tarea a buscar: ");
        scanf("%d", &id);
        fflush(stdin);

        Tnodo *encontrada = BuscarTareaPorID(pendientes, id);
        if (encontrada) {
            printf("Tarea encontrada en pendientes: [%d] %s (%d min)\n",
                   encontrada->T.TareaID, encontrada->T.Descripcion, encontrada->T.Duracion);
        } else {
            encontrada = BuscarTareaPorID(realizadas, id);
            if (encontrada) {
                printf("Tarea encontrada en realizadas: [%d] %s (%d min)\n",
                       encontrada->T.TareaID, encontrada->T.Descripcion, encontrada->T.Duracion);
            } else {
                printf("Tarea no encontrada.\n");
            }
        }

    } else if (tipoBusqueda == 1) {
        printf("Ingrese la palabra clave: ");
        gets(buffer);
        BuscarTareaPorPalabra(pendientes, buffer, "pendientes");
        BuscarTareaPorPalabra(realizadas, buffer, "realizadas");
    }

    return 0;
}

// Implementación de funciones auxiliares
Tnodo *CrearListaVacia() {
    return NULL;
}

Tnodo *CrearNodo(Tarea tarea) {
    Tnodo *nuevo = (Tnodo *)malloc(sizeof(Tnodo));
    nuevo->T = tarea;
    nuevo->siguiente = NULL;
    return nuevo;
}

void InsertarNodo(Tnodo **start, Tnodo *nodo) {
    nodo->siguiente = *start;
    *start = nodo;
}

Tnodo *QuitarNodo(Tnodo **start, int id) {
    Tnodo **aux = start;
    while (*aux && (*aux)->T.TareaID != id) {
        aux = &(*aux)->siguiente;
    }

    if (*aux) {
        Tnodo *temp = *aux;
        *aux = (*aux)->siguiente;
        temp->siguiente = NULL;
        return temp;
    }
    return NULL;
}

void MostrarTareas(Tnodo *start, const char *titulo) {
    printf("\n--- %s ---\n", titulo);
    while (start) {
        printf("[%d] %s (%d min)\n", start->T.TareaID, start->T.Descripcion, start->T.Duracion);
        start = start->siguiente;
    }
}

Tnodo *BuscarTareaPorID(Tnodo *start, int id) {
    while (start && start->T.TareaID != id) {
        start = start->siguiente;
    }
    return start;
}

void BuscarTareaPorPalabra(Tnodo *start, char *palabra, const char *estado) {
    int encontrada = 0;
    while (start) {
        if (strstr(start->T.Descripcion, palabra)) {
            printf("Tarea encontrada en %s: [%d] %s (%d min)\n", estado,
                   start->T.TareaID, start->T.Descripcion, start->T.Duracion);
            encontrada = 1;
        }
        start = start->siguiente;
    }
    if (!encontrada) {
        printf("No se encontró la palabra '%s' en tareas %s.\n", palabra, estado);
    }
}