#ifndef utils_h
#include "utils.c"

void change_dir(const char* dir);

void write_permissions(int fd_output, struct stat fileStat);
int write_bmp(int fd_input, int fd_output, struct stat entryStat, char* nume);
int write_file(int fd_input, int fd_output, struct stat entryStat, char* nume);

int write_symbolic_link(int fd_output, struct stat entryStat, struct stat targetStat, char* nume);

int write_dir(int fd_output, struct stat dirStat, char* nume);
#define utils_h
#endif
