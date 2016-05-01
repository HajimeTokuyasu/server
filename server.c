#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <wiringPi.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 50000

enum {
  STEER_MOVE_PIN = 4, //4
  STEER_DIR_PIN, //5
  LINEAR_MOVE_PIN, // 6
  LINEAR_DIR_PIN // 7
};

enum {
  CW = LOW,
  CCW = HIGH
};

enum {
  FRONT = LOW,
  BACK = HIGH
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
    puts("missing_sock");
    return -1;
  }

  memset(&svaddr, 0, sizeof(struct sockaddr_in));
   
  svaddr.sin_family = AF_INET;
  svaddr.sin_port = htons(PORT);
  svaddr.sin_addr.s_addr = htonl(INADDR_ANY);//!

  if (bind(fd_sock, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_in)) == -1) {
    puts("ERROR:bind");
    return -1;
  }
  puts("bind_done");
  if(wiringPiSetup() == -1){
    puts("ERROR:wiringPi_setup");
    return 1;
  }
  puts("wiringPi_done");
  for(i = STEER_MOVE_PIN; i <= LINEAR_DIR_PIN; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  puts("pin_done");

  while (1){
    num_recv = recvfrom(fd_sock, &recv_dir, sizeof(char),0, (struct sockaddr*)&claddr, &len); 
    if(num_recv == 0) continue;
    puts("newloop");
    switch (recv_dir) {
    case 'K':
      digitalWrite(STEER_MOVE_PIN, LOW);
      puts("Keep");
      break;
    case 'R':
      digitalWrite(STEER_DIR_PIN, CW);
      digitalWrite(STEER_MOVE_PIN, HIGH);
      break;
    case 'L':
      digitalWrite(STEER_DIR_PIN, CCW);
      digitalWrite(STEER_MOVE_PIN, HIGH);
      break;
    case 'N':
      digitalWrite(LINEAR_MOVE_PIN, LOW);
      puts("__Newtral__");
      break;
    case 'F':
      digitalWrite(LINEAR_DIR_PIN, FRONT);
      digitalWrite(LINEAR_MOVE_PIN, HIGH);
      break;
    case 'B':
      digitalWrite(LINEAR_DIR_PIN, BACK);
      digitalWrite(LINEAR_MOVE_PIN, HIGH);
      break;
    }
  }
}
