#include "main.h"

int main()
{
  struct timeval timeout;
  int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

  timeout.tv_sec = 0;
  timeout.tv_usec = TIMEOUT_DURATION;
  
  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
    perror("setsockopt failed");
    close(sockfd);
    return -1;
  }

  struct sockaddr_in dst;
  memset(&dst, 0, sizeof(dst));
  dst.sin_family = AF_INET;

  char buf[16];
  FILE *file_ptr = store_create(FILENAME, 0);
    
  int old_oct;
  for (int i = 0; i < ((uint64_t)1 << 32) ; i++)
  {
    int new_oct = (i >> 24) & 0xFF;
    if (new_oct != old_oct) {
      old_oct = new_oct;
      store_close(file_ptr);
      file_ptr = store_create(FILENAME, old_oct); // update file pointer to next file in row
    }

    // bind ip to dst
    int_to_ip(i, buf);
    if (inet_pton(AF_INET, buf, &dst.sin_addr) <= 0)
    {
      perror("inet_pton");
      exit(EXIT_FAILURE);
    }

    printf("\n -- IP: %s -- \n", buf);
    int ipv3 = ip_to_intv3(buf);

    struct icmphdr *icmp_req = ping(sockfd, &dst);
    if (icmp_req == 0)
    {
      perror("ping");
      // count as 0
      store_write(file_ptr, ipv3, 0);
      continue;
    }

    int icmp_reply = receive(sockfd);

    store_write(file_ptr, ipv3, icmp_reply);
    printf("\n");
  }

  store_close(file_ptr);
}


void int_to_ip(int n, char *buf)
{
  sprintf(buf, "%d.%d.%d.%d", ((n >> 24) & 0xFF), ((n >> 16) & 0xFF), ((n >> 8) & 0xFF), (n & 0xFF));
}

int ip_to_intv3(char *buf) {
  unsigned int oct[4];
  sscanf(buf, "%u.%u.%u.%u", &oct[0], &oct[1], &oct[2], &oct[3]);

  unsigned int res = (oct[1] << 16) | (oct[2] << 8) | oct[3]; 
  printf("IPv3: %u.%u.%u / int: %d\n", oct[1], oct[2], oct[3], res);
  return res;
}