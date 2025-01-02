#include "pipe_networking.h"
#include <fcntl.h>
#include <unistd.h>


void rando(int * r){
  int fd = open("/dev/random", O_RDONLY, 0);
  read(fd, r, sizeof(int));
  *r = *r % 50 + 50;
  close(fd);
}

int main() {
  while(1){
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
  }
}
