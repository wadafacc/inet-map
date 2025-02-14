#include "store.h"
/*
- Write to disk in (fitting) format
*/

FILE *store_create(char *fname, int idx) {
  char fullpath[64];
  sprintf(fullpath, "%s/%d_%s", PATH, idx, fname);

  return fopen(fullpath, "wb");
}

void store_write(FILE *ptr, int bit, unsigned char *bit_buf, int *bit_count) {
  *bit_buf = (*bit_buf << 1) | (bit & 1);
  (*bit_count)++;

  // full byte to write
  if (*bit_count == 8) {
    fwrite(bit_buf, 1, 1, ptr);
    *bit_buf = 0;
    *bit_count = 0;
  }
}

void store_flush(FILE *ptr, unsigned char *bit_buf, int *bit_count) {
  if (*bit_count > 0) {
    *bit_buf <<= (8 - *bit_count);
    fwrite(bit_buf, 1, 1, ptr);
  }
}

void store_close(FILE *ptr) {
  fclose(ptr);
}