all: main hijo nieto contador

main: main.c funciones.c
	gcc -o main main.c funciones.c
hijo: hijo.c funciones.c
	gcc -o hijo hijo.c funciones.c
nieto: nieto.c funciones.c
	gcc -o nieto nieto.c funciones.c
contador: contador.c funciones.c
	gcc -o contador contador.c funciones.c
