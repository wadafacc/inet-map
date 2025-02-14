// benchmark
#include <sys/time.h>
#include <unistd.h>

// custom stuff
#include "ping.h"
#include "store.h"

#define TIMEOUT_DURATION 1000
#define FILENAME "pingu.dat"

void int_to_ip(int n, char *buf);
int ip_to_intv3(char *buf);