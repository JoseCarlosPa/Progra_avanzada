/*----------------------------------------------------------------

*

* Programación avanzada: Examen 3

* Fecha: 26-May-2020

* Autor: A01702828 - Jose carlos pacheco Sanchez

*

*--------------------------------------------------------------*/


#include "header.h"

// Este va primero para crear los semaforso

int main(int argc, char *argv[]) {

    //Decalra -> Busca el sema_id
	key_t key;
	int sema_id, shm_id;

        if (argc != 2) {
            printf("Hace falta: %s tamanio_del_tarro\n", argv[0]);
            return -1;
        }
        if ((key = ftok("/dev/null", 65)) == (key_t) -1) {
            perror(argv[0]);
            return -1;
        }
        if ((sema_id = semget(key, 3, 0666 | IPC_CREAT)) < 0) {
            perror(argv[0]);
            return -1;
        }

    int tarro_tam = atoi(argv[1]);

	//printf(\n El tamaño del tarro es %i \n, tarro_tam);

	semctl(sema_id, FULL, SETVAL, 0);
	sleep(1);
	semctl(sema_id, EMPTY, SETVAL, tarro_tam);
    sleep(1);
	semctl(sema_id, MUTEX, SETVAL, 1);


	if ((shm_id = shmget(key, sizeof(struct buffer), 0666 | IPC_CREAT)) < 0 ) {
		semctl(sema_id, 0, IPC_RMID, 0);
		perror("Error en shm_id");

		return -1;
	}

    //printf(\n El tamaño del tarro es %i \n, tarro_tam);


    // Ejemplo en clase con santa y elfos
	struct buffer *genera;

	genera = (struct buffer*) shmat(shm_id, (void*) 0, 0);
	genera->next = 0;
    genera->tarro = tarro_tam;
	memset(genera->portions, 0, tarro_tam * sizeof(int));
	shmdt(genera);
	printf("Semaforos iniciados y creados, iniciando procesos...\n");


	return 0;
}