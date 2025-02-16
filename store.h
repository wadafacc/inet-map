#include <stdio.h>

#define PATH "./data"

void set_bits(int status, unsigned char *byte_buf, long idx);

FILE *store_create(char *fname, int idx);
void store_write(FILE *ptr, unsigned char *byte_buf, size_t size);
void store_close(FILE *ptr);