#include <stdio.h>

#define PATH "./data"

FILE *create(char *fname, int idx);
void write(FILE *ptr, char *str, int res);
void close(FILE *ptr);