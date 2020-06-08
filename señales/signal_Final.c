/*----------------------------------------------------------------

*

* Programación avanzada: Manejo de señales

* Fecha: 17-Abril-2020

*  Jose Carlos Pacheco - A01702828
  Guillermo Carsolio - A01700041

*

--------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/utsname.h>

typedef unsigned long long ull;
typedef unsigned char uchar;
typedef unsigned long ulong;

void handler1(int sig) {
    printf("Ending...\n");
    exit(0);
}

void handler2(int sig) {

  int res;
  res = fork();

  if(res == 0){
    int source;
    char *token;
    char *argvs[100];
    char s[2] = ",";

    if ( (source = open("file1.txt", O_RDONLY)) < 0 ) {
  		perror("./execute");
  		exit(-1);
  	}

    int bytes;
	  uchar *buffer;
	  ulong size = lseek(source, 0, SEEK_END);
	  buffer = (uchar*) malloc(sizeof(uchar) * size);

	  lseek(source, 0, SEEK_SET);
	  read(source, buffer, size);

    token = strtok(buffer, s);
    argvs[0] = token;

    token =  strtok(NULL, s);
    int i = 1;
    while (token != NULL){
      token =  strtok(NULL, " ");
      argvs[i] = token;
      i++;
    }
    argvs[i]  = 0;
    execvp ("/bin/ls", argvs);
    free(buffer);
    close(source);
    exit(0);
  }

}

void handler3(int sig) {

  int res;
  res = fork();
  if(res == 0){

    int source;
    char *token;
    char *argvs[100];
    char s[2] = ",";

    if ( (source = open("file2.txt", O_RDONLY)) < 0 ) {
  		perror("./execute");
  		exit(-1);
  	}

    int bytes;
	  uchar *buffer;
	  ulong size = lseek(source, 0, SEEK_END);
	  buffer = (uchar*) malloc(sizeof(uchar) * size);

	  lseek(source, 0, SEEK_SET);
	  read(source, buffer, size);

    token = strtok(buffer, s);
    argvs[0] = token;

    token =  strtok(NULL, s);
    int i = 1;
    while (token != NULL){
      token =  strtok(NULL, " ");
      argvs[i] = token;
      i++;
    }
    argvs[i]  = 0;
    execvp ("/bin/ps", argvs);
    free(buffer);
    close(source);
    exit(0);
    }
}

void handler4(int sig) {

  int res;
  res = fork();
  if(res == 0){

    int source;
    char *token;
    char *argvs[100];
    char s[2] = ",";

    if ( (source = open("file3.txt", O_RDONLY)) < 0 ) {
      perror("./execute");
      exit(-1);
    }

    int bytes;
    uchar *buffer;
    ulong size = lseek(source, 0, SEEK_END);
    buffer = (uchar*) malloc(sizeof(uchar) * size);

    lseek(source, 0, SEEK_SET);
    read(source, buffer, size);

    uchar *bufferaux  = buffer;

    token = strtok(buffer, s);
    argvs[0] = token;
    token = strtok(NULL, "\n");
    token = strtok(token, " ");
    int i = 1;
    while (token != NULL){
      token =  strtok(NULL, " ");
      argvs[i] = token;
      i++;
    }
    argvs[i] = NULL;
    execvp(argvs[0], argvs);

    free(buffer);
    close(source);
    exit(0);
  }
}


int main(int argc, char *argv[]){
	int file1, file2,file3;
	ull size;
	if (argc!=4){
		fprintf(stderr, "usage: %s file1 file2 file3\n", argv[0]);
		return -2;
	}

	if ((file1=open(argv[1],O_RDONLY))<0){
		fprintf(stderr, "%s: the file %s does not exist\n", argv[0], argv[1]);
		return -3;
	}

	if ((file2=open(argv[2],O_RDONLY))<0){
		fprintf(stderr, "%s: the file %s does not exist\n", argv[0], argv[2]);
		close(file1);
		return -4;
	}

	if ((file3=open(argv[3],O_RDONLY))<0){
		fprintf(stderr, "%s: the file %s does not exist\n", argv[0], argv[3]);
		close(file1);
		close(file2);
		return -5;
	}

	size=lseek(file1,0,SEEK_END);
	if(size==0){
		fprintf(stderr, "%s: the file %s is empty\n", argv[0], argv[1]);
		close(file1);
		close(file2);
		close(file3);
		return -6;
	}

	size=lseek(file2,0, SEEK_END);
	if (size==0){
		fprintf(stderr, "%s: the file %s is empty\n", argv[0], argv[2]);
		close(file1);
		close(file2);
		close(file3);
		return -7;
	}

	size=lseek(file3,0, SEEK_END);
	if (size==0){
		fprintf(stderr, "%s: the file %s is empty\n", argv[0], argv[3]);
		close(file1);
		close(file2);
		close(file3);
		return -8;
	}

    signal(SIGINT, handler1);
    signal(SIGUSR1, handler2);
    signal(SIGUSR2, handler3);
    signal(SIGPWR, handler4);

    printf("Waiting for a signal...\n");

    while(1) {
        pause();
    }
    //exit(0);

	close(file1);
	close(file2);
	close(file3);
	return 0;
}
