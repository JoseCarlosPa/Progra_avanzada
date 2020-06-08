

/*----------------------------------------------------------------

*

* Programación avanzada: Manejo de archivos

* Fecha: 2-Sep-2015

* Autores : A01702828 - Jose Carlos pacheco Sanchez
 *          A01700041 - Guillero Carsolio Gonzales

*

*--------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //Importante agregarlo siempre para peter
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//Funcion para cambiar mitad de byte por mitad de byte

//Referencia de funcion:  https://www.geeksforgeeks.org/swap-two-nibbles-byte/
unsigned char swapNibbles(unsigned char x)
{
    return ( (x & 0x0F)<<4 | (x & 0xF0)>>4 );
}

int main( int argc, char* argv[]) {
    int source, destination,numbytes;
    unsigned char buffer;

    if (argc != 3){
        printf("Uso: %s File Name: !Faltan argumento!! \n", argv[0]);
        return -2;
    }

    if((source  = open(argv[1], O_RDONLY)) < 0){
        perror(argv[0]);
        printf("%s No such file or directory\n ", argv[0]);
        return -3;
    }

    if((destination = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666 )) < 0){
        perror(argv[0]);
        printf("%s No such file or directory\n ", argv[0]);
        return -3;
    }

    source = open(argv[1], O_RDONLY);
    destination = open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0666);

    /* Bucle de lectura/escritura */
    while ((numbytes = read(source, &buffer, sizeof(char))) > 0){
        buffer = swapNibbles(buffer); //Swapp de byte por byte para lectura
        write(destination, &buffer, numbytes);
    }

    /* Cierre de archivos */
    close(source);
    close(destination);
    printf("\ndone");
}



