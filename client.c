#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    int sock;
    if((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
      printf("Socket error!\n");
      return -1;
    }
    // для отправки серверу
    struct sockaddr_in addr_in;
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(7777);
    addr_in.sin_addr.s_addr = inet_addr("192.168.0.105");
    // для приема
    struct sockaddr_in addr_out;
    addr_out.sin_family = AF_INET;
    addr_out.sin_port = htons(7777);
    addr_out.sin_addr.s_addr = inet_addr("192.168.0.255");
    if(bind(sock, (struct sockaddr *)&addr_out, sizeof(addr_out)) < 0){
      printf("Error bind!\n");
      return -1;
    }
    int yes;
    // задаем тип сокета - BROADCAST
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void*)&yes, sizeof(yes));

    int bytes_read;
    switch(fork()){
      case 0: { // считывающий поток
        while(1){
          char buf[512] = "";
          scanf("%s",buf);
          sendto(sock, buf, sizeof(buf), 0, (struct sockaddr*)&addr_in, sizeof(addr_in)); // отправка серверу
        }
      } break;
      default: { // получающий и выводящий поток
        while(1){
          char buf[512] = "";
          int addrlen = sizeof(addr_out);
          bytes_read = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&addr_out, &addrlen); // прием от клиентов
          printf("Сообщение: %s\n",buf);
        }
      } break;
    }
    close(sock);
    return 0;
}
