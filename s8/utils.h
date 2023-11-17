#ifndef utils_h
#include "utils.c"

const char *FileSuffix(const char path[]);
void change_dir(const char* dir);
void write_permissions(int fd_output, struct stat fileStat);
int get_output_file(char* outputFileName);

#define utils_h
#endif