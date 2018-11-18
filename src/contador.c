#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <stdlib.h>
#include "funciones.h"

int main(int argc, char *argv[]){
	//argv[1] contiene el pid del proceso que recibio la señal para llamar a contador
	contadorHijo(argv[1]);
	exit(0);
}
