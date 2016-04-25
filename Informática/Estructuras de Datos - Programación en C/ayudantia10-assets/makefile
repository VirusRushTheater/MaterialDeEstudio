all:	knuth multi dijkstra count_sort radix_sort
		

knuth:		hash_multiplicativo_knuth.c
			gcc hash_multiplicativo_knuth.c -o hash_multiplicativo_knuth
			
multi:		hash_multiplicativo.c
			gcc hash_multiplicativo.c -o hash_multiplicativo

dijkstra:	dijkstra.c
			gcc dijkstra.c -o dijkstra
			
count_sort:	counting_sort.c
			gcc counting_sort.c -o counting_sort
			
radix_sort:	radix_sort.c
			gcc radix_sort.c -o radix_sort

# Limpiar
clean:		
			rm -rf hash_multiplicativo_knuth hash_multiplicativo dijkstra radix_sort counting_sort
