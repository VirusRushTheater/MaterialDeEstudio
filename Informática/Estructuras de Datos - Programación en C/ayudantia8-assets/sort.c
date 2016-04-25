#include "sort.h"
#include <stdlib.h>

void crappy_sort(int *array, int length)
{
	/*
	Esta función es una variante del insertion sort, de complejidad 0(n^2).
	En cada iteración se busca el máximo elemento y se pone al final. Luego
	se repite la búsqueda, bloqueando ese elemento.
	*/

	int final, cursor, maxindex, swaptmp;

	//Se recorre desde el primero hasta el último.
	for(final = length - 1; final >= 0; final++)
	{
		//Búsqueda del mayor
		maxindex = 0;

		for(cursor = 0; cursor <= final; cursor++)
			if(array[cursor] > array[maxindex])
				maxindex = cursor;

		//Y se intercambia el último elemento por el máximo encontrado.
		swaptmp = array[final];
		array[final] = array[maxindex];
		array[maxindex] = swaptmp;
	}
}

/*
void crappy_sort_rec(int *array, int length)
{

	//Diseño recursivo del algoritmo anterior.


	int final, maxindex, swaptmp;

	//Si se ordena una lista de más de un elemento...
	if(length > 1)
	{
		//Búsqueda del mayor
		maxindex = 0;

		for(cursor = 0; cursor <= final; cursor++)
			if(array[cursor] > array[maxindex])
				maxindex = cursor;

		//Y se intercambia el último elemento por el máximo encontrado.
		swaptmp = array[length - 1];
		array[length - 1] = array[maxindex];
		array[maxindex] = swaptmp;

		crappy_sort_rec(array, length - 1);
	}
}
*/

void quick_sort_rec(int *array, int beg, int end)
{
	/*
	Implementación en C del Quick Sort recursivo.
	En este caso se efectúa un Randomized Quick Sort, en el cual el pivote se elige al azar.
	*/

	int pivot, cleft, cright, advdirection, swap;

	//Si la lista es de más de un elemento (cuando es uno, end y beg son iguales) ...
	if(end > beg)
	{
		cleft = beg;
		cright = end;

		//Se elige un número al azar entre beg y end.
		pivot = (random() % (end - beg)) + beg;

		//Se planea avanzar desde la izquierda (0) por default.
		advdirection = 0;

		while(cleft != cright)
		{
            // Hacia la izquierda
			if(advdirection == 0)
			{
                if (array[cleft] > array[pivot])
                {
                    // Intercambiamos los elementos
                    swap = array[pivot];
                    array[pivot] = array[cleft];
                    array[cleft] = swap;

                    pivot = cleft;
                    advdirection = 1;
                }
                else
                    cleft++;
			}
			// Hacia la derecha
			else if(advdirection == 1)
			{
                if (array[cright] < array[pivot])
                {
                    // Intercambiamos los elementos
                    swap = array[pivot];
                    array[pivot] = array[cright];
                    array[cright] = swap;

                    pivot = cright;
                    advdirection = 0;
                }
                else
                    cleft++;
			}
		}
		quick_sort_rec(array, beg, pivot - 1);
		quick_sort_rec(array, pivot + 1, end);
	}
}

void quick_sort(int *array, int length)
{
	quick_sort_rec (array, 0, length - 1);
}
