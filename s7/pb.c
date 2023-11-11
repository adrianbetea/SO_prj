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

  if(argc != 2) {
    perror("usage ./program <director_intrare>");
    exit(-1);
  }

  DIR *dir;

  struct dirent *entry;

  struct stat entryStat;

  int fd_input, fd_output;

  // deschidem directorul dat ca parametru
  if((dir=opendir(argv[1])) == NULL) {
    perror("folderul nu s-a putut deschide!");
    exit(-1);
  }
  else {
    printf("folderul %s a fost deschis cu succes!\n", argv[1]);
  }

  // deschidem/cream fisierul de ouptut
  char fout[]="statistica.txt";
  fd_output = open(fout, O_RDWR);
  if(fd_output==-1) {
    printf("fisierul %s nu exista\nvom crea fisierul\n", fout);
    fd_output = creat(fout, S_IRUSR | S_IWUSR | S_IXUSR);
    if(fd_output ==-1) {
      perror("fisierul nu s-a putut crea");
      exit(-1);
    }
    else {
  printf("fisierul %s a fost creat cu succes!\n", fout);
    }
  }
  else {
    printf("fisierul %s s-a deschis cu succes!\n", fout);
  }

   change_dir(argv[1]);

  // parcurgem tot directorul folosind readdir
  while((entry = readdir(dir)) != NULL) {

    if( (lstat(entry->d_name, &entryStat)) == -1) {
        perror("eroare");
        exit(-1);
    }

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
          // scriem in fisierul statistica.txt
          if(write_bmp(fd_input, fd_output, entryStat, entry->d_name) == 1) {
            printf("detaliile fisierului BMP %s au fost scrise in %s!\n\n", entry->d_name, fout);

          }
        } else {
          // functie pt fisier normal
          if(write_file(fd_input, fd_output, entryStat, entry->d_name) == 1) {
            printf("detaliile fisierului %s au fost scrise in %s!\n\n", entry->d_name, fout);
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
        if(write_symbolic_link(fd_output, entryStat, targetStat, entry->d_name) == 1) {
          printf("detaliile directorului %s au fost scrise in %s!\n\n", entry->d_name, fout);
        }

        break;
      }
      case IS_DIR:
      {
       if( strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 ) {
         // verificam daca intrarea este . sau ..
         break;

      }
        // functie pt director
        printf("intrarea %s s-a deschis cu succes!\n", entry->d_name);
        if(write_dir(fd_output, entryStat, entry->d_name) == 1) {
          printf("detaliile directorului %s au fost scrise in %s!\n", entry->d_name, fout);
        }
        break;
      }

    }
  }


  if(closedir(dir) == -1) {
    perror("Directorul nu s-a putut inchide\n");
    exit(-1);
  }
  
  return 0;
}
