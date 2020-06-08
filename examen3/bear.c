/*----------------------------------------------------------------

*

* Programación avanzada: Examen 3

* Fecha: 26-May-2020

* Autor: A01702828 - Jose carlos pacheco Sanchez

*

*--------------------------------------------------------------*/

#include "header.h"

// PImero tengo que tener el de crear semaforos para poder ejecutar bear

int main(int argc, char *argv[]) {
	key_t key;
	int sema_id, i, shm_id;
    struct buffer *genera;

    //VALIDACIONES

        if (argc != 1) {
            printf("usage: %s\n", argv[0]);
            return -1;
        }

        if ((key = ftok("/dev/null", 65)) == (key_t) -1) {
            perror(argv[0]);
            return -1;
        }

        if ((sema_id = semget(key, 3, 0666)) < 0) {
            perror(argv[0]);
            return -1;
        }

        if ( (shm_id = shmget(key, sizeof(struct buffer), 0666)) < 0 ) {
            perror("Error en shm_id");
            exit(-1);
        }

	genera = (struct buffer *) shmat(shm_id, (void *) 0, 0);

    // printf("Si llega ahsta aqui!! ");

	printf("Oso: The jar is empty.I will be sleeping while i wait for the honey ..\n");
	while (1) {
		wait_me(sema_id, FULL, 1);
		wait_me(sema_id, MUTEX, 1);
		if (genera->next == genera->tarro) {
			printf("Oso: The jar is full! lets eat!\n");
			señal_sema(sema_id, EMPTY, genera->tarro);
			genera->next = 0;
            printf("Oso: The jar is empty.I will be sleeping while i wait for the honey ..\n");
		} 
		señal_sema(sema_id, MUTEX, 1);
	}
	
	shmdt(genera);
	semctl(sema_id, 0, IPC_RMID, 0);
	shmctl(shm_id, IPC_RMID, 0);


	return 0;
}