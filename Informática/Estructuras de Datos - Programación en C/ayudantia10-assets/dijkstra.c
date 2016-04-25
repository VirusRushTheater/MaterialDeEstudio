#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*
 * Implementación del Algoritmo de Dijkstra en C.
 * Necesitamos dos cosas:
 * 1) Un arreglo de nodos
 * 2) Para cada nodo:
 *      - Un identificador
 *      - Una ruta: Un destino (puntero a nodo) y un peso (un entero positivo)
 * 
 * El algoritmo de Dijkstra buscará el camino más corto entre dos nodos, mostrando
 * tanto su peso como los nodos por los que tiene que pasar.
*/

// Prototipos. Ambas estructuras se necesitan mutuamente, así que es necesario que el compilador
// sepa que existen antes de hacer cualquier cosa con ellas.
struct _dijkstra_ruta;
struct _dijkstra_nodo;

// Esta es una lista enlazada para rutas.
struct _dijkstra_ruta
{
    unsigned int peso;                  // Peso de la ruta
    struct _dijkstra_nodo *direccion;   // Hacia dónde apunta la ruta
    struct _dijkstra_ruta *siguiente;   // Siguiente en la lista enlazada
};

// Estructura para los nodos.
struct _dijkstra_nodo
{
    char *nombre;                       // Identificador para los nodos
    struct _dijkstra_ruta *lista_rutas; // Lista enlazada con las rutas que salen de este nodo.
};

// Oficialmente cada nodo tiene el tipo de dato "nodo".
typedef struct _dijkstra_nodo   nodo;
typedef struct _dijkstra_ruta   ruta;

// Cuántos nodos queremos en el grafo??
#define CANT_NODOS      7

/*
 * Definiremos los nodos como un arreglo de nodos. Como cada uno tiene una lista enlazada de rutas
 * vamos a necesitar una función que añada nodos y otra que libere su memoria.
 * 
 * Si la ruta ya existe, actualiza su peso.
 */
 
void agregar_ruta (nodo* desde, nodo* hacia, unsigned int peso)
{
    ruta **ultima_ruta;
    
    // Si no tiene rutas, añadimos la primera.
    if ((desde->lista_rutas) == NULL)
    {
        desde->lista_rutas = malloc (sizeof (ruta));
        desde->lista_rutas->direccion = hacia;
        desde->lista_rutas->peso = peso;
        desde->lista_rutas->siguiente = NULL;
        return;
    }
    
    ultima_ruta = &(desde->lista_rutas);
    
    // En caso contrario, debemos recorrer la lista.
    while ((*ultima_ruta)->siguiente != NULL)
    {
        // Si apunta a la misma dirección simplemente actualizamos el peso.
        if ((*ultima_ruta)->direccion == hacia)
        {
            (*ultima_ruta)->peso = peso;
            return;
        }
        
        // En caso contrario seguimos recorriendo la lista.
        ultima_ruta = &((*ultima_ruta)->siguiente);
    }
    
    // Al llegar al final de la lista, añadimos la ruta.
    (*ultima_ruta)->siguiente = malloc (sizeof (ruta));
    (*ultima_ruta)->siguiente->direccion = hacia;
    (*ultima_ruta)->siguiente->peso = peso;
    (*ultima_ruta)->siguiente->siguiente = NULL;
}

/*
 * Asigna un identificador al nodo y lo inicializa en NULL.
 * Partimos de un arreglo de nodos, así que no necesitamos usar MALLOC.
 */ 
nodo iniciar_nodo (const char* identificador)
{
    nodo retval;
    retval.nombre = strdup (identificador);
    retval.lista_rutas = NULL;
    
    return retval;
}

/*
 * Función para ingresar un número y un nodo en la cola de prioridad.
 */
 
void push_cola (ruta** cabeza, ruta pushear)
{    
    // Si está vacía insertar el primer elemento.
    if (*cabeza == NULL)
        goto agregar_nodo;
        
    // Si no está vacía, recorrerla.
    else
    {
        // Si cabeza está apuntando a un nodo con un peso mayor, añadir.
        if ((*cabeza)->peso >= pushear.peso)
            goto agregar_nodo;
            
        // Caso contrario, continuar recursivamente.
        else
            push_cola (&((*cabeza)->siguiente), pushear);
    }
    // Para evitar que se pase al GoTo y se traspapele todo.
    return;
    
    // Un GoTo para hacer el código más ¿ordenado?
    // Agregar el nodo antes del elemento actual (*cabeza puede ser Nulo)
    agregar_nodo:
    pushear.siguiente = *cabeza;
    (*cabeza) = malloc (sizeof (ruta));
    **cabeza = pushear;
    return;
}

/*
 * Función para extraer y quitar un dato de una cola de prioridad.
 * Regresa 1 si la cola aún no está vacía y 0 si lo está.
 */
 
int pop_cola (ruta** cabeza, ruta* dispensed)
{
    if (*cabeza == NULL)
        return 0;
    
    ruta *next = (*cabeza)->siguiente;
    *dispensed = **cabeza;
    free (*cabeza);
    (*cabeza) = next;
    
    return 1;
}

/*
 * Imprimir un paso de Dijkstra (usar archivo = stdout para imprimirlo en la consola)
 */
 
void imprimir_paso_dijkstra (FILE* archivo, nodo* grafo, int cant_nodos, ruta* estado, ruta* cola)
{
    int k;
    ruta* nullsafe;
   
    for (k = 0; k < cant_nodos; k++)
        fprintf (archivo, "**************");
    
    fprintf (archivo, "\n");
    
    for (k = 0; k < cant_nodos; k++)
    {
        fprintf (archivo, "| %-12.12s", grafo[k].nombre);
    }
    
    fprintf (archivo, "\n");
    
    for (k = 0; k < cant_nodos; k++)
        fprintf (archivo, "**************");
        
    fprintf (archivo, "\n|");
    
    for (k = 0; k < cant_nodos; k++)
    {
        if (estado[k].direccion != NULL)
            if (estado[k].peso != UINT_MAX)
                fprintf (archivo, "%6u,%-6.6s|", estado[k].peso, estado[k].direccion->nombre);
            else
                fprintf (archivo, "   Inf,%-6.6s|", estado[k].direccion->nombre);
        else
            if (estado[k].peso != UINT_MAX)
                fprintf (archivo, "%6u,  NULL|", estado[k].peso);
            else
                fprintf (archivo, "   Inf,  NULL|");
    }
    
    fprintf (archivo, "\n");
    for (k = 0; k < cant_nodos; k++)
        fprintf (archivo, "**************");
        
    fprintf (archivo, "\n");
    fprintf (archivo, "Cola al final: ");
    
    nullsafe = cola;
    if (nullsafe == NULL)
    {
        fprintf (archivo, "VACIA. Aquí se termina el algoritmo de Dijkstra.\n");
        return;
    }
    
    do
    {
        fprintf (archivo, "(%5u,%-5.5s) ->", nullsafe->peso, nullsafe->direccion->nombre);
        nullsafe = nullsafe->siguiente;
    }
    while (nullsafe != NULL);
    
    fprintf (archivo, "\n\n");
}

/* ---------------------------------------------------------------
 * ALGORITMO DE DIJKSTRA (implementado con una cola de prioridad)
 * 
 * Se parte de un nodo N.
 * Se crea una tabla inicializada en (Infinito, NULL) con un elemento para cada nodo. Esta tabla contendrá
 * el peso más liviano encontrado desde el nodo N, y desde cuál nodo se obtiene ese peso.
 * Se añade el nodo N y un peso 0 a la cola de prioridad.
 * 
 * Mientras la cola de prioridad no esté vacía:
 *     Sacar un nodo P desde la cola de prioridad.
 *     Si el peso actualizado con el nodo P es menor al peso anterior:
 *   
 * Devuelve el arreglo con los pesos desde el nodo indicado.       
 */
ruta* dijkstra (nodo* grafo, unsigned int cant_nodos, unsigned int nodo_inicio)
{
    // Cambia esto si quieres que el resumen de Dijkstra se vaya a otro archivo.
    FILE *archivo_salida = stdout;
    
    fprintf (archivo_salida, "Algoritmo de Dijkstra de caminos desde %s.\n\n", grafo[nodo_inicio].nombre);
    
    int k;
    int indice;
    ruta *cola_prioridad = NULL;
    
    // Usada para añadir nuevos elementos a la cola de prioridad y extraerlos.
    ruta nueva;
    ruta dispensada;
    ruta* ruta_adyacente;
    
    unsigned int peso_actual;
    
    // Se crea un arreglo con pesos y nodos anteriores para cada nodo del grafo.
    ruta *pesos_y_nodos = malloc (cant_nodos * sizeof(ruta));
    
    // Se inicializan todos (excepto el de inicio) con (Infinito, NULL)
    for (k = 0; k < cant_nodos; k++)
    {
        if (k != nodo_inicio)
            pesos_y_nodos[k].peso = UINT_MAX;   // Lo más cercano a infinito con lo que contamos.
        else
            pesos_y_nodos[k].peso = 0;
        
        pesos_y_nodos[k].direccion = NULL;
    }
    
    // Se añade a la cola el nodo de inicio y un peso de 0.
    nueva.direccion = &grafo[nodo_inicio];
    nueva.peso = 0;
    
    push_cola (&cola_prioridad, nueva);
    
    // Imprime un paso de Dijkstra
    imprimir_paso_dijkstra (archivo_salida, grafo, cant_nodos, pesos_y_nodos, cola_prioridad);
    
    // Continuar ejecutando el algoritmo mientras la cola no esté vacía.
    // El while se ejecuta hasta que la función devuelve 0 (cuando la cola está vacía)
    while (pop_cola (&cola_prioridad, &dispensada))
    {
        /* En la cola de prioridad (implementada usando el tipo de dato ruta)...
         * - peso: peso actual.
         * - direccion: Nodo al cual se le examinan las flechas que salen de él.
         * - siguiente: Nada.
         */
        
        // Se observan todos los nodos adyacentes al nodo que recién fue sacado de la cola.
        ruta_adyacente = dispensada.direccion->lista_rutas;
        
        fprintf (archivo_salida, "Examinando nodos adyacentes a %s.\n", dispensada.direccion->nombre);
        while (ruta_adyacente != NULL)
        {
            // Índice de la matriz pesos y nodos correspondiente al nodo indicado. Chamullo mula.
            indice = ((unsigned long) ruta_adyacente->direccion - (unsigned long) grafo) / sizeof(nodo);
            
            // Si la suma entre el peso actual y el de la ruta indicada es menor al almacenado en la matriz recorrida, actualizar y colocar en la cola de prioridad.
            if ((dispensada.peso + ruta_adyacente->peso) <= pesos_y_nodos[indice].peso)
            {
                // Reporte si encuentra un mejor camino. El if es para evitar los segmentation faults.
                if (pesos_y_nodos[indice].direccion != NULL)
                    fprintf (archivo_salida, "Encontrado un mejor camino a %s (Peso %u desde %s v/s %u desde %s)\n",
                            grafo[indice].nombre, 
                            (dispensada.peso + ruta_adyacente->peso), dispensada.direccion->nombre, 
                            pesos_y_nodos[indice].peso, pesos_y_nodos[indice].direccion->nombre);
                else
                    fprintf (archivo_salida, "Encontrado un mejor camino a %s (Peso %u desde %s v/s Infinito desde Ningún Lugar)\n",
                            grafo[indice].nombre, 
                            (dispensada.peso + ruta_adyacente->peso), dispensada.direccion->nombre);
                
                pesos_y_nodos[indice].peso = dispensada.peso + ruta_adyacente->peso;
                pesos_y_nodos[indice].direccion = dispensada.direccion;
                
                nueva.direccion = ruta_adyacente->direccion;
                nueva.peso = dispensada.peso + ruta_adyacente->peso;
                
                push_cola (&cola_prioridad, nueva);
            }
            
            // Pasamos al siguiente.
            ruta_adyacente = ruta_adyacente->siguiente;
        }
        
        imprimir_paso_dijkstra (archivo_salida, grafo, cant_nodos, pesos_y_nodos, cola_prioridad);
    }
    
    free (pesos_y_nodos);
    return NULL;
}

int main (int argc, char** argv)
{
    // Creamos los nodos...
    nodo grafo [CANT_NODOS];
    
    // Y les asignamos un identificador.
    // Como pueden darse cuenta, soy fan de League of Legends.
    grafo[0] = iniciar_nodo ("Demacia");    // Nodo 1
    grafo[1] = iniciar_nodo ("Noxus");      // Nodo 2
    grafo[2] = iniciar_nodo ("Piltover");   // Nodo 3
    grafo[3] = iniciar_nodo ("Zaun");       // Nodo 4
    grafo[4] = iniciar_nodo ("Ionia");      // Nodo 5
    grafo[5] = iniciar_nodo ("Freljord");   // Nodo 6
    grafo[6] = iniciar_nodo ("Bandle City");// Nodo 7
    
    // Añadimos todas las rutas...
    agregar_ruta (&grafo[0], &grafo[1], 20);
    agregar_ruta (&grafo[0], &grafo[3], 80);
    agregar_ruta (&grafo[0], &grafo[6], 90);
    
    agregar_ruta (&grafo[1], &grafo[5], 10);
    
    agregar_ruta (&grafo[2], &grafo[5], 50);
    
    agregar_ruta (&grafo[3], &grafo[2], 10);
    agregar_ruta (&grafo[3], &grafo[6], 20);
    
    agregar_ruta (&grafo[4], &grafo[1], 50);
    agregar_ruta (&grafo[4], &grafo[6], 30);
    
    agregar_ruta (&grafo[5], &grafo[2], 10);
    agregar_ruta (&grafo[5], &grafo[3], 40);
    
    agregar_ruta (&grafo[6], &grafo[0], 20);
    
    // Ejecuta el algoritmo de Dijkstra para buscar la distancia entre Demacia y las otras ciudades.
    // (Cambia el 0 por algún número entre el 0 y el 6 para buscar otra ciudad)
    dijkstra (grafo, CANT_NODOS, 4);
    
    return 0;
}
