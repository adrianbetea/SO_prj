int write_symbolic_link(char* outputFileName, struct stat entryStat, struct stat targetStat, char* nume, char* input_dir, char* output_dir) {
    char out[1024];
    int nr_scrieri = 0;

    // intram in directorul de output
    change_dir("..");
    change_dir(output_dir);
    int fd_output = get_output_file(outputFileName);

    // nume legatura
    sprintf(out, "nume legatura: %s\n", nume);
    write(fd_output, out, strlen(out));
    nr_scrieri++;

    // dimensiunea legaturii
    sprintf(out, "dimensiune legatura: %ld\n", entryStat.st_size);
    write(fd_output, out, strlen(out));
    nr_scrieri++;

    // dimensiunea fisierului target
    sprintf(out, "dimensiune fisier target: %ld\n", targetStat.st_size);
    write(fd_output, out, strlen(out));
    nr_scrieri++;

    // functie pentru scrierea permisiunilor
    write_permissions(fd_output, entryStat);
    nr_scrieri+=3;

    // la final ne intoarem in directorul de input
    change_dir("..");
    change_dir(input_dir);

    // daca returneaza 0 inseamna ca nu s-a scris nimic in fisier
    return nr_scrieri;
}

int symlink_process(struct dirent *entryArray, struct stat entryStat, char *inp_dir, char *out_dir) {
    int nr_scrieri = 0;

    printf("intrarea %s s-a deschis cu succes!\n", entryArray->d_name);
    char buf[1024];
    char *r = realpath(entryArray->d_name, buf);
    struct stat targetStat;
    stat(r, &targetStat);
    // fisierul statistica.txt

    char out_name[50];
    //cream numele fisierului statistica.txt
    strcpy(out_name, entryArray->d_name);
    strcat(out_name, "_");
    strcat(out_name, "statistica.txt");

    if( ( nr_scrieri = write_symbolic_link(out_name, entryStat, targetStat, entryArray->d_name, inp_dir, out_dir)) != 0) {
        printf("detaliile directorului %s au fost scrise in %s!\n\n", entryArray->d_name, out_name);;
    }
    
    return nr_scrieri;
}
