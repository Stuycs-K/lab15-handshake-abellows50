#include "pipe_networking.h"
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>


void sighandler(int signo){
  if(signo == SIGPIPE){ //child 
    // cleanup();
  }
}

void rando(int * r){
  int fd = open("/dev/random", O_RDONLY, 0);
  read(fd, r, sizeof(int));
  *r = *r % 50 + 50;
  close(fd);
}

int main() {
  while(1){
    printf("establishing connection to client...\n");
    int to_client;
    int from_client;

    from_client = server_handshake( &to_client );

    //actual thing it does
    int r;
    rando(&r);
    while(1){
      if(write(to_client, &r, sizeof(r)) > 0){
        printf("sent %d\n",r);
        rando(&r);
        sleep(1);
      }
      else{
        break;
      }
    }
    printf("client broke connection...\n");
  }
}
