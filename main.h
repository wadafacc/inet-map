// benchmark
#include <sys/time.h>

// custom stuff
#include "ping.h"
#include "store.h"

#define TIMEOUT_DURATION 1000

void int_to_ip(int n, char *buf);
int ip_to_intv3(char *buf);