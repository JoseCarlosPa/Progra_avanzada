/*----------------------------------------------------------------

*

* Programación avanzada: Examen 1

* Fecha: 10-Marzo-2020

* Autor: A01702828 Jose Carlos Pacheco Sanchez 

*

*--------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <limits.h>

long int dirSize = 0;

void get_info(char *name, char *dir_base, char *program);

void list(char *dir_base, int recursive, char *program);

void checkDir(char *dir_name, char *program);

int main(int argc, char *argv[]) {
    char dir_name[PATH_MAX + 1];
    char *dir_base;
    int recursive = 1;

    if (argc > 2) {
        fprintf(stderr, "usage: %s [directory]\n", argv[0]);
        return -1;
    }

    if (argc == 1) {
        getcwd(dir_name, PATH_MAX);
        dir_base = dir_name;
    } else {
        dir_base = argv[1];
        if (strcmp(dir_base, "dir1") == 0) {
            dir_base = ".";
        }
    }

    checkDir(dir_base, argv[0]);

    list(dir_base, recursive, argv[0]);
    return 0;
}

void checkDir(char *dir_name, char *program) {
    DIR *dir;
    struct stat info;

    if ((dir = opendir(dir_name)) == NULL) {
        printf("%s: No such file or directory\n", program);
        exit(-2);
    }

    lstat(dir_name, &info);
    if ((info.st_mode & S_IFMT) != S_IFDIR) {
        printf("%s: Not a directory\n", program);
        exit(-2);
    }
}

void list(char *dir_base, int recursive, char *program) {
    char path[PATH_MAX + NAME_MAX + 1];
    DIR *dir;
    struct dirent *direntry;
    struct stat info;

    if ((dir = opendir(dir_base)) == NULL) {
        perror(program);
        exit(-2);
    }


    if (recursive) {
        rewinddir(dir);
        while ((direntry = readdir(dir)) != NULL) {
            if (strcmp(direntry->d_name, ".") != 0 &&
                strcmp(direntry->d_name, "..") != 0) {

                sprintf(path, "%s/%s", dir_base, direntry->d_name);
                lstat(path, &info);
                if ((info.st_mode & S_IFMT) == S_IFDIR) {
                    list(path, recursive, program);
                    get_info(direntry->d_name, dir_base, program);
                }
            }
        }
    }
    printf("\n");
}

void get_info(char *name, char *dir_base, char *program) {
    int i;
    struct stat info;
    struct stat info_file;
    char filename[PATH_MAX + NAME_MAX + 1];
    char checkFilename[PATH_MAX + NAME_MAX + 1];

    int file_count = 0;

    DIR *dir;
    struct dirent *direntry;
    struct dirent *max_direntry;
    long int maxNumber = 0;
    char maxfilename[NAME_MAX + 1];
    struct dirent *min_direntry;
    long int minNumber = 2147483647;
    char minfilename[NAME_MAX + 1];

    sprintf(filename, "%s/%s", dir_base, name);

    lstat(filename, &info);

    if ((info.st_mode & S_IFMT) == S_IFDIR) {

        printf("directory: %s\n", filename);
        dir = opendir(filename);
        rewinddir(dir);

        while ((direntry = readdir(dir)) != NULL) {
            FILE *archivo;
            int v1;
            int max = 0, min = 0;

            if (direntry->d_type == DT_REG) { /* If the entry is a regular file */

                archivo=fopen(filename,"rb");
                if (archivo==NULL)exit(1);

                while(feof(archivo)){
                    fread(&v1, sizeof(int), 1, archivo);

                    printf("entero: %i\n",v1);
                    fclose(archivo);
                }

                sprintf(checkFilename, "%s/%s", filename, direntry->d_name);
                lstat(checkFilename, &info_file);
                maxNumber = info_file.st_size;
                sprintf(maxfilename, "%s", direntry->d_name);
                minNumber = info_file.st_size;
                sprintf(minfilename, "%s", direntry->d_name);
                printf("%s/%s - min =  %li - max = %li\n", filename, minfilename, minNumber, maxNumber);
            }
        }
    }
}