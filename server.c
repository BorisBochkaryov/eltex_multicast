#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
  int sock;
  // создаем сокет
  if((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
    printf("Socket error!\n");
    return -1;
  }
  // multicast адрес
  struct sockaddr_in addr_m;
  addr_m.sin_family = AF_INET;
  addr_m.sin_port = htons(2016);
  addr_m.sin_addr.s_addr = inet_addr("225.0.0.21");

  char msg[512] = "Hello, World!";
  while(1){
    sendto(sock, msg, sizeof(msg), 0, (struct sockaddr*)&addr_m, sizeof(addr_m)); // отправляем на multicast
    sleep(2); // пауза
  }
  close(sock);
  return 0;
}
