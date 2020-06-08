/*----------------------------------------------------------------

*

* Programación avanzada: Examen 3

* Fecha: 26-May-2020

* Autor: A01702828 - Jose carlos pacheco Sanchez

*

*--------------------------------------------------------------*/

#include "header.h"

// Primero tengo que haber eerjcutado bees

void create_abeja(int id) {
	key_t key;
	int sema_id, shm_id, i, num;
    struct buffer *genera;
	
        if ((key = ftok("/dev/null", 65)) == (key_t) -1) {
            perror("ftok");
            exit(-1);
        }
        if ((sema_id = semget(key, 3, 0666)) < 0) {
            perror("Error en sema_id");
            exit(-1);
        }
        if ((shm_id = shmget(key, sizeof(struct buffer), 0666)) < 0 ) {
            perror("Error en shm_id");
            exit(-1);
        }
	
	genera = (struct buffer *) shmat(shm_id, (void *) 0, 0);
    genera->abejas++;
	
	srand(getpid());
	while (1) {

		sleep(1);
        printf("Abeja %i: Im goiing to put one portion\n", id);
        wait_me(sema_id, EMPTY, 1);
        sleep(1);
        wait_me(sema_id, MUTEX, 1);
        genera->portions[genera->next++] = 1;
        // printf(" Valor generado %i",genera);
        if(genera->next >= genera->tarro){
            printf("Abeja %i: The buffer is full!...\n", id);
            señal_sema(sema_id, FULL, 1);
        } 
        printf("Abeja %i: I already put the portion. There are %i portions of %i\n", id, genera->next, genera->tarro);
        señal_sema(sema_id, MUTEX, 1);
        sleep(1);
        señal_sema(sema_id, FULL, 1);
	}
	shmdt(genera);


	exit(0);
}

int main(int argc, char *argv[]) {

    // Receurda el sem_id
	int pid;
	
        if (argc != 2) {
            printf("usage: %s numero_abejas\n", argv[0]);
            return -1;
        }

	// printf("Valor en argv[1] es %s, argv[1]);

    int number_bees = atoi(argv[1]);
    // printf("Valor en argv[1] es %i, number_bees);

        if (number_bees <= 0) {
            printf("%s: Tiene que ser un numero positivo\n", argv[0]);
            return -1;
        }


	for (int i = 0; i < number_bees; i++) {
		if ((pid = fork()) < 0) {
			perror("fork");
			exit(-1);
		} else if (pid == 0) {
			create_abeja(i);
		} else {
		}
	}

	/*
	 * int i = 0;
	 While(1){
	  kill(create_abeja(i), SIGTERM);
	  i++;
	 exit(0);
	 }
	 */


	return 0;
}