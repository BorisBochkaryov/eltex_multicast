#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
  int sock;
  // создаем сокет
  if((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
    printf("Socket error!\n");
    return -1;
  }
  int yes;
  // задаем тип сокета (для использования того же адреса/порта)
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*)&yes, sizeof(yes));
  // структура для bind
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(2016);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
    printf("Error bind!\n");
    return -1;
  }
  // структура multicast
  struct ip_mreq mr;
  mr.imr_multiaddr.s_addr = inet_addr("225.0.0.21"); // multicast адрес
  mr.imr_interface.s_addr = htonl(INADDR_ANY); // интерфейс

  // задаем тип сокеты (член multicast группы)
  setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mr, sizeof(mr));
  
  while(1){
    char buf[512] = "";
    int addrlen = sizeof(addr);
    int bytes_read = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&addr, &addrlen); // принимаем данные с multicast
    printf(">%s\n",buf); // выводим принятую информацию
  }
  close(sock);
  return 0;
}
