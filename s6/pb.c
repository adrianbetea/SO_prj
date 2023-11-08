#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include <time.h>

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
    perror("usage ./program <fisier_intrare>");
    exit(-1);
  } else  {
    // verificam extensia fisierului 
    if(strcmp(FileSuffix(argv[1]), ".bmp") !=0) {
	perror("fisierul de intrare nu are extensia buna");
	exit(-1);
      }
  }

  int fd_input, fd_output;
  // fisierul input este numai pentru citire  fd_input = open(argv[1], O_RDONLY); 
  fd_input = open(argv[1], O_RDONLY);

  if(fd_input == -1) {
    perror("fisierul nu s-a putut deschide");
    exit(-1);
  }
  else {
    printf("fisierul %s s-a deschis cu succes!\n", argv[1]);
  }

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

  
  char unused_buffer[1024];
  char out[1024];
  int width, height;
  read(fd_input, unused_buffer, 18);
  
  
  read(fd_input, &width, 4);

  sprintf(out, "pentru fisierul %s\n\n", argv[1]);
  write(fd_output, out, strlen(out));
  
  sprintf(out, "width: %d\n", width);
  write(fd_output, out, strlen(out));
  
  read(fd_input, &height, 4);
  sprintf(out, "height: %d\n", height);
  write(fd_output, out, strlen(out));

  struct stat fileStat;

  if(fstat(fd_input, &fileStat) == -1) {
    perror("nu s-a putut citi dimensiunea fisierului!\n");
    exit(-1);
  }

  //folosim file stat pentru a gasi informatii despre fisier
  sprintf(out, "size: %ld\n", fileStat.st_size);
  write(fd_output, out, strlen(out));
  
  sprintf(out, "user_id: %d\n", fileStat.st_uid);
  write(fd_output, out, strlen(out));
  
  sprintf(out, "nr legaturi: %ld\n", fileStat.st_nlink);
  write(fd_output, out, strlen(out));

  // DATA ULTIMEI MODIFICARI

  sprintf(out, "Last modified time: %s", ctime(&fileStat.st_mtime));
  write(fd_output, out, strlen(out));

  // DREPTURI DE ACCES
  
  sprintf(out, "Drepturi de acces user:");
  write(fd_output, out, strlen(out));
  
  sprintf(out, (fileStat.st_mode & S_IRUSR) ? "R" : "-");
  write(fd_output, out, strlen(out));

  sprintf(out, (fileStat.st_mode & S_IWUSR) ? "W" : "-");
  write(fd_output, out, strlen(out));
  
  sprintf(out, (fileStat.st_mode & S_IXUSR) ? "X\n" : "-\n");
  write(fd_output, out, strlen(out));
  
  sprintf(out, "Drepturi de acces grup: ");
  write(fd_output, out, strlen(out));
  
  sprintf(out, (fileStat.st_mode & S_IRGRP) ? "R" : "-");
  write(fd_output, out, strlen(out));
  
  sprintf(out, (fileStat.st_mode & S_IWGRP) ? "W" : "-");
  write(fd_output, out, strlen(out));
  
  sprintf(out, (fileStat.st_mode & S_IXGRP) ? "X\n" : "-\n");
  write(fd_output, out, strlen(out));
  
  sprintf(out, "Drepturi de acces altii: ");
  write(fd_output, out, strlen(out));
  
  
  sprintf(out, (fileStat.st_mode & S_IROTH) ? "R" : "-");
  write(fd_output, out, strlen(out));
  
  sprintf(out, (fileStat.st_mode & S_IWOTH) ? "W" : "-");
  write(fd_output, out, strlen(out));
  
  sprintf(out, (fileStat.st_mode & S_IXOTH) ? "X\n" : "-\n");
  write(fd_output, out, strlen(out));
  

  if(close(fd_input) == -1) {
    printf("fisierul %s nu s-a putut inchide\n", argv[1]);
  }
  else {
    printf("fisierul %s s-a inchis cu succes\n", argv[1]);
  }

  
  if(close(fd_output) == -1) {
    printf("fisierul %s nu s-a putut inchide\n", fout);
  }
  else {    
    printf("fisierul %s s-a inchis cu succes!!!!\n", fout);
  }
  
  return 0;
}
