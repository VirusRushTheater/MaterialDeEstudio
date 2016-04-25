#include <stdio.h>

/*
 * Esto es una demostración del hashing multiplicativo implementado usando
 * el método de Knuth. Los detalles están explicados en la guía de ayudantía.
 * 
 * Modifica estos "defines" para que el algoritmo cambie su comportamiento.
 */

#define EXPM        8                  // p = log2 M
#define SIZEM       potencia(2, EXPM)   // M = 2^p (tamaño del arreglo)

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
 * Esta función implementa el método multiplicativo usando el algoritmo de
 * Knuth. La gracia de un buen hash es que:
 * - A partir de dos números distintos se generen dos hashes distintos.
 * - Consecuencia de ello, no haya colisiones.
 * - Los hashes sean lo menos reversibles que se pueda. (Teniendo un hash sea
 * difícil adivinar la clave que lo generó)
 */
unsigned long knuth_hash (unsigned long key)
{
    unsigned long prod, s, w, M;
    unsigned int ret;
    double C = 0.6180339887498949;  // Número áureo.
    
    M = SIZEM;              // Mira el define: M = 2^p
    w = sizeof (int) * 8;   // w = tamaño en bits del tipo int.
    
    s = (unsigned int) (C * (double) potencia (2, 32));
    //s = 2654435769; //s = C * 2^32
    
    prod = key * s;
    prod = prod & 0x00000000FFFFFFFF; //Tomamos los 32 bits más bajos
    
    ret = prod & ((potencia (2, EXPM) - 1) << (w - EXPM)); //Tomamos los EXPM bits superiores
    
    return ret >> (w - EXPM); //Los bajamos de bits.
}

int main ()
{
    unsigned long key;
    
    printf ("Ingrese llave: ");
    scanf ("%ld", &key);
    
    printf ("La clave generada es %ld.\n", knuth_hash(key));
    
    return 0;
}
