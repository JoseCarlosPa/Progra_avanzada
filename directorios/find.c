/*----------------------------------------------------------------

*

* Programación avanzada: Manejo de directorios

* Fecha: 13-Marz-2020

* Autores: Jose Carlos Pacheco Sanchez - A01702828

* Guillermo carsolio gonzales - A01700041

*

*--------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>

void get_info(char *, char *, char *);
void list(char *,char *, int, char *);
int dir_file(char *);

//Esta funcion nos permite imprimier en pantalla el formato deseado.
void get_info(char *name, char *directory, char *program)
    struct stat info;
    char filename[PATH_MAX + NAME_MAX + 1];
    //leemos del filename el direcotio y el nombre para imprimir en pantalla.
    sprintf(filename, "%s/%s", directory, name);
    //con lstat asignamos la informacion correspondiente a cada filename
    lstat(filename, &info);
    printf("Name: %s\n", name);
    printf("Path: %s\n", directory);
    printf("Last access: %s", ctime(&info.st_atime));
    printf("Last modification: %s\n", ctime(&info.st_mtime));

}

//Este es nuestro main
int main(int argc, char* argv[]) {

    char dir_name[PATH_MAX + 1];
    char *directory;
    int recursive;
    char *cadena;
    //Validamos que la cantidad de entradas sea la correcta
    if (argc >= 5 || argc < 3) {
        //Si sale fuera de los parametros retornamos error
        fprintf(stderr, "usage: %s string initial_directory [-r]\n", argv[0]);
        return -2;
    }
    //Obtenemos el directorio alocado en el directorio actual
    getcwd(dir_name, PATH_MAX);
    directory = dir_name;
    //Bandera que solo se activa con -r
    recursive = 0;

    //Si tenemos la cantidad de entradas correcta checamos su valides
    if (argc == 4) {
        //Validamos que si sea -r para prender la bandera recursiva
        if (strcmp(argv[3], "-r") == 0) {
            recursive = 1;
        } else {
            //Si no es correcta retornamos error
            fprintf(stderr, "usage: %s string initial_directory [-r]\n", argv[0]);
            return -4;
        }
    }
    //ahora Obtenemos el nombre del direrctorio que dio el usuario
    directory = argv[2];
    //Y validamos si existe dentro del directorio actual
    if(dir_file(directory) == -1){
        //si marca error [-1] retornamos error
        fprintf(stderr, "%s: No such file or directory\n", argv[0]);
        return -3;
    } else if(dir_file(directory) != 0){
        //Si marca cualquier cosa que no sea exitosos [0] retornamos error
        fprintf(stderr, "%s: Not a directory\n", argv[0]);
        return -3;
    }
    //Aqui recibimos la cadena que va a buscar dentro de los archivos de texto dentro de los directorios
    cadena = argv[1];
    //Lo mandamos a la funcion con el nombre del directorio la bandera de recursion y el filename
    list(cadena, directory, recursive, argv[0]);
    return 0;
}

//aqui validamos si es direcorio
int dir_file(char *name){
    struct stat info;
    if(lstat(name, &info) < 0){
        return -1;
    }
    if(S_ISDIR(info.st_mode)){
        return 0;
    } else {
        return -2;
    }
}
//Esta es una funcion que busca dentro del directorio
int busqueda(char *cadena, char *filename){
    //asignamos funciones ya dadas por LINUX
    char str[NAME_MAX + 1], search[NAME_MAX + 1];
    sprintf(str, "%s%s", filename, " ");
    sprintf(search, "%s%s", cadena, " ");
    int count1 = 0, count2 = 0, i, j, flag;
    //empezamos un ciclo que busca la cadena de caracteres en los directorios
    while (str[count1] != 32)
        count1++;
    while (search[count2] != 32)
        count2++;
    for (i = 0; i <= count1 - count2; i++){
        for (j = i; j < i + count2; j++){
            flag = 1;
            if (str[j] != search[j - i]){
                flag = 0;
                break;
            }
        }
        if (flag == 1) break;
    }
    if (flag == 1)
        return 0;
    else
        return -1;
}

//Esta funcion es la que se enfoca en desplegar y ordena la busqueda recursiva
void list(char *cadena, char *dir_name, int recursive, char *program) {
    char path[PATH_MAX + NAME_MAX + 1];
    DIR *dir;

    struct dirent *direntry;
    struct stat info;
    //Validamos que exista el directrio
    if ( (dir = opendir(dir_name)) == NULL ) {
        //si no retorna error
        fprintf(stderr, "%s: string initial_directory [-r]\n", program);
        exit(-2);
    }
    //Entra al directorio y se vuelve nuestro nuevo directorio local
    dir = opendir(dir_name);
    //esto es la busqueda del direcotio actual
    while( (direntry = readdir(dir)) != NULL) {
        if (strcmp(direntry->d_name, ".") != 0 &&
            strcmp(direntry->d_name, "..") != 0) {
            if(busqueda(cadena, direntry->d_name) == 0) get_info(direntry->d_name, dir_name, program);
        }
    }
    //esto ya es buscar por cada directorio de manera recursiva
    if (recursive) {
        rewinddir(dir);
        while ( (direntry = readdir(dir)) != NULL ) {
            if (strcmp(direntry->d_name, ".") != 0 &&
                strcmp(direntry->d_name, "..") != 0) {

                sprintf(path, "%s/%s", dir_name, direntry->d_name);
                lstat(path, &info);
                if ( (info.st_mode & S_IFMT) == S_IFDIR) {
                    list(cadena, path, recursive, program);
                }
            }
        }
    }
}
