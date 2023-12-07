void write_permissions(int fd_output, struct stat fileStat) {
    char out[1024];
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

    sprintf(out, (fileStat.st_mode & S_IXOTH) ? "X\n\n" : "-\n\n");
    if(write(fd_output, out, strlen(out)) == -1) {
        perror("eroare la scriere");
        exit(-1);
    }

}

int write_bmp(int fd_input, int fd_output, struct stat fileStat, char* nume) {
    char unused_buffer[1024];
    char out[1024];

    int width, height;
    if(read(fd_input, unused_buffer, 18) == -1) {
        perror("eroare la citire");
        exit(-1);
    }

    sprintf(out, "nume fisier: %s\n", nume);
    if(write(fd_output, out, strlen(out)) == -1) {
        perror("eroare la scriere");
        exit(-1);
    }

    if(read(fd_input, &width, 4) == -1) {
        perror("eroare la citire");
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


    write_permissions(fd_output, fileStat);

    return 1;
}

int write_file(int fd_input, int fd_output, struct stat fileStat, char* nume) {
    char out[1024];

    sprintf(out, "nume fisier: %s\n", nume);
    if(write(fd_output, out, strlen(out)) == -1) {
        perror("eroare la scriere");
        exit(-1);
    }

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

    write_permissions(fd_output, fileStat);

    return 1;
}

int write_symbolic_link(int fd_output, struct stat entryStat, struct stat targetStat, char* nume) {
    char out[1024];

    // nume legatura
    sprintf(out, "nume legatura: %s\n", nume);
    if(write(fd_output, out, strlen(out)) == -1) {
        perror("eroare la scriere");
        exit(-1);
    }

    // dimensiunea legaturii
    sprintf(out, "dimensiune legatura: %ld\n", entryStat.st_size);
    if(write(fd_output, out, strlen(out)) == -1) {
        perror("eroare la scriere");
        exit(-1);
    }

    // dimensiunea fisierului target
    sprintf(out, "dimensiune fisier target: %ld\n", targetStat.st_size);
    if(write(fd_output, out, strlen(out)) == -1) {
        perror("eroare la scriere");
        exit(-1);
    }

    write_permissions(fd_output, entryStat);

    return 1;

}

int write_dir(int fd_output, struct stat dirStat, char *nume) {
    char out[1024];

    sprintf(out, "nume director: %s\n", nume);
    if(write(fd_output, out, strlen(out)) == -1) {
        perror("eroare la scriere");
        exit(-1);
    }


    // USER ID
    sprintf(out, "user_id: %d\n", dirStat.st_uid);
    if(write(fd_output, out, strlen(out)) == -1) {
        perror("eroare la scriere");
        exit(-1);
    }

    write_permissions(fd_output, dirStat);


    return 1;
}

void change_dir(const char* dir) {
 // current working directory
   char cwd[PATH_MAX];
   // shimbam cwd in cel dat ca argument
   chdir(dir);

   // afisez din nou cwe pentru a verifica unde ma aflu
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("Current working dir: %s\n\n", cwd);
   } else {
       perror("getcwd() error");
       exit(-1);
   }
}
