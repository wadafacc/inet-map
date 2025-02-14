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
    
  int old_oct = 0;

  // writing stuff -> storing each IP's availability as a bit
  unsigned char bit_buf = 0;
  int bit_count = 0;
  
  for (int i = 2549656576; i < 2549656831 + 1; i++)
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

    printf("--- %s ---\n", buf);

    struct icmphdr *icmp_req = ping(sockfd, &dst);
    if (icmp_req == 0)
    {
      perror("ping");

      // count as 0
      store_write(file_ptr, icmp_req, &bit_buf, &bit_count);
      continue;
    }

    int icmp_reply = receive(sockfd);

    store_write(file_ptr, icmp_reply, &bit_buf, &bit_count);
    printf("\n");
  }

  // make sure to flush
  store_flush(file_ptr, &bit_buf, &bit_count);

  store_close(file_ptr);
}


void int_to_ip(int n, char *buf)
{
  sprintf(buf, "%d.%d.%d.%d", ((n >> 24) & 0xFF), ((n >> 16) & 0xFF), ((n >> 8) & 0xFF), (n & 0xFF));
}
