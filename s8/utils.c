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

void change_dir(const char* dir) {
 // current working directory
   char cwd[100];
   // shimbam cwd in cel dat ca argument
   chdir(dir);

   // afisez din nou cwe pentru a verifica unde ma aflu
   if (getcwd(cwd, sizeof(cwd)) == NULL) {
       perror("getcwd() error");
       exit(-1);
   } 
}

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

int get_output_file(char* outputFileName) {

    int fd_output;

    fd_output = open(outputFileName, O_RDWR);
    if(fd_output==-1) {
        printf("fisierul %s nu exista\nvom crea fisierul\n", outputFileName);
        fd_output = creat(outputFileName, S_IRUSR | S_IWUSR | S_IXUSR);
        if(fd_output ==-1) {
            perror("fisierul nu s-a putut crea");
            return 0;
        }
        else {
            printf("fisierul %s a fost creat cu succes!\n\n", outputFileName);
        }
    }
    else {
        printf("fisierul %s s-a deschis cu succes!\n\n", outputFileName);
    }
    return fd_output;
}
