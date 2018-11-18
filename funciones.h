#include "Signal.h"

#ifndef FUNCIONES_H
#define FUNCIONES_H

int comprobarSenial(char *strSignal, Signal *sig, int numHijos);
void contadorHijo(char *pid);
void controlador(int signum);
void controladorNieto(int signum);
void createSons(int numHijos, int *arrayPID, int flag);
void getArguments(int argc, char *argv[], int *numHijos, int *flag);
void handler(int signum);
int sendSignal(Signal *s, int *arrayPID, int numHijos);

#endif
