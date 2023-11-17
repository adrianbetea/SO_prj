#ifndef file_h
#include "file.c"

int write_file(int fd_input, char* outputFileName, struct stat fileStat, char* nume, char* input_dir, char* output_dir);
int file_process(int fd_input, struct dirent *entryArray, struct stat entryStat, char *inp_dir, char* out_dir);

#define file_h
#endif