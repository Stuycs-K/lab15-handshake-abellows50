#include "pipe_networking.h"

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  int rando;
  while(read(from_server, &rando, sizeof(int))){
    printf("recived: %d\n", rando);
  }
  printf("Server dropped connection...\n");
  printf("terminating...\n");
}
