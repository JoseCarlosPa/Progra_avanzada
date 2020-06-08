
/*----------------------------------------------------------------

*

* Programación avanzada: Manejo de procesos

* Fecha: 31-Marzo-2020

* Autor: Jose Carlos Pacheco Sanchez - A01702828

*		 Guillermo Carsolio - a01700041

*

*--------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

//Funcion que nos permitira crear nuevos procesos
void crearProceso(int actual, int nivel){
	int i = 0, j = 0, ahora = 0, pid = 0, ppid = 0;
	if(actual < nivel){
		for(i = 0; i < actual; i++){
			fork();  //Creacion de proceso y agarrar los PIDs y PPIDs
			pid = getpid(); 
            ppid = getppid();

			//Para cumplir con los TABS 
			for(j = 0; j < actual; j++){
				printf("\t");
			}

			printf("PPID = %i PID = %i NIVEL = %i\n", pid, ppid, actual);
			ahora = actual + 1; //Contador para no ciclarnos
			crearProceso(ahora, nivel); //Recursividad 
		}
		wait(NULL);
	} else {

		//Lo mismo pero con el sigueinte nivel
		for(i = 0; i < nivel; i++){
			fork();
			pid = getpid(); 
            ppid = getppid();
			//sleep(10);
			wait(NULL);
			for(j = 0; j < nivel; j++){
				printf("\t");
			}
			printf("PPID = %i PID = %i NIVEL = %i\n", ppid, pid, nivel);
		}
	}
}

int main(int argc, char *argv[]){
	int num = 0;
	//Validaciones 
	if(argc != 2){
		fprintf(stderr, "usage: descending number\n");
		return -2;
	}
	num = atoi(argv[1]); //Pasar a entero el string
	if(num < 1){
		fprintf(stderr, "descending: the parameter must be a positive integer\n");
		return -3;
	}
	printf("PPID = %i PID = %i NIVEL = 0\n", getppid(), getpid());

	crearProceso(1, num); //Primera llamada con el numero ingresado 
	return 0;
}
