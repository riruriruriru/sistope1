#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "funciones.h"


static volatile int counter = 0;


int main(){
	while(1){
		signal(SIGINT,controladorNieto);	
	}
	return 0;
}
