#include <stdio.h>

/*
 * Esto es una demostración del hashing multiplicativo general.
 */

#define SIZEM       777                // El tamaño del arreglo a hashear.
#define C           0.098609876987598759875              // La constante C a usar. (Elige una entre 0 y 1)

/*
 * Esta función implementa el método multiplicativo usando el método de hasheo
 * multiplicativo. Prueba con distintos C's a ver qué pasa.
 */
unsigned long multi_hash (unsigned long key)
{
    double parte_fraccionaria, parte_entera, modulo;
    
    parte_fraccionaria = C * key;
    parte_entera = (double)(unsigned long) parte_fraccionaria;  // Técnica para redondear hacia abajo.
    
    modulo = parte_fraccionaria - parte_entera; // Esto entrega la parte fraccionaria de C * k.
    
    modulo *= SIZEM;    // Multiplico por M...
    
    return (unsigned long) modulo;  // Y lo redondeo hacia abajo.
}

int main ()
{
    unsigned long key;
    
    printf ("Ingrese llave: ");
    scanf ("%ld", &key);
    
    printf ("La clave generada es %ld.\n", multi_hash (key));
    
    return 0;
}
