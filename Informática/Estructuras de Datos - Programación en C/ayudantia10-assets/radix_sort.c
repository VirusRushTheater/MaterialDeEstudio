#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXVALUE    9999
#define BASE        10
#define ITEMCOUNT   15

/*
 * Función auxiliar que calcula una potencia entera.
 */
unsigned long potencia (unsigned long base, unsigned long exp)
{
    // Casos extremos
    if (exp == 1)
        return base;
    if (exp == 0)
        return 1;
        
    // Exponente impar
    if (exp & 1)
        return (base * potencia (base, exp/2) * potencia (base, exp/2));
    else
        return (potencia (base, exp/2) * potencia (base, exp/2));
}

/*
 * Simplemente marca un dígito. (Se debe hacer "free" después a la variable que salga!)
 * numero: número de entrada
 * digito: dígito a marcar, Ej: para unidad, usar digito = 0; decena: digito = 1; y así...
 * max_digitos: cuántos dígitos máximos tiene el número. Si MAXVALUE = 9999, usar 4.
 * 
 * OJO: SOLO SIRVE PARA BASE 10
 */
char* marcar_digito (int numero, int digito, int max_digitos)
{
    char *retorno;
    char *patron;
    
    int potencia_base = potencia (10, digito);
    
    int digito_marcado = (numero / potencia_base) % 10;
    int digitos_antes = (numero / (potencia_base * 10));
    int digitos_despues = (numero % potencia_base);
    
    // Los dígitos de antes se ignoran
    // asprintf: es como combinar un malloc con un sprintf. Vean "man asprintf" si les da curiosidad.
    if (digito >= (max_digitos - 1))
    {
        asprintf (&patron, "[%%d]%%0%dd", max_digitos - 1);
        //asprintf (&patron, "\033[22;30;43m%%d\e[m%%0%dd", max_digitos - 1);
        asprintf (&retorno, patron, digito_marcado, digitos_despues);
    }
    else if (digito == 0)
    {
        asprintf (&patron, "%%0%dd[%%d]", max_digitos - 1);
        //asprintf (&patron, "%%0%dd\033[22;30;43m%%d\e[m", max_digitos - 1);
        asprintf (&retorno, patron, digitos_antes, digito_marcado);
    }
    else
    {
        asprintf (&patron, "%%0%dd[%%d]%%0%dd", max_digitos - digito - 1, digito);
        //asprintf (&patron, "%%0%dd\033[22;30;43m%%d\e[m%%0%dd", max_digitos - digito - 1, digito);
        asprintf (&retorno, patron, digitos_antes, digito_marcado, digitos_despues);
    }
    
    free (patron);
    return retorno;
}

/*
 * Paso para el Radix-sort: ordena en el arreglo el arreglo usando un método parecido a counting-sort
 * usando el dígito indicado. Ej: para unidad, usar digito = 0; decena: digito = 1; y así...
 */

void radix_sort_step (int* arreglo, int cantidad, int base, int digito, int max_digitos)
{
    int hcursor, acursor, hhcursor, digito_histograma;
    char *mark;
    
    // Para desenmascarar el dígito dentro del número.
    int potencia_base = potencia (base, digito);
    
    // Histograma acumulado para cada dígito. Lo tenemos que inicializar en ceros, y por eso ocupamos calloc.
    int *hist_acumulado = calloc (base, sizeof (int));
    
    // Necesitamos copiar el arreglo original para proceder al radix sort.
    int *original = malloc (cantidad * sizeof (int));
    memcpy (original, arreglo, cantidad * sizeof (int));
    
    // Recorremos el arreglo para elaborar el histograma.
    for (acursor = 0; acursor < cantidad; acursor++)
        // Le añadimos 1 al índice correspondiente del histograma.
        // Recuerda que (arreglo[acursor] / potencia_base) % base es el valor del dígito que necesitamos ver.
        
        // OJO: ES IMPORTANTE QUE CALCULEMOS LA VARIABLE potencia_base FUERA DE ESTE FOR, SI NO LO HACEMOS ESTE ALGORITMO SE CONVERTIRÍA EN UN theta(n log n)
        hist_acumulado [ (arreglo[acursor] / potencia_base) % base ]++;
    
    // Imprime lo conseguido en el histograma.
    printf ("Ordenar:\n");
    
    for (acursor = 0; acursor < cantidad; acursor++)
    {
        mark = marcar_digito (arreglo[acursor], digito, max_digitos);
        printf ("%s | ", mark);
        free (mark);
    }
    
    printf ("\n*************************************************************\n\
Histograma realizado - base %d, dígito %d:\n\
| # ||", base, digito);
            
    for (hcursor = 0; hcursor < base; hcursor++)
        printf ("%4d|", hcursor);
        
    printf ("\n*************************************************************\n\
| n ||");
            
    for (hcursor = 0; hcursor < base; hcursor++)
        printf ("%4d|", hist_acumulado[hcursor]);
        
    printf ("\n*************************************************************\n\
| N ||");
    
    // Elabora el histograma acumulado.
    for (hcursor = 1; hcursor < base; hcursor++)
        hist_acumulado[hcursor] += hist_acumulado[hcursor - 1];
        
    for (hcursor = base - 1; hcursor > 0; hcursor--)
        hist_acumulado[hcursor] = hist_acumulado[hcursor - 1];
    
    hist_acumulado[0] = 0;
    
    // Imprime el histograma acumulado.
    for (hcursor = 0; hcursor < base; hcursor++)
        printf ("%4d|", hist_acumulado[hcursor]);
        
    printf ("\n*************************************************************\n");
    
    // Ordena de acuerdo al histograma.
    
    for (acursor = 0; acursor < cantidad; acursor++)
    {
        digito_histograma = (original[acursor] / potencia_base) % base;
        // Buscamos en el histograma el número asociado al dígito que buscamos (hist_acumulado[digito_histograma])
        // Y sacamos el elemento apuntado por "acursor", poniéndolo en esa posición en el arreglo ordenado.
        arreglo[ hist_acumulado[digito_histograma]++ ] = original[acursor];
    }
    
    // Liberamos la memoria.
    free (hist_acumulado);
    free (original);
}

int main (int argc, char** argv)
{

    int k;
    
    // Arreglo de los elementos a ordenar
    int scrambled [ITEMCOUNT];
    
    // Para que genere números aleatorios distintos cada vez que se inicie el programa.
    srand (time (0));
    
    for (k = 0; k < ITEMCOUNT; k++)
        scrambled[k] = (rand () % MAXVALUE) + 1;    // Genera valores entre 1 y 10.
    
    // Imprime lo generado acá.
    printf ("Elementos iniciales: \n");
    for (k = 0; k < ITEMCOUNT; k++)
        printf ("%d - ", scrambled[k]);
    
    printf ("\n");
    
    radix_sort_step (scrambled, ITEMCOUNT, BASE, 0, 4);
    radix_sort_step (scrambled, ITEMCOUNT, BASE, 1, 4);
    radix_sort_step (scrambled, ITEMCOUNT, BASE, 2, 4);
    radix_sort_step (scrambled, ITEMCOUNT, BASE, 3, 4);
    
    // Imprime lo generado acá.
    printf ("\nElementos finales: \n");
    for (k = 0; k < ITEMCOUNT; k++)
        printf ("%d - ", scrambled[k]);
    
    printf ("\n");

    return 0;
}
