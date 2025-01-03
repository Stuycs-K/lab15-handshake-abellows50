#include "pipe_networking.h"
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

void rando(int * r);
void server_cycle();
void sighandler(int signo);

int to_client;
int from_client;

void rando(int * r){
  int fd = open("/dev/random", O_RDONLY, 0);
  read(fd, r, sizeof(int));
  *r = *r % 50 + 50;
  close(fd);
}

void server_cycle(){
  printf("establishing connection to client...\n");

  from_client = server_handshake( &to_client );

  //actual thing it does
  int r;
  rando(&r);
  int bytes;
  while(1){
    bytes = write(to_client, &r, sizeof(r));

    if(bytes != 0 && bytes != -1){
      printf("sent %d\n",r);
      rando(&r);
      sleep(1);
    }
    else{
      break;
    }
    
  }
  printf("client broke connection...\n\n");
}

void sighandler(int signo){
  if(signo == SIGPIPE){ //child 
    close(to_client);
    close(from_client);
    to_client = 0;
    from_client = 0;
  }
}


int main() {
  signal(SIGPIPE, sighandler);
  
  printf("PERSISTANT SERVER...\n");
  printf("SERVER WILL STAY ACTIVE AND SERVES ONE CLIENT AT A TIME...\n\n");


  while(1){
    server_cycle();
  }
  perror("Something has gone wrong. Exiting...");
}

