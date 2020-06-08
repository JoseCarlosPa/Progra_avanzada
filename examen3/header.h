/*----------------------------------------------------------------

*

* Programación avanzada: Examen 3

* Fecha: 26-May-2020

* Autor: A01702828 - Jose carlos pacehco Sanchez

*

*--------------------------------------------------------------*/

#ifndef HEADER_H

#define HEADER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>


#define	FULL  0
#define EMPTY 1
#define MUTEX 2



/*
 * Pasos a seguir
 * 1.- Ejecutar tarro_create_sema.c para iniciar la matriz
 * 2.- Ejecutar bear.c
 * 3.- Ejectuar beas.c
 *
 *
 */


struct buffer {

    int tarro;
    int abejas;
    int next;
	int portions[];
};


int señal_sema(int sema_id, int sem_num, unsigned int valor) {

	struct sembuf op;
	op.sem_num = sem_num;
	op.sem_op = valor;
	op.sem_flg = 0;
	return semop(sema_id, &op, 1);
}

int wait_me(int sema_id, int sem_num, unsigned int valor) {

    struct sembuf op;
    op.sem_num = sem_num;
    op.sem_op = -valor;
    op.sem_flg = 0;
    return semop(sema_id, &op, 1);
}


#endif