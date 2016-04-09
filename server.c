#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h>

int main(int argc, char const *argv[]) {
  int sock, listener, binding, bytes_read;
  struct sockaddr_in addr;
  char buf[1024];

  listener = socket(AF_INET, SOCK_STREAM, 0);
  if(listener < 0){
    perror("Ошибка создания сокета");
    exit(1);
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(3424);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  binding = bind(listener, (struct sockaddr *)&addr, sizeof(addr));
  if(binding < 0){
    perror("Ошибка Binding");
    exit(2);
  }

  listen(listener, 10);

  while(1){
    sock = accept(listener, NULL, NULL);
    if(sock < 0){
      perror("Ошибка слушающего сокета");
      exit(3);
    }

    while(1){
      bytes_read = recv(sock, buf, sizeof(buf), 0);
      if(bytes_read <= 0) break;
      printf("%s ", buf);
    //  send(sock, buf, bytes_read, 0);
    }

    close(sock);
  }

  return 0;
}
