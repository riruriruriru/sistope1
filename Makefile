
all: clean bin/main bin/hijo bin/nieto bin/contador 

bin/main: obj/main.o obj/funciones.o
	gcc obj/main.o obj/funciones.o -o bin/main
	
bin/hijo: obj/hijo.o obj/funciones.o
	gcc obj/hijo.o obj/funciones.o -o bin/hijo
	
bin/nieto: obj/nieto.o obj/funciones.o
	gcc obj/nieto.o obj/funciones.o -o bin/nieto
	
bin/contador: obj/contador.o obj/funciones.o
	gcc obj/contador.o obj/funciones.o -o bin/contador
	
obj/main.o:
	gcc -c -Iinclude src/main.c -o obj/main.o

obj/hijo.o:
	gcc -c -Iinclude src/hijo.c -o obj/hijo.o

obj/nieto.o:
	gcc -c -Iinclude src/nieto.c -o obj/nieto.o

obj/contador.o:
	gcc -c -Iinclude src/contador.c -o obj/contador.o
	
obj/funciones.o:
	gcc -c -Iinclude src/funciones.c -o obj/funciones.o

.PHONY: clean

clean:
	rm -f obj/*.o *~ core $(INCDIR)/*~ 
