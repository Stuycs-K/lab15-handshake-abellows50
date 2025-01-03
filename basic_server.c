#include "pipe_networking.h"
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>


void sighandler(int signo){
  switch(signo){ //child 
    case SIGPIPE:
      {
        break;
      }
    case SIGINT:
      {
        remove("WKP");
        perror("Exiting due to ctrl-c...");
        exit(0);
      }
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
  signal(SIGINT, sighandler);
  while(1){
    printf("establishing connection to client...\n");
    int to_client;
    int from_client;

    from_client = server_handshake( &to_client );

    //actual thing it does
    int r;
    rando(&r);
    while(write(to_client, &r, sizeof(r))){
        printf("sent %d\n",r);
        rando(&r);
        sleep(1);
    }
    close(to_client);
    printf("client broke connection...\n");
  }
}
