/*

*

* Programacion avanzada : Examen parcial No.2 

* Author: Jose Carlos Pacheco Sanchez - A01702828

*/


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <limits.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <math.h>

//Variables globales
char *nombre,*output;
int contador = 1;


float find_media( ){

	int fd, *data ,numbytes;
	float suma = 0,media;
	unsigned char buffer;

	char index[2];
	sprintf(index,"%d",contador);
	strcat(nombre, index);
	strcat(nombre, ".dat");
	printf("\n\nAbriendo: %s para calcular la media:",nombre);
	if ( (fd = open(nombre, O_RDONLY)) < 0 ) {
		perror("No puedo abrir el archivo");
		return -1;
	}

 	while ((numbytes = read(fd, &buffer, sizeof(char))) > 0){
		 if(buffer != 0){
			suma = suma + buffer;
		 }	
    }

	 media = suma/100;
	 close(fd);

	return media; //regresamosla media
}

float find_varianza(float media){

	int fd, *data ,numbytes;
	float sumando = 0,varianza;
	unsigned char buffer;

	char index[2];

	printf("\n\nAbriendo: %s para calcular la varianza:",nombre);

	if ( (fd = open(nombre, O_RDONLY)) < 0 ) {
		perror("No puedo abrir el archivo");
		return -1;
	}

 	while ((numbytes = read(fd, &buffer, sizeof(char))) > 0){
		 if(buffer != 0){
			sumando = sumando + pow(fabsf(buffer-media),2);
		 }	
    }
	varianza = sumando/99;
	 close(fd);
	return varianza;//regresamosla varianza
}


void processTwo(int sig){

		printf("\nSOY EL PROCESO No.2 : %i TRABAJANDO .. \n", getpid());

		int fd, *data,numbytes;
		char index[2];
		unsigned char mybuffer;

		sprintf(index,"%d",contador);
		strcat(output, index);
		strcat(output, ".dat");
		
		printf("\nEscribiendo datos en -> %s ",output);

	
		if ( (fd = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
			perror("open");
			
		}

		//Asignamos el espacio de memoria y guardamos e

		data = (int*) malloc(100);
		data[0] = find_media();
		data[1] = find_varianza(data[0]);
		write(fd,data,16);

		printf("\nLa media es: %i",data[0]);
		printf("\nLa varainza es: %i\n\n",data[1]);
		close(fd);
		free(data);

}

void processOne(int sig){

		int fd, i,*data;
		char index[2];

		printf("\nSOY EL PROCESO No.1 : %i TRABAJANDO .. \n", getpid());
        sprintf(index,"%d",contador);
        strcat(nombre, index);
        strcat(nombre, ".dat");
		
		printf("\nfile: %s -> ",nombre);

		srand( 12345 );
		data = (int*) malloc(100 * sizeof(int));
		for (i = 0; i < 100; i++) {
			data[i] = rand() % (100) + 1;
			printf(" %i",data[i]);
		}

		if ( (fd = open(nombre, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
			perror("open");
			
		}
		write(fd, data, 100 * sizeof(int));
		close(fd);
		free(data);

	
}

void child_process() {
	signal(SIGUSR1, processOne);
	signal(SIGUSR2, processTwo);
	pause();
	exit(0);
}

int main(int argc, char* argv[]) {

	int pid,i,data[2],timeout;

    if (argc != 4) {
		fprintf(stderr, "usage: %s timeout prefix_input prefix_output\n", argv[0]);
		return -1;
	}

	timeout = atoi(argv[1]);
	if (timeout <= 0) {
		fprintf(stderr, "%s: el tiemout tiene que ser un numero positivo.\n", argv[0]);
		return -2;
	}


	nombre = argv[2];
	output = argv[3];

	for(contador = 1;contador<=3;contador++){
		for (i = 0; i < 2; i++) {
				if ( (pid = fork()) < 0 ) {
					perror(argv[0]);
				} else if (pid == 0) {
					child_process();  // Este intemedio
				} else {
					data[i] = pid;
					printf("\nProceso con Pid: %i creado",data[i]);
				}
			}
			
		sleep(timeout);
			printf("\nEnviando señal a ONE: %i...\n", data[0]);
			kill(data[0], SIGUSR1);
		sleep(timeout);
			printf("\nEnviando señal a TWO: %i...\n", data[1]);
			kill(data[1], SIGUSR2);
		
		wait(NULL); 
	}
  return 0;
}
