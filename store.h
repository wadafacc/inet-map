#include <stdio.h>

#define PATH "./data"

FILE *create(char *fname, int idx);
void write(FILE *ptr, int ip, int res);
void close(FILE *ptr);