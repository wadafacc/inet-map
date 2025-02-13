#include "main.h"

int main()
{
  int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

  struct sockaddr_in dst;
  memset(&dst, 0, sizeof(dst));
  dst.sin_family = AF_INET;

  char buf[16];
  FILE *file_ptr = create("storage.dat", 0);

  int old = ((0 >> 24) & 0xFF);
  for (int i = 0; i < ((uint64_t)1 << 8); i++)
  {
    int first = (i >> 24) & 0xFF;
    if (first != old)
    {
      old = first;
      close(file_ptr);
      file_ptr = create("storage.txt", old); // update file pointer to next file in row
    }

    int_to_ip(i, buf);
    printf("IP: %s | ", buf);

    if (inet_pton(AF_INET, buf, &dst.sin_addr) <= 0)
    {
      perror("inet_pton");
      exit(EXIT_FAILURE);
    }

    struct icmphdr *icmp_req = ping(sockfd, &dst);
    if (icmp_req == 0)
    {
      printf("euou send\n");
      // count as 0
    }
    struct icmphdr *icmp_reply = receive(sockfd);
    if (icmp_reply == 0)
    {
      printf("euou recv\n");
      // count as 0
    }

    write(file_ptr, buf, icmp_reply);
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