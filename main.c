#include "main.h"

int main()
{
  if (getuid()) {
    printf("ICMP Socket requires root privileges. please rerun with `sudo`.\n");
    exit(0);
  }


  struct timeval timeout;
  int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

  timeout.tv_sec = 0;
  timeout.tv_usec = TIMEOUT_DURATION;

  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
  {
    perror("setsockopt failed");
    close(sockfd);
    return -1;
  }

  struct sockaddr_in dst;
  memset(&dst, 0, sizeof(dst));
  dst.sin_family = AF_INET;

  char buf[16];
  FILE *file_ptr = store_create(FILENAME, 0);

  // writing stuff -> storing each IP's availability as 2 bits
  /*
  00 -> UNSET
  01 -> TIMEOUT
  11 -> OK
  10 -> TBD
  */
  unsigned char byte_buf[(256 * 256 * 256) / 4] = {0};

  int current_oct = 0;
  for (long i = 2130706432; i < 2130706687 + 1; i++) {
    int new_oct = (i >> 24) & 0xFF;
    if (new_oct != current_oct) {
      current_oct = new_oct;

      store_write(file_ptr, byte_buf, sizeof(byte_buf));
      store_close(file_ptr);

      memset(byte_buf, 0, sizeof(byte_buf));
      file_ptr = store_create(FILENAME, current_oct); // update file pointer to next file in row
    }

    // bind ip to dst
    int_to_ip(i, buf);
    if (inet_pton(AF_INET, buf, &dst.sin_addr) <= 0) {
      perror("inet_pton");
      exit(EXIT_FAILURE);
    }

    printf("--- %s ---\n", buf);

    struct icmphdr *icmp_req = ping(sockfd, &dst);
    if (icmp_req == 0) {
      perror("ping");
      // count as 0
      set_bits(0, byte_buf, ip_to_intv3(buf));
      continue;
    }

    int icmp_reply = receive(sockfd);
    set_bits(icmp_reply, byte_buf, ip_to_intv3(buf));

    printf("\n");
  }

  // print_bits(byte_buf, sizeof(byte_buf));

  store_write(file_ptr, byte_buf, sizeof(byte_buf));

  store_close(file_ptr);
}

void int_to_ip(int n, char *buf)
{
  sprintf(buf, "%d.%d.%d.%d", ((n >> 24) & 0xFF), ((n >> 16) & 0xFF), ((n >> 8) & 0xFF), (n & 0xFF));
}

int ip_to_intv3(char *ip)
{
  int oct0, oct1, oct2, oct3;
  sscanf(ip, "%u.%u.%u.%u", &oct0, &oct1, &oct2, &oct3);
  return (oct1 << 16) | (oct2 << 8) | oct3;
}

void print_bits(unsigned char *arr, size_t size)
{
  for (size_t i = 0; i < size; i++)
  { // Loop through each byte in the array
    for (int j = 7; j >= 0; j--)
    {                                  // Loop through each bit in the byte (MSB to LSB)
      printf("%d", (arr[i] >> j) & 1); // Extract and print the j-th bit
    }
    printf(" "); // Space between bytes for readability
  }
  printf("\n");
}
