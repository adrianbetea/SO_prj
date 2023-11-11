
int write_bmp(int fd_input, int fd_output, struct stat fileStat, char* nume) {
    char unused_buffer[1024];
    char out[1024];

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

    return 1;
}

int write_file(int fd_input, int fd_output, struct stat fileStat, char* nume) {
    char out[1024];

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

    return 1;
}

int write_symbolic_link(int fd_output, struct stat entryStat, struct stat targetStat, char* nume) {
    char out[1024];

    // nume legatura
    sprintf(out, "nume legatura: %s\n", nume);
    write(fd_output, out, strlen(out));

    // dimensiunea legaturii
    sprintf(out, "dimensiune legatura: %ld\n", entryStat.st_size);
    write(fd_output, out, strlen(out));

    // dimensiunea fisierului target
    sprintf(out, "dimensiune fisier target: %ld\n", targetStat.st_size);
    write(fd_output, out, strlen(out));

    // DREPTURI DE ACCES

    // drepturi pentru user
    sprintf(out, "Drepturi de acces user:");
    write(fd_output, out, strlen(out));

    sprintf(out, (entryStat.st_mode & S_IRUSR) ? "R" : "-");
    write(fd_output, out, strlen(out));

    sprintf(out, (entryStat.st_mode & S_IWUSR) ? "W" : "-");
    write(fd_output, out, strlen(out));

    sprintf(out, (entryStat.st_mode & S_IXUSR) ? "X\n" : "-\n");
    write(fd_output, out, strlen(out));


    // drepturi pentru grup
    sprintf(out, "Drepturi de acces grup: ");
    write(fd_output, out, strlen(out));

    sprintf(out, (entryStat.st_mode & S_IRGRP) ? "R" : "-");
    write(fd_output, out, strlen(out));

    sprintf(out, (entryStat.st_mode & S_IWGRP) ? "W" : "-");
    write(fd_output, out, strlen(out));

    sprintf(out, (entryStat.st_mode & S_IXGRP) ? "X\n" : "-\n");
    write(fd_output, out, strlen(out));


    // drepturi pentru altii
    sprintf(out, "Drepturi de acces altii: ");
    write(fd_output, out, strlen(out));

    sprintf(out, (entryStat.st_mode & S_IROTH) ? "R" : "-");
    write(fd_output, out, strlen(out));

    sprintf(out, (entryStat.st_mode & S_IWOTH) ? "W" : "-");
    write(fd_output, out, strlen(out));

    sprintf(out, (entryStat.st_mode & S_IXOTH) ? "X\n\n" : "-\n\n");
    write(fd_output, out, strlen(out));

    return 1;

}

int write_dir(int fd_output, struct stat dirStat, char *nume) {
    char out[1024];

    sprintf(out, "nume director: %s\n", nume);
    write(fd_output, out, strlen(out));


    // USER ID
    sprintf(out, "user_id: %d\n", dirStat.st_uid);
    write(fd_output, out, strlen(out));

    // DREPTURI DE ACCES

    // drepturi pentru user
    sprintf(out, "Drepturi de acces user: ");
    write(fd_output, out, strlen(out));

    sprintf(out, (dirStat.st_mode & S_IRUSR) ? "R" : "-");
    write(fd_output, out, strlen(out));

    sprintf(out, (dirStat.st_mode & S_IWUSR) ? "W" : "-");
    write(fd_output, out, strlen(out));

    sprintf(out, (dirStat.st_mode & S_IXUSR) ? "X\n" : "-\n");
    write(fd_output, out, strlen(out));


    // drepturi pentru grup
    sprintf(out, "Drepturi de acces grup: ");
    write(fd_output, out, strlen(out));

    sprintf(out, (dirStat.st_mode & S_IRGRP) ? "R" : "-");
    write(fd_output, out, strlen(out));

    sprintf(out, (dirStat.st_mode & S_IWGRP) ? "W" : "-");
    write(fd_output, out, strlen(out));

    sprintf(out, (dirStat.st_mode & S_IXGRP) ? "X\n" : "-\n");
    write(fd_output, out, strlen(out));


    // drepturi pentru altii
    sprintf(out, "Drepturi de acces altii: ");
    write(fd_output, out, strlen(out));

    sprintf(out, (dirStat.st_mode & S_IROTH) ? "R" : "-");
    write(fd_output, out, strlen(out));

    sprintf(out, (dirStat.st_mode & S_IWOTH) ? "W" : "-");
    write(fd_output, out, strlen(out));

    sprintf(out, (dirStat.st_mode & S_IXOTH) ? "X\n\n" : "-\n\n");
    write(fd_output, out, strlen(out));


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
