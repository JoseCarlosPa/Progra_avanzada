/*----------------------------------------------------------------

*

* Programación avanzada: Manejo de threads

* Fecha: 22 - May - 2020

* Autores:  Jose Carlos Pacheco Sanchez - A01702828
            Guillermo Carsolio -  A01700041

*

*--------------------------------------------------------------*/

#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <time.h>


int step_i = 0,threads,residuo,max;

float **matriz_a;
float **matriz_b;
float **matriz_c;

void* work(void* param) {
    int i, j, k;
    int core = step_i++;

    // Para el work
    int numRows = ceil((double) (max/threads));
    int lastRow = max-(threads-1)*numRows;
    int start = core*numRows;
    int rows = start + numRows;

    if(core == max){
        numRows = lastRow;
    }


    if(core == threads - 1){
      rows = rows + residuo;
    }

    for (i = start; i < rows; i++){
        for (j = 0; j < max; j++){
            for (k = 0; k < max; k++){
                matriz_c[i][j] += matriz_a[i][k] * matriz_b[k][j];
            }
        }
    }
       
	return 0;
               
}


int main(int argc, char* argv[]) {
    void* ret;
    int numth, num_a, num_b, i, j;
    FILE *fa, *fb;
    time_t startTime, endTime;

	//Validacion args
	if (argc == 7) {
		if (strcmp(argv[1], "-a") == 0) {
			if(strcmp(argv[3], "-b") == 0){
				if(strcmp(argv[5], "-t") == 0){
					//Do nothing
				} else {
					printf("%s: te hace falta el -t, found %s\n", argv[0], argv[5]);
					return -5;
				}
			} else {
				printf("%s: te hace falta el -b, found %s\n", argv[0], argv[3]);
				return -4;
			}
		} else {
			printf("%s: te hace falta el -a, found %s\n", argv[0], argv[1]);
			return -3;
		}
	} else {
		printf("usage: %s [-a] [file_matA] [-b] [file_matriz_b] [-t] [num_ber_of_threads]\n", argv[0]);
		return -2;
	}
	fa = fopen(argv[2], "r");
	if(fa == 0){
		printf("%s La Matriz A no es valida, no existe\n", argv[2]);
		return -6;
	}
	fb = fopen(argv[4], "r");
	if(fb == 0){
		printf("%sLa matriz B no es valida, no existe\n", argv[4]);
		return -7;
	}
	numth = atoi(argv[6]);
	if(numth < 1){
		printf("El numero de threads debe ser mayor o igual a 1\n");
		return -8;
	}

	fscanf(fa, "%i", &num_a);
	fscanf(fb, "%i", &num_b);
	if(num_a != num_b){
		printf("Las matrices no son de la misma dimencion\n");
		return -9;
	}

    threads = numth;
    max = num_a;
    residuo = max % threads;

    pthread_t  *pthread_id = (pthread_t *)malloc(threads * sizeof(pthread_t *));

	
	//Allocacion de memoria, creacion de matrizes 
	matriz_a = (float **)malloc(num_a * sizeof(float *));

	for (i=0; i<num_a; i++)
		matriz_a[i] = (float *)malloc(num_a * sizeof(float));
		matriz_b = (float **)malloc(num_b * sizeof(float *));
	for (i=0; i<num_b; i++)
		matriz_b[i] = (float *)malloc(num_b * sizeof(float));
		matriz_c = (float **)malloc(num_b * sizeof(float *));
	for (i=0; i<num_b; i++)
		matriz_c[i] = (float *)malloc(num_b * sizeof(float));

	//Leer el archivo para llenar la matriz
	for(i = 0; i < num_a; i++){
		for(j = 0; j < num_b; j++){
			fscanf(fa, "%f", &matriz_a[i][j]);
		}
	}
	for(i = 0; i < num_a; i++){
		for(j = 0; j < num_b; j++){
			fscanf(fb, "%f", &matriz_b[i][j]);
		}
	}
	//Cerrar archivos
    fclose(fa);
    fclose(fb);

  if(threads > max)
    threads = max ;


  
		printf("%i\n", num_a);
    time(&startTime);
  	for (i = 0; (i < threads) && (i < (max - 1)) ; i++) {
  		pthread_create(&pthread_id[i], NULL, work, (void *)0);
  	}

    // haciendo el join y el wait
  	for (i = 0; (i < threads) && (i < (max - 1)); i++)
  		pthread_join(pthread_id[i], &ret);

		time(&endTime);


    // Haciendo print de los resultados
    for (int i = 0; i < max; i++) {
        for (int j = 0; j < max; j++)
            printf("%0.3f\t", matriz_c[i][j]);
        printf("\n");
    }

	//Liberar memoria
    free(matriz_a);
    free(matriz_b);

	return 0;
}
