#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include "utils.h"
#include "bmp.h"
#include "file.h"
#include "symlink.h"
#include "dir.h"


#define IS_REG_FILE 8
#define IS_SYMB_LINK 10
#define IS_DIR 4
#define MAX_P 1000
#define MAX_DIR 1000


int nr_scrieri_array[MAX_P];

int main(int argc, char** argv) {

    if(argc != 3) {
        printf("usage ./program <director_intrare> <director_iesire>\n");
        exit(-1);
    }

    int pids[MAX_P]; // maxim 1000 de procese
    
    

    DIR *input_dir;
    DIR *output_dir;

    struct dirent *entry;
    struct dirent *entryArray[MAX_DIR];

    struct stat entryStat;

    int fd_input;

    // deschidem directorul dat ca parametru
    // verificam apelul sistem opendir
    if((input_dir=opendir(argv[1])) == NULL) {
        perror("folderul input nu s-a putut deschide!");
        exit(-1);
    }
    else {
        printf("folderul %s a fost deschis cu succes!\n", argv[1]);
    }

    if((output_dir=opendir(argv[2])) == NULL) {
        perror("folderul input nu s-a putut deschide!");
        exit(-1);
    }
    else {
        printf("folderul %s a fost deschis cu succes!\n\n", argv[2]);
    }

    change_dir(argv[1]);


    int i = 0;
    // parcurgem tot directorul folosind readdir
    while((entry = readdir(input_dir)) != NULL) {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
           // nu se intampla nimic la . si ..
           continue;
        }
        entryArray[i++]=entry;
    }

    // avem toate intrarile in entryArray
    int n = i;

    // parcurgem toate intrarile
    // cream un proces nou pentru fiecare intrare
    for(i = 0; i < n; i++) {
        if((pids[i] = fork()) < 0) {
            perror("Eroare la crearea procesului");
            exit(-1);
        }
        if(pids[i] == 0) {
            // folosim lstat pentru a verifica si fisierele de tip SYMBOLIC LINK
            if( (lstat(entryArray[i]->d_name, &entryStat)) == -1) {
                perror("eroare");
                exit(-1);
            }


            int nr_scrieri = 0;
            // switch case in functie de tipul fisierului
            switch(entryArray[i]->d_type) {
                
                case IS_REG_FILE:   
                {
                    // deschidem fisierul folosind O_RDWR pentru a suprascrie in .bmp
                    if((fd_input = open(entryArray[i]->d_name, O_RDONLY)) == -1) {
                        perror("intrarea nu s-a putut deschide!");
                        exit(-1);
                    }
                    else {
                        printf("intrarea %s s-a deschis cu succes!\n", entryArray[i]->d_name);
                    }

                    // daca fisierul este cu extensia .bmp
                    if(strcmp(FileSuffix(entryArray[i]->d_name), ".bmp") == 0) {

                        nr_scrieri = bmp_process(fd_input, entryArray[i], entryStat, argv[1], argv[2]);

                        
                    } else {

                        // functie pt fisier normal
                        nr_scrieri = file_process(fd_input, entryArray[i], entryStat, argv[1], argv[2]);
                    }
                    break;
                }
                case IS_SYMB_LINK:
                {
                    
                    // functie pt symbolic link
                    nr_scrieri = symlink_process(entryArray[i], entryStat, argv[1], argv[2]);

                    break;
                }
                case IS_DIR:
                {
                    // functie pt director
                    nr_scrieri = dir_process(entryArray[i], entryStat, argv[1], argv[2]);

                    break;
                }

                default:
                {
                    printf("acest tip de fisier nu e recunoscut\n");
                    break;
                }

            }

            printf("proces %d: Numar scrieri fisier %s = %d\n\n", getpid(), entryArray[i]->d_name, nr_scrieri);
            
            exit(nr_scrieri);    
        }

        

        // ne folosim de exit status pentru a afla numarul de scrieri din fisierul statistica.txt
        
        
    }
    
    for(int j = 0; j < n; j++) {
        int status;
        if ( waitpid(pids[j], &status, 0) == -1 ) {
            perror("waitpid failed");
            return EXIT_FAILURE;
        }
        if ( WIFEXITED(status) ) {
            const int exit_status = WEXITSTATUS(status);
            nr_scrieri_array[j] = exit_status;
        }

        
    }

    
    for(i = 0; i < n; i++) {
        printf("proces %d: Numar scrieri fisier %s = %d\n", pids[i], entryArray[i]->d_name, nr_scrieri_array[i]);
    }

    if(closedir(input_dir) == -1) {
        perror("Directorul nu s-a putut inchide\n");
        exit(-1);
    }
    if(closedir(output_dir) == -1) {
        perror("Directorul nu s-a putut inchide\n");
        exit(-1);
    }

    return 0;
}