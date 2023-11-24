#ifndef dir_h
#include "dir.c"

int write_dir(char* outputFileName, struct stat dirStat, char* nume, char* input_dir, char* output_dir);
int dir_process(struct dirent *entryArray, struct stat entryStat, char *inp_dir, char *out_dir);

#define dir_h
#endif