#ifndef utils_h
#include "utils.c"

void change_dir(const char* dir);

void write_permissions(int fd_output, struct stat fileStat);
int write_bmp(int fd_input, char* outputFileName, struct stat entryStat, char* nume, char* input_dir, char* ouptut_dir);
int write_file(int fd_input, char* outputFileName, struct stat fileStat, char* nume, char* input_dir, char* output_dir);

int write_symbolic_link(char* outputFileName, struct stat entryStat, struct stat targetStat, char* nume, char* input_dir, char* output_dir);

int write_dir(char* outputFileName, struct stat dirStat, char* nume, char* input_dir, char* output_dir);
#define utils_h
#endif
