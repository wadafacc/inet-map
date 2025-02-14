// benchmark
#include <sys/time.h>
#include <unistd.h>

// custom stuff
#include "ping.h"
#include "store.h"

#define TIMEOUT_DURATION 1000
#define FILENAME "pingu.bin"

void int_to_ip(int n, char *buf);