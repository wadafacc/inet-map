#include "store.h"
/*
- Write to disk in (fitting) format
*/

FILE *store_create(char *fname, int idx) {
  char fullpath[64];
  sprintf(fullpath, "%s/%d_%s", PATH, idx, fname);

  return fopen(fullpath, "w");
}

void store_write(FILE *ptr, int ip, int res) {
  fprintf(ptr,"%d %d\n", ip, res);
}

void store_close(FILE *ptr) {
  fclose(ptr);
}