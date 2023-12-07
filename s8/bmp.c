int write_bmp(int fd_input, char* outputFileName, struct stat fileStat, char* nume, char* input_dir, char* output_dir) {
    char unused_buffer[1024];
    char out[1024];
    int nr_scrieri = 0;
    // intram in directorul de output
    change_dir("..");
    change_dir(output_dir);
    int fd_output = get_output_file(outputFileName);

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
    // dupa ce scrie incrementam nr_scrieri
    nr_scrieri++;

    if(read(fd_input, &width, 4) == -1) {
        perror("eroare la citire");
        exit(-1);
    }

    sprintf(out, "width: %d\n", width);
    if(write(fd_output, out, strlen(out)) == -1) {
        perror("eroare la scriere");
        exit(-1);
    }
    //incrementam nr_scrieri
    nr_scrieri++;

    if(read(fd_input, &height, 4) == -1) {
        perror("eroare la citire");
        exit(-1);
    }
    sprintf(out, "height: %d\n", height);
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
    // se scriu 3 linii la write_permissions, pentru useri, pentru grup si pentru altii 
    nr_scrieri+=3;

    // la final ne intoarem in directorul de input
    change_dir("..");
    change_dir(input_dir);
 

    // daca returneaza 0 inseamna ca nu s-a scris nimic in fisier
    return nr_scrieri;
}

void convert_bmp(struct dirent *entryArray) {
    char unused_buffer_1[20], unused_buffer_2[30];
    int width, height;
    // ne aflam in directorul de input, nu schimbam direcotrul

    // deschidem fisierul
    int fd_input;

    if((fd_input = open(entryArray->d_name, O_RDWR)) == -1) {
        perror("intrarea nu s-a putut deschide!");
        exit(-1);
    }

    

    // citim urmatorii 54 de bytes din fisierul bmp
    // ca sa ajungem in zona culorilor 
    if(read(fd_input, unused_buffer_1, 18) == -1) {
        perror("eroare la citire");
        exit(-1);
    }
    if(read(fd_input, &width, 4) == -1) {
        perror("eroare la citire");
        exit(-1);
    }
    if(read(fd_input, &height, 4) == -1) {
        perror("eroare la citire");
        exit(-1);
    }
    if(read(fd_input, unused_buffer_2, 28) == -1) {
        perror("eroare la citire");
        exit(-1);
    }

    

    // unsigned char are un singur byte
    unsigned char px[3];
    long int pixelCount = width * height;

    for(long int i = 0; i < pixelCount; i++) {
        
        if(read(fd_input, px, 3) == -1) {
            perror("eroare la citire");
            exit(-1);
        }

        unsigned char gray = 0.299 * px[2] + 0.587 * px[1] + 0.114 * px[0]; 
        memset(px, gray, sizeof(px));
        
        if(write(fd_input, px, 3) == -1) {
            perror("eroare la scriere");
            exit(-1);
        }

    }

}

int bmp_process(int fd_input, struct dirent *entryArray, struct stat entryStat, char* inp_dir, char* out_dir) {
    int nr_scrieri = 0;
    char out_name[50];


    //cream numele fisierului statistica.txt
    strcpy(out_name, entryArray->d_name);
    strcat(out_name, "_");
    strcat(out_name, "statistica.txt");

    

    if( (nr_scrieri = write_bmp(fd_input, out_name, entryStat, entryArray->d_name, inp_dir, out_dir)) != 0) {
        printf("detaliile fisierului BMP %s au fost scrise in %s!\n", entryArray->d_name, out_name);
        
    }   

    int pid;
    // cream al doilea proces pentru bmp
    if((pid = fork()) < 0) {
        perror("Eroare la crearea procesului");
        exit(-1);
    }
    if(pid == 0) {
        convert_bmp(entryArray);
        exit(0);
    }

    int status;
    if ( waitpid(pid, &status, 0) == -1 ) {
        perror("waitpid failed");
        return EXIT_FAILURE;
    }
    
    // mesaj incheiere proces
    printf("!!!S-a incheiat procesul cu PID-ul %d si codul %d!!!\n\n", pid, 0);

    
    
    if(close(fd_input) == -1) {
        perror("fisierul nu s-a putut inchide!");
        exit(-1);
    } else {
        printf("fisierul %s s-a inchis cu succes!\n\n", entryArray->d_name);
    }

    return nr_scrieri;
}