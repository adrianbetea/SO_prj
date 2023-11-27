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
    int pid_second;
    int j = 0;
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

    change_dir(argv[1]);

    if(strlen(argv[3]) > 1) {
        perror("al treilea argument nu este caracter, este string!");
        exit(-1);
    }
    // salvam caracterul dat ca argument intr-o variabila
    char caracter_alfan = argv[3][0];

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
    //int pfd[2];
    // parcurgem toate intrarile
    // cream un proces nou pentru fiecare intrare
    for(i = 0; i < n; i++) {
        //cream pipe-ul cu ajutorul caruia facem legatura dintre procesul curent si procesul de propozitii corecte
        //if(pipe(pfd)<0) {
          //  perror("Eroare la crearea pipe-ului\n");
           // exit(1);
        //}

        if((pids[i] = fork()) < 0) {
            perror("Eroare la crearea procesului");
            exit(-1);
        }
        if(pids[i] == 0) {
            // inchide capatul de citire
            //close(pfd[0]);

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
                        char buffer[4096];
                        get_content(fd_input, entryStat, argv[1], argv[2]);
                        printf("%s\n", buffer);
                        //scriem in pipe continutul fisierului
                       // write(pfd[1], buffer, 4096);

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
            //inchidem capatul utilizat
            //close(pfd[1]);
            exit(nr_scrieri);    
        }

        // ne folosim de exit status pentru a afla numarul de scrieri din fisierul statistica.txt
        int status;
        if ( waitpid(pids[i], &status, 0) == -1 ) {
            perror("waitpid failed");
            return EXIT_FAILURE;
        }
        if ( WIFEXITED(status) ) {
            const int exit_status = WEXITSTATUS(status);
            nr_scrieri_array[i] = exit_status;
        }
    
         // mesaj incheiere proces
        printf("!!!S-a incheiat procesul cu PID-ul %d si codul %d!!!\n\n", pids[i], nr_scrieri_array[i]);

        
        
        if(entryArray[j]->d_type == IS_REG_FILE) {
            // daca e fisier normal fara extensia .bmp facem un al doilea proces in procesul parinte
            if(!(strcmp(FileSuffix(entryArray[i]->d_name), ".bmp") == 0)) {
                //if((pid_second = vfork()) < 0) {
                   // perror("Eroare la crearea procesului");
                   // exit(-1);
                //}
                //if(pid_second == 0) {

                    
                    //execlp("ls","ls","-l", NULL);
               // }

               get_content()

                //close(pfd[1]);
                //char buffer[4096];
                //read(pfd[0], buffer, 4096);
                
                //printf("%s\n", buffer);
                
                //close(pfd[0]);

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