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

    if(argc != 4) {
        perror("usage ./program <director_intrare> <director_iesire>");
        exit(-1);
    }

    int pids[MAX_P]; // maxim 1000 de procese
    int pid_second; // pentru procesul de verificare propozitii corecte.
    int wpid;
    int status = 0; // exit(0) 
    
    
    DIR *input_dir;
    DIR *output_dir;

    struct dirent *entry;
    struct dirent *entryArray[MAX_DIR];

    struct stat entryStat;

    int fd_input;

    // deschidem directorul dat ca parametru
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

    if(strlen(argv[3]) > 1) {
        perror("al treilea argument nu este caracter, este string!");
        exit(-1);
    }
    // salvam caracterul dat ca argument intr-o variabila
    char caracter_alfan = argv[3][0];

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

    int pfd1[2]; // folosit pentru a salva capetele pipe-ului dintre procesele copii
    int pfd2[2]; // folosit pentru a salva capetele pipe-ului dintre procesul copil2 si parinte
    // parcurgem toate intrarile
    // cream un proces nou pentru fiecare intrare
    for(i = 0; i < n; i++) {
        //cream pipe-ul cu ajutorul caruia facem legatura dintre procesul curent si procesul de propozitii corecte
        if(pipe(pfd1)<0) {
            perror("Eroare la crearea pipe-ului\n");
            exit(1);
        }


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

                        close(pfd1[0]);// close reading end of the pipe

                        // functie pt fisier normal
                        nr_scrieri = file_process(fd_input, entryArray[i], entryStat, argv[1], argv[2]);

                        if((fd_input = open(entryArray[i]->d_name, O_RDONLY)) == -1) {
                            perror("intrarea nu s-a putut deschide!");
                            exit(-1);
                        }
                        else {
                            printf("intrarea %s s-a deschis cu succes!\n", entryArray[i]->d_name);
                        }
                        // get regular file content
                        char buffer[1024];
                        //salvam continutul fisierului in buffer
                        get_content(fd_input, entryStat, argv[1], argv[2], buffer);
                        //scrie bufferul in pipe si dupa inchidem scrierea
                        write(pfd1[1], buffer, strlen(buffer)+1);
                        dup2(pfd1[1], 1); // redirecteaza iesirea standard spre pipe
                        close(pfd1[1]);
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

        int status;
        if ( waitpid(pids[i], &status, 0) == -1 ) {
            perror("waitpid failed");
            return EXIT_FAILURE;
        }
        if ( WIFEXITED(status) ) {
            const int exit_status = WEXITSTATUS(status);
            nr_scrieri_array[i] = exit_status;
        }

        // ne folosim de exit status pentru a afla numarul de scrieri din fisierul statistica.txt
        
        // mesaj incheiere proces
        printf("!!!S-a incheiat procesul cu PID-ul %d si codul %d!!!\n\n", pids[i], nr_scrieri_array[i]);

        //daca e fisier normal si nu are extensia .bmp
        if(entryArray[i]->d_type == IS_REG_FILE) {
            if(!(strcmp(FileSuffix(entryArray[i]->d_name), ".bmp") == 0)) {
                if((pid_second = fork()) < 0) {
                perror("Eroare la crearea procesului");
                exit(-1);
                }

                if(pid_second == 0) {

                    close(pfd1[1]); // inchide scrirea din pipe
                    char file_content[1024];
                    
                    // salveaza in file_content ce avem in pipe
                    
                    
                    //avem in file_content continutul fisierului normal
                    char shell_path[100]="/home/adrian/Desktop/SO/Proiect_SO/s9/bash.sh";
                    strcat(shell_path, " ");
                    strcat(shell_path, argv[3]);

                    //dup2(pfd1[0], 0); // redirecteaza intrarea standard spre pipe
                    FILE *stream = fdopen(pfd1[0], "r");

                    fscanf(stream, "%s", file_content);

                    execlp("/home/adrian/Desktop/SO/Proiect_SO/s9/bash.sh", "/home/adrian/Desktop/SO/Proiect_SO/s9/bash.sh", argv[3], NULL);

                    close(pfd1[0]);

                    //cream pipe-ul cu ajutorul caruia facem legatura dintre procesul curent si procesul parinte
                    if(pipe(pfd2)<0) {
                        perror("Eroare la crearea pipe-ului\n");
                        exit(1);
                    }

                    //close(pfd2[0]);// close reading end of the pipe

                    //scrie in pipe 
                    //write(pfd2[1], file_content, strlen(file_content)+1);
                    //close(pfd2[1]); // close writing end of the pipe
                    
                    exit(0);
                }
            } 
        }

    }
    while( (wpid = wait(&status)) > 0); // asteapta toate procesele sa se termine

    
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