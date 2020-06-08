/*-------------------------------------------------------------

* HEADER

* Programación avanzada: Manejo de semáforos.

* Fecha: 13 - 05 - 2020

* Autor: Jose Carlos Pacheco Sanchez - A01702828
         Guillermo Carsolio - A01700041

*

*------------------------------------------------------------*/

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>


#define N_REINDEER 	9
#define	SEMILLA 	0
#define SEMILLA_DEER	1
#define	ELFT 		2
#define MUTEX 		3


int elve_santa;
int deer_santa;

int sem_wait(int semid, int sem_num, unsigned int val) {
	struct sembuf op;
	op.sem_num = sem_num;
	op.sem_op = -val;
	op.sem_flg = 0;
	return semop(semid, &op, 1);
}

int sem_signal(int semid, int sem_num, unsigned int val) {
	struct sembuf op;
	op.sem_num = sem_num;
	op.sem_op = val;
	op.sem_flg = 0;
	return semop(semid, &op, 1);
}

#endif