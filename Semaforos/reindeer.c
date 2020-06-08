/*-------------------------------------------------------------

* RENOS

* Programación avanzada: Manejo de semáforos.

* Fecha: 13 - 05 - 2020

* Autor: Jose Carlos Pacheco Sanchez - A01702828
         Guillermo Carsolio - A01700041

*

*------------------------------------------------------------*/

#include "header.h"

void reindeers(char *program) {
	int semid;
	key_t key;
	
	if ((key = ftok("/dev/null", 200)) == (key_t) -1) {
		perror(program);
		exit(-1);
	}
	if ((semid = semget(key, 4, 0666))  < 0) {
		perror(program);
		exit(-1);
	}

	// Seguir hastaque el semaforo indique
	while(1){
		printf("UN Reno %d\n", deer_santa);
		sem_wait(semid, MUTEX, 1);
		deer_santa++;
		if(deer_santa == N_REINDEER){
			sem_signal(semid, SEMILLA, 1);
        }
        sleep(1);
		sem_signal(semid, MUTEX, 1);
		sem_wait(semid, SEMILLA_DEER, 1);

		printf("Reno %d listo\n", deer_santa);
		sleep(1);

	}
}

int main(int argc, char* argv[]) {
	int semid, pid;
    key_t key;
	
	for (int i = 0; i < N_REINDEER; i++) {
		if ((pid = fork()) < 0) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			reindeers(argv[0]);
		} else {
		}
	}
	return 0;
}