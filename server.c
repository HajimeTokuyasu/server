#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <wiringPi.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 50000


enum {
  _CW_ = 0,//
  _CCW_,//1
  _pinINT0_,//2
  _pinINT1_,//3
  __,//4
};
enum {

  _REAR_FWD_ = 8,//
  _REAR_REV_//9
};

//server_code_lastmod20160408
//with_many_pin

int main (int argc, char** argv){
   int i;
   int num_recv;
   char recv_dir;
   int fd_sock;
   socklen_t len;   
   struct sockaddr_in svaddr, claddr;

   fd_sock = socket(AF_INET, SOCK_DGRAM, 0);
   if (fd_sock == -1) {
      printf("missing_sock\n");
      return -1;
   }

   memset(&svaddr, 0, sizeof(struct sockaddr_in));
   
   svaddr.sin_family = AF_INET;
   svaddr.sin_port = htons(PORT);
   svaddr.sin_addr.s_addr = htonl(INADDR_ANY);//!

   if (bind(fd_sock, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_in)) == -1) {
      printf("bind_error\n");
      return -1;
   }
      puts("bind_done\n");
   if(wiringPiSetup() == -1){
     printf("error wiringPi setup\n");
     return 1;
   }
      puts("wiringPi_done\n");
   for(i=_CW_;i<=_REAR_REV_;i++){
     pinMode(i,OUTPUT);
     digitalWrite(i,LOW);
   }
   puts("pin_done\n");
  //len = sizeof(struct sockaddr_in); 

   while (1){
     num_recv = recvfrom(fd_sock, &recv_dir, sizeof(char),0, (struct sockaddr*)&claddr, &len); 
    // if(num_recv == -1) return -1;
     if(num_recv == 0) continue;
     puts("newloop\n");//not printed
     switch(recv_dir){
      case 'R':
        digitalWrite(_pinINT0_,HIGH);
        digitalWrite(_CW_,HIGH);
        puts("Right\n");
        digitalWrite(_pinINT0_,LOW);
        break;
      case 'L':
        digitalWrite(_pinINT0_,HIGH);
        digitalWrite(_CCW_,HIGH);
        puts("Left\n");
        digitalWrite(_pinINT0_,LOW);
        break;
      case 'K':
        puts("Keep\n");
        break;
      case 'N':
        puts("__Newtral__\n");
        break;
      case 'F':
        digitalWrite(_pinINT1_,HIGH);
        digitalWrite(_REAR_FWD_,HIGH); 
        puts("__Foward__\n");
        digitalWrite(_pinINT1_,LOW);
        break;
      case 'B':
        digitalWrite(_pinINT1_,HIGH);
        digitalWrite(_REAR_REV_,HIGH);
        puts("__Back__\n");
        digitalWrite(_pinINT1_,LOW);
        break;
     }
   };
}
