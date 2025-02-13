#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>

#define PACKET_SIZE 64  // Total packet size (ICMP header + data)
#define ODDBYTE(v)	(v)

struct icmphdr *ping(int sockfd, struct sockaddr_in *addr);
struct icmphdr *receive(int sockfd);

void print(struct sockaddr *sa);
unsigned short in_cksum(const unsigned short *addr, register int len, unsigned short csum);