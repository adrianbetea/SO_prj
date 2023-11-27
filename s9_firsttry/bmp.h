#ifndef bmp_h
#include "bmp.c"

int write_bmp(int fd_input, char* outputFileName, struct stat fileStat, char* nume, char* input_dir, char* output_dir);
void convert_bmp(struct dirent *entryArray);
int bmp_process(int fd_input, struct dirent *entryArray, struct stat entryStat, char* inp_dir, char* out_dir);

#define bmp_h
#endif