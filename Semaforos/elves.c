/*-------------------------------------------------------------

* ELFOS

* Programación avanzada: Manejo de semáforos.

* Fecha: 13 - 05 - 2020

* Autor: Jose Carlos Pacheco Sanchez - A01702828
         Guillermo Carsolio - A01700041

*

*------------------------------------------------------------*/

#include "header.h"

void elve_santas(char *program){
	int semid;
	key_t key;

	// Uso y creacion de elfos en base al create
	if ((key = ftok("/dev/null", 200)) == (key_t) -1) {
		perror(program);
		exit(-1);
	}
	if ((semid = semget(key, 4, 0666))  < 0) {
		perror(program);
		exit(-1);
	}

	// Correr hasta que el semaforo indique lo contrario
	while(1) {
		printf("Elfo preparandose! %d\n", elve_santa);
		int help = rand() % 1;
		if (help == 1){
			sem_wait(semid, ELFT, 1);
			sem_wait(semid, MUTEX, 1);

			elve_santa = elve_santa + 1;

			if (elve_santa == 3){
				sem_signal(semid, SEMILLA, 1);
            } else {
				sem_signal(semid, ELFT, 1);
            }

			sem_signal(semid, MUTEX, 1);
            sleep(1);
			printf("Un elfo %d esta pidiendo ayuda\n", elve_santa);
			sleep(1);
			sem_wait(semid, MUTEX, 1);
			elve_santa = elve_santa - 1;

			// Hasta que el elfo este en ceros salir
			if (elve_santas == 0){
				sem_signal(semid, ELFT, 1);
            }
			sem_signal(semid, MUTEX, 1);
		}
		printf("El elfo %d puede regresar\n", elve_santa);
		sleep(rand() % 3);
	}
}

int main(int argc, char* argv[]) {
	int semid, pid, number_elve_santas = 0;
    key_t key;

	if (argc != 2) {
		printf("usage: %s Numero de elfos\n", argv[0]);
		return -1;
	}
	
	number_elve_santas = atoi(argv[1]);
	if (number_elve_santas < 1) {
		printf("%s: The amount must be a positive integer number.\n", argv[0]);
		return -1;
	}

	// Creacion basica de procesos
	for (int i = 0; i < number_elve_santas; i++) {
		if ((pid = fork()) < 0) {
			perror("fork");
			return -1;
		} else if (pid == 0) {
			elve_santas(argv[0]);
		} else {
		}
	}
	return 0;
}