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

void scriere_fisier(int fd_input, int fd_output, struct stat fileStat) {
  char out[1024];

 //folosim file stat pentru a gasi informatii despre fisier
  if(fstat(fd_input, &fileStat) == -1) {
    perror("nu s-a putut citi dimensiunea fisierului!\n");
    exit(-1);
  }

  // DIMENSIUNEA FISIERULUI
  sprintf(out, "size: %ld\n", fileStat.st_size);
  if(write(fd_output, out, strlen(out)) == -1) {
    perror("eroare la scriere");
    exit(-1);
  }

  // USER ID
  sprintf(out, "user_id: %d\n", fileStat.st_uid);
  if(write(fd_output, out, strlen(out)) == -1) {
    perror("eroare la scriere");
    exit(-1);
  }

  // NUMARUL DE LEGATURI
  sprintf(out, "nr legaturi: %ld\n", fileStat.st_nlink);
  if(write(fd_output, out, strlen(out)) == -1) {
    perror("eroare la scriere");
    exit(-1);
  }

  // DATA ULTIMEI MODIFICARI
  sprintf(out, "Last modified time: %s", ctime(&fileStat.st_mtime));
  if(write(fd_output, out, strlen(out)) == -1) {
    perror("eroare la scriere");
    exit(-1);
  }

  // DREPTURI DE ACCES

  // drepturi pentru user
  sprintf(out, "Drepturi de acces user:");
  if(write(fd_output, out, strlen(out)) == -1) {
    perror("eroare la scriere");
    exit(-1);
  }

  sprintf(out, (fileStat.st_mode & S_IRUSR) ? "R" : "-");
  if(write(fd_output, out, strlen(out)) == -1) {
    perror("eroare la scriere");
    exit(-1);
  }

  sprintf(out, (fileStat.st_mode & S_IWUSR) ? "W" : "-");
  if(write(fd_output, out, strlen(out)) == -1) {
    perror("eroare la scriere");
    exit(-1);
  }

  sprintf(out, (fileStat.st_mode & S_IXUSR) ? "X\n" : "-\n");
  if(write(fd_output, out, strlen(out)) == -1) {
    perror("eroare la scriere");
    exit(-1);
  }


  // drepturi pentru grup
  sprintf(out, "Drepturi de acces grup: ");
  if(write(fd_output, out, strlen(out)) == -1) {
    perror("eroare la scriere");
    exit(-1);
  }

  sprintf(out, (fileStat.st_mode & S_IRGRP) ? "R" : "-");
  if(write(fd_output, out, strlen(out)) == -1) {
    perror("eroare la scriere");
    exit(-1);
  }

  sprintf(out, (fileStat.st_mode & S_IWGRP) ? "W" : "-");
  if(write(fd_output, out, strlen(out)) == -1) {
    perror("eroare la scriere");
    exit(-1);
  }

  sprintf(out, (fileStat.st_mode & S_IXGRP) ? "X\n" : "-\n");
  if(write(fd_output, out, strlen(out)) == -1) {
    perror("eroare la scriere");
    exit(-1);
  }


  // drepturi pentru altii
  sprintf(out, "Drepturi de acces altii: ");
  if(write(fd_output, out, strlen(out)) == -1) {
    perror("eroare la scriere");
    exit(-1);
  }

  sprintf(out, (fileStat.st_mode & S_IROTH) ? "R" : "-");
  if(write(fd_output, out, strlen(out)) == -1) {
    perror("eroare la scriere");
    exit(-1);
  }

  sprintf(out, (fileStat.st_mode & S_IWOTH) ? "W" : "-");
  if(write(fd_output, out, strlen(out)) == -1) {
    perror("eroare la scriere");
    exit(-1);
  }

  sprintf(out, (fileStat.st_mode & S_IXOTH) ? "X\n" : "-\n");
  if(write(fd_output, out, strlen(out)) == -1) {
    perror("eroare la scriere");
    exit(-1);
  }

}

int main(int argc, char** argv) {

  if(argc != 2) {
    printf("Eroare: usage ./program <fisier_intrare>");
    exit(-1);
  } else  {
    // verificam extensia fisierului 
    if(strcmp(FileSuffix(argv[1]), ".bmp") !=0) {
	    printf("Eroare: fisierul de intrare nu are extensia buna");
	    exit(-1);
    }
  }

  int fd_input, fd_output;
  // fisierul input este numai pentru citire fd_input = open(argv[1], O_RDONLY); 
  // apel sistem pentru deschdere fisier
  fd_input = open(argv[1], O_RDONLY);

  if(fd_input == -1) {
    // perror se foloseste la apeluri sistem!
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
    //creat - apel sistem pentru crearea unui fisier
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
  // apel sistem pentru citirea din fisier
  if(read(fd_input, unused_buffer, 18) == -1) {
    perror("eroare la citire");
    exit(-1);
  }
  
  
  if(read(fd_input, &width, 4) == -1) {
    perror("eroare la scriere");
    exit(-1);
  }

  sprintf(out, "pentru fisierul %s\n\n", argv[1]);
  if(write(fd_output, out, strlen(out)) == -1) {
    perror("eroare la scriere");
    exit(-1);
  }
  
  sprintf(out, "width: %d\n", width);
  if(write(fd_output, out, strlen(out)) == -1) {
    perror("eroare la scriere");
    exit(-1);
  }
  
  if(read(fd_input, &height, 4) == -1) {
    perror("eroare la citire");
    exit(-1);
  }

  sprintf(out, "height: %d\n", height);
  if(write(fd_output, out, strlen(out)) == -1) {
    perror("eroare la scriere");
    exit(-1);
  }

  struct stat fileStat;

  scriere_fisier(fd_input, fd_output, fileStat);

  if(close(fd_input) == -1) {
    printf("fisierul %s nu s-a putut inchide\n", argv[1]);
    perror("eroare la inchidere fisier");
    exit(-1);
  }
  else {
    printf("fisierul %s s-a inchis cu succes\n", argv[1]);
  }

  
  if(close(fd_output) == -1) {
    printf("fisierul %s nu s-a putut inchide\n", fout);
    perror("eroare la inchidere fisier");
    exit(-1);
  }
  else {    
    printf("fisierul %s s-a inchis cu succes!!!!\n", fout);
  }
  
  return 0;
}
