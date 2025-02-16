// benchmark
#include <sys/time.h>
#include <unistd.h>

// custom stuff
#include "ping.h"
#include "store.h"

#define TIMEOUT_DURATION 1000
#define FILENAME "pingu.bin"

void int_to_ip(int n, char *buf);
int ip_to_intv3(char *ip);
void print_bits(unsigned char *arr, size_t size);