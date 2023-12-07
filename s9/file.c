int write_file(int fd_input, char* outputFileName, struct stat fileStat, char* nume, char* input_dir, char* output_dir) {
    char out[1024];
    int nr_scrieri = 0;

    // intram in directorul de output
    change_dir("..");
    change_dir(output_dir);
    int fd_output = get_output_file(outputFileName);

    

    sprintf(out, "nume fisier: %s\n", nume);
    if(write(fd_output, out, strlen(out)) == -1) {
        perror("eroare la scriere");
        exit(-1);
    }
    nr_scrieri++;

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
    nr_scrieri++;

    // USER ID
    sprintf(out, "user_id: %d\n", fileStat.st_uid);
    if(write(fd_output, out, strlen(out)) == -1) {
        perror("eroare la scriere");
        exit(-1);
    }
    nr_scrieri++;

    // NUMARUL DE LEGATURI
    sprintf(out, "nr legaturi: %ld\n", fileStat.st_nlink);
    if(write(fd_output, out, strlen(out)) == -1) {
        perror("eroare la scriere");
        exit(-1);
    }
    nr_scrieri++;

    // DATA ULTIMEI MODIFICARI
    sprintf(out, "Last modified time: %s", ctime(&fileStat.st_mtime));
    if(write(fd_output, out, strlen(out)) == -1) {
        perror("eroare la scriere");
        exit(-1);
    }
    nr_scrieri++;

    // functie pentru scrierea permisiunilor
    write_permissions(fd_output, fileStat);
    nr_scrieri+=3;

    // la final ne intoarem in directorul de input
    change_dir("..");
    change_dir(input_dir);

    // daca returneaza 0 inseamna ca nu s-a scris nimic in fisier
    return nr_scrieri;
}

int file_process(int fd_input, struct dirent *entryArray, struct stat entryStat, char *inp_dir, char* out_dir) {

    int nr_scrieri=0;
    char out_name[50];
    //cream numele fisierului statistica.txt
    strcpy(out_name, entryArray->d_name);
    strcat(out_name, "_");
    strcat(out_name, "statistica.txt");

    if( (nr_scrieri = write_file(fd_input, out_name, entryStat, entryArray->d_name, inp_dir, out_dir)) != 0) {
        
        printf("detaliile fisierului %s au fost scrise in %s!\n", entryArray->d_name, out_name);
        if(close(fd_input) == -1) {
            perror("fisierul nu s-a putut inchide!");
        } else {
        printf("fisierul %s s-a inchis cu succes!\n\n", entryArray->d_name);
        
        }
    } 
    return nr_scrieri;
}

int get_content(int fd_input, struct stat entryArray, char *inp_dir, char*out_dir, char *buffer) {
    char temp_buffer[1024];

    if(read(fd_input, temp_buffer, sizeof(temp_buffer)) == -1) {
        perror("eroare la citire");
        exit(-1);
    }
    // salvam continutul fisierului in buffer
    strcpy(buffer, temp_buffer);

    return 0;

}