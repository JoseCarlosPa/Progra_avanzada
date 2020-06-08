/*-------------------------------------------------------------

* SANTA

* Programación avanzada: Manejo de semáforos.

* Fecha: 13 - 05 - 2020

* Autor: Jose Carlos Pacheco Sanchez - A01702828
         Guillermo Carsolio - A01700041

*

*------------------------------------------------------------*/

#include "header.h"

void santa(char *program) {
	int semid;
	key_t key;

	// Recibir correctamente con el key
	if ((key = ftok("/dev/null", 200)) == (key_t) -1) {
		perror(program);
		exit(-1);
	}
	if ((semid = semget(key, 4, 0666))  < 0) {
		perror(program);
		exit(-1);
	}

	printf("Santa Claus ha llegado!! .\n");
	while(1) {
		sem_wait(semid, SEMILLA, 1);
		sem_wait(semid, MUTEX, 1);
		if (deer_santa >= N_REINDEER){
			printf("Preparando el trineo...\n");
			for (int i = 0; i < N_REINDEER; i++){
				sem_signal(semid, SEMILLA_DEER, 1);
			}
			printf("¡FELIZ NAVIDAD!\n");
			deer_santa = 0;
		} else if (elve_santa == 3){
			printf("Santa Claus va ayudar a los elfos.\n");
		}
		sem_signal(semid, MUTEX, 1);
	}
}

int main(int argc, char* argv[]) {
	int semid, pid;
    key_t key;

	if ((pid = fork()) < 0 ) {
		perror("fork");
		return -1;
	} else if (pid == 0) {
		santa(argv[0]);
	} else {
	}
	
	while(1);
	return 0;
}