int write_dir(char* outputFileName, struct stat dirStat, char* nume, char* input_dir, char* output_dir) {
    char out[1024];
    int nr_scrieri = 0;

    // intram in directorul de output
    change_dir("..");
    change_dir(output_dir);
    int fd_output = get_output_file(outputFileName);

    sprintf(out, "nume director: %s\n", nume);
    write(fd_output, out, strlen(out));
    nr_scrieri++;


    // USER ID
    sprintf(out, "user_id: %d\n", dirStat.st_uid);
    write(fd_output, out, strlen(out));
    nr_scrieri++;

    // functie pentru scrierea permisiunilor
    write_permissions(fd_output, dirStat); 
    nr_scrieri+=3;


    // la final ne intoarem in directorul de input
    change_dir("..");
    change_dir(input_dir);

    // daca returneaza 0 inseamna ca nu s-a scris nimic in fisier
    return nr_scrieri;
}



int dir_process(struct dirent *entryArray, struct stat entryStat, char *inp_dir, char *out_dir) {
    int nr_scrieri = 0;

    //cream numele fisierului statistica.txt
    char out_name[50];
    strcpy(out_name, entryArray->d_name);
    strcat(out_name, "_");
    strcat(out_name, "statistica.txt"); 

    // functie pt director
    printf("intrarea %s s-a deschis cu succes!\n", entryArray->d_name);
    if( (nr_scrieri = write_dir(out_name, entryStat, entryArray->d_name, inp_dir, out_dir)) != 0) {
        printf("detaliile directorului %s au fost scrise in %s!\n", entryArray->d_name, out_name);
    }

    return nr_scrieri;
}
