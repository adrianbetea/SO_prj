#ifndef symlink_h
#include "symlink.c"

int write_symbolic_link(char* outputFileName, struct stat entryStat, struct stat targetStat, char* nume, char* input_dir, char* output_dir);
int symlink_process(struct dirent *entryArray, struct stat entryStat, char *inp_dir, char *out_dir);

#define symlink_h
#endif