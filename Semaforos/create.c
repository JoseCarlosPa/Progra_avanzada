/*-------------------------------------------------------------

* CREATE

* Programación avanzada: Manejo de semáforos.

* Fecha: 13 - 05 - 2020

* Autor: Jose Carlos Pacheco Sanchez - A01702828
         Guillermo Carsolio - A01700041

*

*------------------------------------------------------------*/

#include "header.h"

int main(int argc, char* argv[]) {
	int semid, i;
    key_t key;

	// validaciones 
	if (argc != 1) {
		printf("usage: %s\n", argv[0]);
		return -1;
	}
	if ((key = ftok("/dev/null", 200)) == (key_t) -1 ) {
		perror(argv[0]);
		return -1;
	}
	if ((semid = semget(key, 4, 0666 | IPC_CREAT)) < 0 ) {
		perror(argv[0]);
		return -1;
	}

    // Inciocar contadores
	elve_santa = 0;
	deer_santa = 0;

	// Declaracion de Semaforos
    semctl(semid, SEMILLA, SETVAL, 0);
    sleep(1);
    semctl(semid, SEMILLA_DEER, SETVAL, 0);
    sleep(1);
    semctl(semid, ELFT, SETVAL, 1);
    sleep(1);
    semctl(semid, MUTEX, SETVAL, 1);
	return 0;
}