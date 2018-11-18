#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "funciones.h"
#include "Signal.h"
#include "Shared.h"

//Función que es ejecutado al momento de recibir las señales especificadas, SIGUSR1,SIGUSR2,SIGTERM y SIGINT
//Entradas: Entero que representa el numero de la señal que fue escuchada por el proceso hijo correspondiente
//Salidas: Vacio
void controlador(int signum){
	
	//Comprueba si la señal escuchada es SIGUSR1
	if (signum == SIGUSR1){
		printf("Soy el hijo %d y recibí SIGUSR1\n", getpid());
		int pidF = getpid();
		int pid = 0;
		char id[12];
		sprintf(id, "%d", pidF);
		
		//Crea un proceso hijo para que ejecute contralador
		if ((pid=fork())==0){
			if(getpid()!=0){
				
				//Se utiliza execv para ejecutar controlador y manda el pid del padre de este proceso hijo temporañ
				char *argv[] = {"./contador", id,NULL};
				execv("contador", argv);
			}
		}
	}
	
	//Comprueba si la señal escuchada es SIGUSR2
	else if(signum == SIGUSR2){
		printf("Soy el hijo %d y recibí SIGUSR2\n", getpid());
		int pid = 0;
		if((pid=fork())==0){
			if(getpid()!=0){
				char *argv[] = {"./nieto", NULL};
				execv("nieto", argv);
				}
			
			}
		
		}
	//Comprueba si la señal escuchada es SIGTERM
	else if(signum == SIGTERM){
		printf("Soy el hijo %d y recibí SIGTERM\n", getpid());
		printf("Soy el hijo con pid: %d y mi papá me quiere matar\n" ,getpid());
		exit(0);
		}
		
	//Comprueba si la señal escuchada es SIGINT al momento de presionar Ctrl+c
	else if(signum == SIGINT){
		//Primera advertencia al presionar Ctrl+c
		if(counterHijo==0){
			printf("Soy el hijo con pid: %d, y estoy vivo aún. No me mates papá\n", getpid());
			counterHijo++;
			}
		//La segunda vez que se presiona Ctrl + c se termina el proceso	
		else if(counterHijo==1){
			exit(0);
			}
		
		}
	
}


//Función que es ejecutado al momento de recibir la señale especificada SIGINT, recibe solo esta ya que es un preceso nieto
//Entradas: Entero que representa el numero de la señal que fue escuchada por el proceso correspondiente
//Salidas: Vacio
void controladorNieto(int signum){
	
	if(signum==SIGINT){
		//Primer advertencia
		if(counterNieto==0){
			printf("Soy el nieto con pid: %d y estoy vivo aún. No me mates abuelo\n", getpid());
			counterNieto++;
			}
		//La segunda vez que se preciona Ctrl + c se termina el proceso
		else if(counterNieto==1){
			exit(0);
			}
		}
	
}

//Función contador que generá un archivo con el nombre del proceso que lo llamó y guarda las veces que ha sido llamado por el proceso
//Entradas: String que representa el pid del proceso que llamó a este proceso
//Salidas: Vacio
void contadorHijo(char *pid){
	int auxCont;
	char directorio[50];
	char archivo[20];
	
	//Se copia el directorio donde esta el archivo en la variable directorio
	strcpy(directorio,"temp/");
	strcpy(archivo,pid);
	strcat(archivo,".txt");
	strcat(directorio,archivo);
	
	if (opendir("temp")==NULL){ //Verifica si no existe la carpeta temporal temp, que guarda los archivos para el contador
		system("mkdir temp");   //Crea la carpeta temp
	}
	FILE *cont;
	cont = fopen(directorio,"r");
	
	if (cont == NULL){ //Verifica si el archivo de nombre del pid del proceso no existe
		cont = fopen(directorio,"w");//Si no existe el archivo lo crea 
		fprintf(cont,"%d",1);
		printf("pid: %s, y he recibido esta llamada 1 vez.\n",pid);
		fclose(cont);
	}
	
	//Si existe el archivo, recupera el numero le suma uno, lo muestra por pantalla y guarda ese numero
	else{
		fscanf(cont,"%d",&auxCont);
		auxCont+=1;
		printf("pid: %s, y he recibido esta llamada %d veces.\n",pid,auxCont);
		fclose(cont);
		FILE *cont;
		cont = fopen(directorio,"w");
		fprintf(cont,"%d",auxCont);
		fclose(cont);
		
	}
}




//Funcion que comprueba si una señal fue enviada correctamente verificando que se envien por pantalla solo señales de numero 15, 16 y 17, es decir sigusr1, sigusr2 y sigterm
//SIGINT solo se recibe cuando se presiona ctrl+c, por lo tanto no se considera como opcion para mandarlo por pantalla
//Entradas: estructura vacia de tipo Signal y string con todo lo que escribio el usuario por pantalla
//Salidas: Entero igual a 1 si la señal cumple con todos los parametros o entero igual a 0 si la señal no cumple con el formato
int comprobarSenial(char *strSignal, Signal *sig, int numHijos){
	//printf("INICIO COMPROBAR SEÑAL\n");
	char *strAux, *strAux2, *aux3;
	int i = 0,bool=0, cont=0, contNumber=0,contSig=0, nChild=0, nSig=0; //se inicializan variables
	for(i=0;i<25;i++){ //se recorre el string ingresado por el usuario
		if(bool==0&&strSignal[i]!='-'){ //se cuenta el numero de digitos del numero de hijo ingresado
			contNumber++;
			}
		else if(bool==1&&strSignal[i]!='-'){ //se cuenta el numero de digitos de la señal ingresada
			contSig++;
			}
		if(strSignal[i]=='-'){//cuando se encuentra el guion, se cambia el valor de variable bool para buscar el numero de la señal ingresada
			bool = 1;
			}
		
		}
	if(bool==1){//si bool = 1, la señal se encuentra separada por un guion y por lo tanto se procede a parsear sus componentes
		bool = 0;
		strAux = (char *)malloc(contNumber*sizeof(char));
		strAux2 = (char *)malloc(contSig*sizeof(char));
		for(i=0;i<25;i++){ //se recorre otra vez el string
			if(bool==0&&strSignal[i]!='-'){ //se ingresan a un arreglo los caracteres que constituyen el numero del hijo
				strAux[cont]=strSignal[i];
				cont++;
			}
			else if(bool==1&&strSignal[i]!='-'){//se ingresan a un arreglo los caracteres que constituyen el numero de la señal
				strAux2[cont]=strSignal[i];
				cont++;
				}
			if(strSignal[i]=='-'){//se reinicia contador al leer un guion
				bool = 1;
				cont = 0;
				}
			}
		nChild = strtol(strAux, &aux3, 10);
		nSig = strtol(strAux2, &aux3, 10); //se castean los arreglos de caracteres con strtol
		//printf("%s - %s\n",strAux,strAux2);
		//printf("numero hijo: %d\n",nChild);
		//printf("señal; %d\n", nSig);
		if(nChild==0||nSig==0||nChild<=0||nChild>numHijos){ //si la conversion falla, strtol retorna 0, por lo tanto se retorna con un error
			return 0;
			}
		else{//si se cumplen todas las condiciones, se guarda la señal en la estructura signal
			sig->numHijo = nChild;
			sig->sName = nSig;
			}
		return 1;
		}
	else{ //si bool == 0, no se cumple con el formato N-S, por lo tanto se retorna con un error
		return 0;
		}
	}
//Procedimiento que se encarga de crear hijos respecto al proceso principal padre y segun los valores ingresados en getopt
//Entradas: numero de hijos, arreglo para guardar los PID de los hijos creados, identificador de ingreso de bandera
//Salidas: procedimiento tipo void
void createSons(int numHijos, int *arrayPID, int flag){
	if(flag==1){//si se ingreso el flag -m por getopt, el valor de flag deberia ser igual a 1 y por lo tanto se deberia imprimir la informacion de los hijos
		printf("Mostrando informacion por pantalla:\n");
		}
	int i = 0, pid=0;
	for(i = 0; i<numHijos;i++){
		if((pid=fork())==0){	//se crea un hijo y en el mismo proceso hijo recien creado se entra al if
			if(getpid()!=0){ 
				if(flag==1){ //si el identificador de flag es igual a 1, se muestra la informacion del hijo
					printf("Número: %d, PID: %d\n",1+i,getpid());
				}
				//char *argv[] = { "gcc", "-c", "-o", "hijo.o", "hijo.c", 0 };
				execl("./hijo","","", (char *)0); //se ejecuta un codigo correspondiente al hijo
				
			}
			else{
				//printf("UWU\n");
			}
		}
		else{
			//en el proceso padre se guarda el pid del hijo recien creado
			arrayPID[i]=pid;
			}
		}
	}
//Funcion que se encarga de recibir informacion de la señal y los hijos para enviar una señal especifica a un hijo indicado por el usuario
//Entradas: estructura de tipo Signal que contiene la señal parseada indicando el hijo destino y la señal enviada, el arreglo del pid de los hijos y el numero de hijos creados inicialmente
//Salida: int identificador que indica si la señal pudo ser enviada correctamente o no (1 caso exitoso, 0 cuando se produce un fallo)
int sendSignal(Signal *s, int *arrayPID, int numHijos){
	if(s->numHijo-1>numHijos||s->numHijo<=0){//si se intenta mandar una señal a un hijo inexistente, se retorna un 0
		printf("Hijo invalido\n");
		return 0;
		}
	if(arrayPID[s->numHijo-1]==-1){//Si se intenta mandar una señal a un hijo que fue creado pero ya murio, se indica como fallo y no se manda la señal
		printf("Hijo ya esta muerto\n");
		return 0;
		}
	//printf("hijo destino pid: %d\n", arrayPID[s->numHijo-1]);
	if(s->sName==15||s->sName==16||s->sName==17){//se comprueba que la señal mandada corresponda a las indicadas por enunciado: 15, 16 y 17
		if(s->sName==16){//si la señal es 16:
			kill(arrayPID[s->numHijo-1],SIGUSR1);//se manda la señal 10, que SIGURS1 al hijo correspondiente
			}
		else if(s->sName==15){//si la señal es 15
			kill(arrayPID[s->numHijo-1],SIGTERM);//se manda señal sigterm al hijo correspondiente 
			arrayPID[s->numHijo-1]=-1;//se setea el valor del hijo muerto como -1 en el arreglo de PID
			
			}
		else{//si la señal es 17
			kill(arrayPID[s->numHijo-1],SIGUSR2);//se manda la señal SIGURS2 al hijo correspondiente
			}
		
		return 1;
		}
	return 0;
	}
//Procedimiento que se encarga de reconocer y procesar los Ctrl + c ingresados por terminal
//Entradas: entero correspondiente a la señal recibida
void handler(int signum){
	//printf("RECIBI UN %d\n",signum);
	if(signum==SIGINT){ //si se recibe un signum, se aumenta el contador global
		counterPadre++; //Numero de veces que se presionó Ctrl + c
		}
	
	
	
	//se reconoce cuando se apreta por segunda vez Ctrl + c
	if(counterPadre==2){
		if (opendir("temp")==NULL){
			
			exit(0);//se ejecuta un exit que cerrara el proceso principal
			
		}
		else{
			system("rm -r ./temp"); //función para eliminar la carpeta con los archivos del contador
			exit(0);//se ejecuta un exit que cerrara el proceso principal
			
		}
	}
}
//Procedimiento que usa getopt para recibir argumentos respecto al numero de hijos y el flag -m que indica si debe mostrarse informacion por pantalla o no
//Entradas: recibe el numero de argumentos ingresados, los argumentos y un puntero a las variables numHijos y flag para modificarlas en el scope global del proceso
void getArguments(int argc, char *argv[], int *numHijos, int *flag){
	int flags, opt;
	char *aux3;
	aux3 = malloc(10*sizeof(char));
	if(argc <3){//si se ingresa un numero de argumentos menor a 3, se finaliza la ejecucion del programa
		printf("Se ingreso un numero incorrecto de argumentos\n");
		exit(EXIT_FAILURE);
		}
	int nChild = -1;
	flags = 0;
	while ((opt = getopt(argc, argv, "mh:")) != -1) {
	   switch (opt) {
	   case 'm'://se busca el flag -m, en caso de ser encontrado se setea el valor flags = 1, no se considera lo que se ingrese despues del flag -m
		   flags = 1;
		   break;
	   case 'h': //se busca el flag -h
		   nChild = strtol(optarg, &aux3, 10);//se parsea el argumento ingresado junto al flag -h a entero
		   if(optarg!=0 && nChild==0){//si no se ingresa un argumento junto a -h o si no se logra parsear el argumento ingresado, se considera como invalido
				fprintf(stderr, "Uso correcto: %s [-h nchild] [-m]\n", argv[0]);
				exit(EXIT_FAILURE);
			   }
		   //printf("optarg: %s\n", optarg);
		   break;
	   default: /* '?' */
		   fprintf(stderr, "Uso correcto: %s [-h nchild] [-m]\n",
				   argv[0]);
		   exit(EXIT_FAILURE);
	   }
	}

	if(flags==1){//si se encontro un flag -m, se setea la variable global flag = 1, respecto al scope del proceso principal
		(*flag) = 1;
		}
	(*numHijos) = nChild; //se iguala la variable numHijos a nChild
	if(nChild<0){
		fprintf(stderr, "Usage: %s [-h nchild] [-m]\n", argv[0]); //si la cantidad de hijos es negativa, se retorna un error
		exit(EXIT_FAILURE);
		}


}
