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

void write_permissions(int fd_output, struct stat fileStat) {
    char out[1024];
    // DREPTURI DE ACCES

    // drepturi pentru user
    sprintf(out, "Drepturi de acces user:");
    write(fd_output, out, strlen(out));

    sprintf(out, (fileStat.st_mode & S_IRUSR) ? "R" : "-");
    write(fd_output, out, strlen(out));

    sprintf(out, (fileStat.st_mode & S_IWUSR) ? "W" : "-");
    write(fd_output, out, strlen(out));

    sprintf(out, (fileStat.st_mode & S_IXUSR) ? "X\n" : "-\n");
    write(fd_output, out, strlen(out));


    // drepturi pentru grup
    sprintf(out, "Drepturi de acces grup: ");
    write(fd_output, out, strlen(out));

    sprintf(out, (fileStat.st_mode & S_IRGRP) ? "R" : "-");
    write(fd_output, out, strlen(out));

    sprintf(out, (fileStat.st_mode & S_IWGRP) ? "W" : "-");
    write(fd_output, out, strlen(out));

    sprintf(out, (fileStat.st_mode & S_IXGRP) ? "X\n" : "-\n");
    write(fd_output, out, strlen(out));


    // drepturi pentru altii
    sprintf(out, "Drepturi de acces altii: ");
    write(fd_output, out, strlen(out));

    sprintf(out, (fileStat.st_mode & S_IROTH) ? "R" : "-");
    write(fd_output, out, strlen(out));

    sprintf(out, (fileStat.st_mode & S_IWOTH) ? "W" : "-");
    write(fd_output, out, strlen(out));

    sprintf(out, (fileStat.st_mode & S_IXOTH) ? "X\n\n" : "-\n\n");
    write(fd_output, out, strlen(out));

}


int write_bmp(int fd_input, char* outputFileName, struct stat fileStat, char* nume, char* input_dir, char* output_dir) {
    char unused_buffer[1024];
    char out[1024];

    // intram in directorul de output
    change_dir("..");
    change_dir(output_dir);
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
            printf("fisierul %s a fost creat cu succes!\n", outputFileName);
        }
    }
    else {
        printf("fisierul %s s-a deschis cu succes!\n", outputFileName);
    }

    int width, height;
    read(fd_input, unused_buffer, 18);

    sprintf(out, "nume fisier: %s\n", nume);
    write(fd_output, out, strlen(out));

    read(fd_input, &width, 4);
    sprintf(out, "width: %d\n", width);
    write(fd_output, out, strlen(out));

    read(fd_input, &height, 4);
    sprintf(out, "height: %d\n", height);
    write(fd_output, out, strlen(out));

    //folosim file stat pentru a gasi informatii despre fisier
    if(fstat(fd_input, &fileStat) == -1) {
        perror("nu s-a putut citi dimensiunea fisierului!\n");
        exit(-1);
    }

    // DIMENSIUNEA FISIERULUI
    sprintf(out, "size: %ld\n", fileStat.st_size);
    write(fd_output, out, strlen(out));

    // USER ID
    sprintf(out, "user_id: %d\n", fileStat.st_uid);
    write(fd_output, out, strlen(out));

    // NUMARUL DE LEGATURI
    sprintf(out, "nr legaturi: %ld\n", fileStat.st_nlink);
    write(fd_output, out, strlen(out));

    // DATA ULTIMEI MODIFICARI
    sprintf(out, "Last modified time: %s", ctime(&fileStat.st_mtime));
    write(fd_output, out, strlen(out));


    write_permissions(fd_output, fileStat);

    // la final ne intoarem in directorul de input
    change_dir("..");
    change_dir(input_dir);

    return 1;
}

int write_file(int fd_input, char* outputFileName, struct stat fileStat, char* nume, char* input_dir, char* output_dir) {
    char out[1024];

    // intram in directorul de output
    change_dir("..");
    change_dir(output_dir);
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
            printf("fisierul %s a fost creat cu succes!\n", outputFileName);
        }
    }
    else {
        printf("fisierul %s s-a deschis cu succes!\n", outputFileName);
    }

    sprintf(out, "nume fisier: %s\n", nume);
    write(fd_output, out, strlen(out));

    //folosim file stat pentru a gasi informatii despre fisier
    if(fstat(fd_input, &fileStat) == -1) {
        perror("nu s-a putut citi dimensiunea fisierului!\n");
        exit(-1);
    }

    // DIMENSIUNEA FISIERULUI
    sprintf(out, "size: %ld\n", fileStat.st_size);
    write(fd_output, out, strlen(out));

    // USER ID
    sprintf(out, "user_id: %d\n", fileStat.st_uid);
    write(fd_output, out, strlen(out));

    // NUMARUL DE LEGATURI
    sprintf(out, "nr legaturi: %ld\n", fileStat.st_nlink);
    write(fd_output, out, strlen(out));

    // DATA ULTIMEI MODIFICARI
    sprintf(out, "Last modified time: %s", ctime(&fileStat.st_mtime));
    write(fd_output, out, strlen(out));

    write_permissions(fd_output, fileStat);

    // la final ne intoarem in directorul de input
    change_dir("..");
    change_dir(input_dir);

    return 1;
}

int write_symbolic_link(char* outputFileName, struct stat entryStat, struct stat targetStat, char* nume, char* input_dir, char* output_dir) {
    char out[1024];

    // intram in directorul de output
    change_dir("..");
    change_dir(output_dir);
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
            printf("fisierul %s a fost creat cu succes!\n", outputFileName);
        }
    }
    else {
        printf("fisierul %s s-a deschis cu succes!\n", outputFileName);
    }

    // nume legatura
    sprintf(out, "nume legatura: %s\n", nume);
    write(fd_output, out, strlen(out));

    // dimensiunea legaturii
    sprintf(out, "dimensiune legatura: %ld\n", entryStat.st_size);
    write(fd_output, out, strlen(out));

    // dimensiunea fisierului target
    sprintf(out, "dimensiune fisier target: %ld\n", targetStat.st_size);
    write(fd_output, out, strlen(out));

    write_permissions(fd_output, entryStat);

    // la final ne intoarem in directorul de input
    change_dir("..");
    change_dir(input_dir);

    return 1;
}

int write_dir(char* outputFileName, struct stat dirStat, char* nume, char* input_dir, char* output_dir) {
    char out[1024];

    // intram in directorul de output
    change_dir("..");
    change_dir(output_dir);
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
            printf("fisierul %s a fost creat cu succes!\n", outputFileName);
        }
    }
    else {
        printf("fisierul %s s-a deschis cu succes!\n", outputFileName);
    }

    sprintf(out, "nume director: %s\n", nume);
    write(fd_output, out, strlen(out));


    // USER ID
    sprintf(out, "user_id: %d\n", dirStat.st_uid);
    write(fd_output, out, strlen(out));

    write_permissions(fd_output, dirStat);


    // la final ne intoarem in directorul de input
    change_dir("..");
    change_dir(input_dir);

    return 1;
}
