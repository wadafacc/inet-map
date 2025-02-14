#include "ping.h"

/// @brief returns 0 if request cant be sent 
struct icmphdr *ping(int sockfd, struct sockaddr_in *dst) {
  struct icmphdr *icmp;
  char packet[PACKET_SIZE];  
  memset(packet, 0x0A0B0C0D, PACKET_SIZE);


  // create icmp packet
  icmp = (struct icmphdr *)packet;
  icmp->type = ICMP_ECHO;
  icmp->code = 0;
  icmp->checksum = 0;
  icmp->un.echo.sequence = 1;
  // calc checksum
  icmp->checksum = in_cksum((unsigned short *)icmp, PACKET_SIZE, 0);

  ssize_t bytes_sent = sendto(sockfd, icmp, PACKET_SIZE, 0, (struct sockaddr *)dst, sizeof(*dst));
  if (bytes_sent < 0) {
    return 0;
  }

  return icmp;
}

/// @brief returns 0 if response isn't code=ICMP_ECHOREPLY && type=ICMP_ECHO 
int receive(int sockfd) {
  unsigned char recv_buf[128];
  struct sockaddr_in reply_addr;
  socklen_t addr_len = sizeof(reply_addr);

  int res = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&reply_addr, &addr_len);

  if (res < 0) {
    perror("recvfrom");
    return 0;
  }

  struct iphdr *ip_hdr = (struct iphdr *) recv_buf;
  struct icmphdr *icmp_reply = (struct icmphdr *)((char *)ip_hdr + (4 * ip_hdr->ihl));
  icmp_reply->checksum = in_cksum((unsigned short *)icmp_reply, PACKET_SIZE, 0);

  char reply_ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &reply_addr.sin_addr, reply_ip, sizeof(reply_ip));
  
  printf("ICMP: TYPE: %d, Code: %d, Bytes: %d, IP: %s\n", icmp_reply->type, icmp_reply->code, res, reply_ip);

  if (icmp_reply->code == ICMP_ECHOREPLY) {
    return 1;
  }

  return 0;
}


/*
- HELPERS
*/

// https://github.com/amitsaha/ping/blob/master/ping.c#L26
unsigned short in_cksum(const unsigned short *addr, register int len, unsigned short csum)
{
	register int nleft = len;
	const unsigned short *w = addr;
	register unsigned short answer;
	register int sum = csum;

	while (nleft > 1)  {
		sum += *w++;
		nleft -= 2;
	}

	/* mop up an odd byte, if necessary */
	if (nleft == 1)
		sum += ODDBYTE(*(unsigned char *)w); /* le16toh() may be unavailable on old systems */

	/*
	 * add back carry outs from top 16 bits to low 16 bits
	 */
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);			/* add carry */
	answer = ~sum;				/* truncate to 16 bits */
	return (answer);
}

void print(struct sockaddr *sa) {
  char ipStr[128];
  struct sockaddr_in *addr_in = (struct sockaddr_in *)sa;
  if (inet_ntop(AF_INET, &addr_in->sin_addr, ipStr, sizeof(ipStr)) != NULL) {
    printf("IP: %s\n", ipStr);
  }
}