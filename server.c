#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    int sock;
    if((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0){
      printf("Socket error!\n");
      return -1;
    }
    // для приема
    struct sockaddr_in addr_in;
    addr_in.sin_family = AF_INET;
    addr_in.sin_port = htons(7777);
    addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock, (struct sockaddr *)&addr_in, sizeof(addr_in)) < 0){
      printf("Error bind!\n");
      return -1;
    }
    // для отправки
    struct sockaddr_in addr_out;
    addr_out.sin_family = AF_INET;
    addr_out.sin_port = htons(7777);
    addr_out.sin_addr.s_addr = inet_addr("192.168.0.255");

    int yes;
    // тип сокета - BROADCAST
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *) &yes, sizeof(yes));

    int bytes_read;
    char buf[512] = "", str[512] = "";
    while(1){
      buf[512] = "",str[512] = "";
      int addrlen = sizeof(addr_in);
      // принимаем
      bytes_read = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&addr_in, &addrlen); // прием от клиентов
      if(buf[0] != '@'){ // если это сообщение не от сервера
        sprintf(str,"@%s",buf); // дописываем символ
	printf("%s",str);
        // отправляем клиентам
        sendto(sock, str, sizeof(str), 0, (struct sockaddr*)&addr_out, sizeof(addr_out)); // отправка всем клиентам
      }
    }
    close(sock);
    return 0;
}
