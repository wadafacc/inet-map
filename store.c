#include "store.h"
/*
- Write to disk in (fitting) format
*/

void set_bits(int status, unsigned char (*byte_buf), long idx) {
  int byte_idx = idx / 4;
  int bit_idx = 6 - ((idx % 8) * 2);  // only use every second bit as first place -> 4 ips per byte

  byte_buf[byte_idx] |= (status << bit_idx);

  printf("Status: %d | Current Byte: ", status);
  for (int j = 7; j >= 0; j--)
  {                                  // Loop through each bit in the byte (MSB to LSB)
    printf("%d", (byte_buf[byte_idx] >> j) & 1); // Extract and print the j-th bit
  }
}


FILE *store_create(char *fname, int idx) {
  char fullpath[64];
  sprintf(fullpath, "%s/%d_%s", PATH, idx, fname);

  return fopen(fullpath, "wb");
}

/// @brief  write entire byte buffer to file
/// @param ptr 
/// @param byte_buf 
void store_write(FILE *ptr, unsigned char *byte_buf, size_t size) {
  fwrite(byte_buf, 1, size, ptr);
}

void store_close(FILE *ptr) {
  fclose(ptr);
}
