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
#include <dirent.h>
#include "utils.h"

#define IS_REG_FILE 8
#define IS_SYMB_LINK 10
#define IS_DIR 4

const char *FileSuffix(const char path[]) {
    const char *result;
    int i, n;

    n = strlen(path);
    i = n - 1;
    while ((i > 0) && (path[i] != '.') && (path[i] != '/') && (path[i] != '\\')) {
        i--;
    }
    if ((i > 0) && (i < n - 1) && (path[i] == '.') && (path[i - 1] != '/') && (path[i - 1] != '\\')) {
        result = path + i;
    } else {
        result = path + n;
    }
    return result;
}

int main(int argc, char** argv) {

  if(argc != 3) {
    perror("usage ./program <director_intrare> <director_iesire>");
    exit(-1);
  }

  DIR *input_dir;
  DIR *output_dir;

  struct dirent *entry;

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
    printf("folderul %s a fost deschis cu succes!\n", argv[2]);
  }

change_dir(argv[1]);

  // parcurgem tot directorul folosind readdir
  while((entry = readdir(input_dir)) != NULL) {

    // folosim lstat pentru a verifica si fisierele de tip SYMBOLIC LINK
    if( (lstat(entry->d_name, &entryStat)) == -1) {
        perror("eroare");
        exit(-1);
    }

    // switch case in functie de tipul fisierului
    switch(entry->d_type) {
      case IS_REG_FILE:
      {
        if((fd_input = open(entry->d_name, O_RDONLY)) == -1) {
          perror("intrarea nu s-a putut deschide!");
          exit(-1);
        }
        else {
          printf("intrarea %s s-a deschis cu succes!\n", entry->d_name);
        }

        // daca fisierul este cu extensia .bmp
        if(strcmp(FileSuffix(entry->d_name), ".bmp") == 0) {
          // scriem in fisierul nume_statistica.txt
          char out_name[50];
          strcpy(out_name, entry->d_name);
          strcat(out_name, "_");
          strcat(out_name, "statistica.txt");
          if(write_bmp(fd_input, out_name, entryStat, entry->d_name, argv[1], argv[2]) == 1) {
            printf("detaliile fisierului BMP %s au fost scrise in %s!\n", entry->d_name, out_name);
            if(close(fd_input) == -1) {
                perror("fisierul nu s-a putut inchide!");
            } else {
              printf("fisierul %s s-a inchis cu succes!\n\n", entry->d_name);
            }
          }
        } else {
          // functie pt fisier normal
          char out_name[50];
          strcpy(out_name, entry->d_name);
          strcat(out_name, "_");
          strcat(out_name, "statistica.txt");
          if(write_file(fd_input, out_name, entryStat, entry->d_name, argv[1], argv[2]) == 1) {
            
            printf("detaliile fisierului %s au fost scrise in %s!\n", entry->d_name, out_name);
            if(close(fd_input) == -1) {
                perror("fisierul nu s-a putut inchide!");
            }else {
              printf("fisierul %s s-a inchis cu succes!\n\n", entry->d_name);
            }
          }
        }
          break;
      }
      case IS_SYMB_LINK:
      {
        printf("intrarea %s s-a deschis cu succes!\n", entry->d_name);
        char buf[1024];
        char *r = realpath(entry->d_name, buf);
        struct stat targetStat;
        stat(r, &targetStat);
        // fisierul statistica.txt
        
        char out_name[50];
        strcpy(out_name, entry->d_name);
        strcat(out_name, "_");
        strcat(out_name, "statistica.txt");

        if(write_symbolic_link(out_name, entryStat, targetStat, entry->d_name, argv[1], argv[2]) == 1) {
          printf("detaliile directorului %s au fost scrise in %s!\n\n", entry->d_name, out_name);
        }

        break;
      }
      case IS_DIR:
      {
       if( strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 ) {
         // verificam daca intrarea este . sau ..
         break;

      }
        char out_name[50];
        strcpy(out_name, entry->d_name);
        strcat(out_name, "_");
        strcat(out_name, "statistica.txt"); 
        // functie pt director
        printf("intrarea %s s-a deschis cu succes!\n", entry->d_name);
        if(write_dir(out_name, entryStat, entry->d_name, argv[1], argv[2]) == 1) {
          printf("detaliile directorului %s au fost scrise in %s!\n", entry->d_name, out_name);
        }
        break;
      }

      default:
      {
        printf("acest tip de fisier nu e recunoscut\n");
        break;
      }

    }
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
