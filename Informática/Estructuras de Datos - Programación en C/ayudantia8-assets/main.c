#include "sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUMELEMS    100

int main (int argc, char** argv)
{
    int* arreglo1;
    int* arreglo2;

    int cnt;

    arreglo1 = malloc (NUMELEMS * sizeof (int));
    arreglo2 = malloc (NUMELEMS * sizeof (int));

    srand(time(0));

    for (cnt = 0; cnt < NUMELEMS; cnt++)
    {
        arreglo1[cnt] = random() % 1000000;
        arreglo2[cnt] = random() % 1000000;
    }

    quick_sort(arreglo1, NUMELEMS);
    for (cnt = 0; cnt < NUMELEMS; cnt++)
    {
        printf ("Arreglo 1 (%d):\t%d\n", cnt, arreglo1[cnt]);
    }

    crappy_sort(arreglo2, NUMELEMS);
    for (cnt = 0; cnt < NUMELEMS; cnt++)
    {
        printf ("Arreglo 2 (%d):\t%d\n", cnt, arreglo2[cnt]);
    }
}
