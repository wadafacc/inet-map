#include <stdio.h>

#define PATH "./data"

FILE *store_create(char *fname, int idx);
void store_write(FILE *ptr, int ip, int res);
void store_close(FILE *ptr);