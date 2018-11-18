#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <signal.h>
#include <stdlib.h>
#include "Signal.h"
#include "funciones.h"


static volatile int keepRunning = 1;



int main(int argc, char *argv[]){

	int numHijos = 0, flag = 0;
	pid_t *arrayPID;
	char *string;
	Signal *sig;
	getArguments(argc, argv, &numHijos, &flag);
	sig = (Signal *)malloc(sizeof(Signal));//se le da memoria a la estructura Signal
	string = (char *)malloc(25*sizeof(char));
	arrayPID = (pid_t *) malloc(numHijos*sizeof(pid_t));
	createSons(numHijos, arrayPID, flag);//se ejecuta el procedimiento que crea hijos
	signal(SIGINT,handler);	//Se agrega nuevamente para poder eliminar los archivos de contador
	int comprobacion = 0;
	while(keepRunning){//ciclo while para que el padre envie señales indefinidamente hasta que se cierre el proceso con ctrl + c
		do{
			printf("Ingresar numero de hijo y señal a enviar (X-Y):\n");
			scanf("%s",string);
			comprobacion=comprobarSenial(string,sig,numHijos);//se comprueba si la señal ingresada por el usuario tiene el formato correcto
			if(comprobacion==0){
				printf("Señal ingresada de manera incorrecta, intente otra vez\n");
				}
		}while(comprobacion==0);//si la comprobacion falla, se pide una nueva señal
		
		printf("La señal %d será enviada al hijo %d de pid %d, \n",sig->sName,sig->numHijo,arrayPID[sig->numHijo-1]);
		sendSignal(sig, arrayPID, numHijos);//se manda la señal ya comprobada
		getchar();
		
		}
	for(int i=0;i<numHijos;i++){ 
		exit(0);
		wait(NULL); 
	}
	printf("FINALIZANDO...\n");
	//exit(0);
	
	}
