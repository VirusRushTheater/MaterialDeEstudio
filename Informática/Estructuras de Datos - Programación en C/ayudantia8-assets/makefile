# Variables: compilador
CC = gcc
CFLAGS = -g

OBJECTS = main.o sort.o
HEADERS = sort.h
OUTPUT = sort

# Comandos: El all es lo primero que se corre con el comando make.
all:	$(OBJECTS)
	$(CC) $(OBJECTS) -o $(OUTPUT)

# Esto se ejecuta recursivamente.
main.o:	main.c
	$(CC) -c $(CFLAGS) main.c
	
sort.o:	sort.c
	$(CC) -c $(CFLAGS) sort.c
	
# Se incluye por buena costumbre: borra los archivos generados por el makefile.
clean:	
	rm -f $(OBJECTS) $(OUTPUT)
