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
  FILE *file_ptr = create("storage.dat", 0);

  int old = ((0 >> 24) & 0xFF);
  // for (int i = 0; i < ((uint64_t)1 << 32); i++)
  for (int i = 2449763866; i < 2449763872; i++)
  {
    int first = (i >> 24) & 0xFF;
    if (first != old)
    {
      old = first;
      close(file_ptr);
      file_ptr = create("storage.dat", old); // update file pointer to next file in row
    }

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
      write(file_ptr, ipv3, icmp_req);
      continue;
    }

    struct icmphdr *icmp_reply = receive(sockfd);
    if (icmp_reply == 0) {
      perror("receive");
    }

    write(file_ptr, ipv3, icmp_reply);
    printf("\n");
  }

  close(file_ptr);
}

// start benchmark timer
// clock_t start_time = clock();
// int count = 0;
// while (((double)(clock() - start_time) / CLOCKS_PER_SEC) <= 1)
// {

//   count++;
// }

// double elapsed = (double)(clock() - start_time) / CLOCKS_PER_SEC;
// printf("Took %f seconds | Requests sent: %d\n", elapsed, count);

void int_to_ip(int n, char *buf)
{
  sprintf(buf, "%d.%d.%d.%d", ((n >> 24) & 0xFF), ((n >> 16) & 0xFF), ((n >> 8) & 0xFF), (n & 0xFF));
}

int ip_to_intv3(char *buf) {
  unsigned int oct[4];
  sscanf(buf, "%u.%u.%u.%u", &oct[0], &oct[1], &oct[2], &oct[3]);

  
  // to int
  unsigned int res = (oct[1] << 16) | (oct[2] << 8) | oct[3]; 
  printf("IPv3: %u.%u.%u / int: %d\n", oct[1], oct[2], oct[3], res);
  return res;
}