#include "pipe_networking.h"

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );

  char line[1000];
  while(fgets(line, 1000, stdin)){
    write(to_server, line, 1000);
  }
}
