#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>

int sendall(int s, char *buf, int len, int flags)
{
    int total = 0;
    int n;

    while(total < len)
    {
        n = send(s, buf+total, len-total, flags);
        if(n == -1) break;
        total += n;
    }

    return (n==-1 ? -1 : total);
}

int main(int argc, char const *argv[]) {
  int sock, conneting, sending;
  struct sockaddr_in addr;
  char msg[] = "Test\n";

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock < 0){
    perror("Ошибка создания сокета!");
    exit(1);
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(3424);
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  conneting = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
  if(conneting < 0){
    perror("Ошибка подключения");
    exit(2);
  }

  while(1){
    sending = sendall(sock, msg, sizeof(msg), 0);
    if(sending > 0) {
      printf("Успешная передача!\n");
    }
    sleep(1);
  }

  close(sock);

  return 0;
}
