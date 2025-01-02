#include "pipe_networking.h"

int main() {
  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );

  char line[1000];
  while(read(from_client, line, 1000)){
    printf("recived: %s\n", line);
  }
}
