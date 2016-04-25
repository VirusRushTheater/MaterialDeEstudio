#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MINVALUE    1
#define MAXVALUE    15
#define ITEMCOUNT   300

/*
 * ALGORITMO DE COUNTING SORT
 * Crea un histograma de n = (valor_max - valor_min + 1) elementos, y vuelve a llenar
 * el arreglo de origen de acuerdo al histograma generado.
 */

void counting_sort (int *arreglo, int cantidad, int valor_min, int valor_max)
{
    int* histograma;
    int tam_histograma;     // Tamaño del histograma
    int hcursor, acursor, hhcursor;   // Cursores para el histograma y arreglo.
    
    tam_histograma = (valor_max - valor_min + 1);
    
    // Crea el histograma con la cantidad de elementos necesarios.
    // calloc = malloc y asignar ceros a todos los elementos del nuevo arreglo.
    histograma = calloc (tam_histograma, sizeof (int));
    
    for (acursor = 0; acursor < cantidad; acursor++)
    {
        // Añade 1 al elemento del histograma correspondiente.
        histograma[ arreglo[acursor] - valor_min ]++;
    }
    
    // Imprime lo conseguido en el histograma.
    printf ("\n*************************************************************\n\
Histograma realizado:\n\
| # ||");
            
    for (hcursor = 0; hcursor < tam_histograma; hcursor++)
        printf ("%4d|", hcursor + valor_min);
        
    printf ("\n*************************************************************\n\
| n ||");
            
    for (hcursor = 0; hcursor < tam_histograma; hcursor++)
        printf ("%4d|", histograma[hcursor]);
        
    printf ("\n*************************************************************\n");
    
    // Ahora ordena el arreglo usando el counting-sort, insertando elementos de acuerdo a lo que
    // diga el histograma.
    
    // Hay dos 'for' anidados acá. Tentaría pensar de que eso lo haría un algoritmo de theta(n²),
    // pero si observan detenidamente, las veces que se ejecutan es exactamente 'n' veces.
    
    acursor = 0;
    
    for (hcursor = 0; hcursor < tam_histograma; hcursor++)
    {
        printf ("Insertando %d '%ds' en el arreglo.\n", histograma[hcursor], hcursor + valor_min);
        
        for (hhcursor = 0; hhcursor < histograma[hcursor]; hhcursor++)
        {
            arreglo[acursor++] = hcursor + valor_min;
        }
    }
    
    // Libera la memoria del histograma.
    free (histograma);
}

int main (int argc, char** argv)
{
    int k;
    
    // Arreglo de los elementos a ordenar
    int scrambled [ITEMCOUNT];
    
    // Para que genere números aleatorios distintos cada vez que se inicie el programa.
    srand (time (0));
    
    for (k = 0; k < ITEMCOUNT; k++)
        scrambled[k] = (rand () % (MAXVALUE - MINVALUE + 1)) + MINVALUE;    // Genera valores entre 1 y 10.
    
    // Imprime lo generado acá.
    printf ("Elementos iniciales: \n");
    for (k = 0; k < ITEMCOUNT; k++)
        printf ("%d - ", scrambled[k]);
    
    printf ("\n");
    
    counting_sort (scrambled, ITEMCOUNT, 1, MAXVALUE);
    
    // Imprime lo generado acá.
    printf ("\nElementos finales: \n");
    for (k = 0; k < ITEMCOUNT; k++)
        printf ("%d - ", scrambled[k]);
    
    printf ("\n");
        
    return 0;
}
