#include "store.h"
/*
- Write to disk in (fitting) format
*/

FILE *create(char *fname, int idx) {
  char fullpath[64];
  sprintf(fullpath, "%s/%d_%s", PATH, idx, fname);

  return fopen(fullpath, "w");
}

void write(FILE *ptr, int ip, int res) {
  fprintf(ptr,"%d %d\n", ip, res);
}

void close(FILE *ptr) {
  fclose(ptr);
}