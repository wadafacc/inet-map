#include <stdio.h>

#define PATH "./data"

FILE *store_create(char *fname, int idx);

void store_write(FILE *ptr, int bit, unsigned char *bit_buf, int *bit_count);
void store_flush(FILE *ptr, unsigned char *bit_buf, int *bit_count);

void store_close(FILE *ptr);