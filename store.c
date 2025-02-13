#include "store.h"
/*
- Write to disk in (fitting) format
*/

FILE *create(char *fname, int idx) {
  char fullpath[64];
  sprintf(fullpath, "%s/%d_%s", PATH, idx, fname);

  return fopen(fullpath, "w");
}

void write(FILE *ptr, char *str, int res) {
  fprintf(ptr,"%s=%d\n", str, res);
}

void close(FILE *ptr) {
  fclose(ptr);
}