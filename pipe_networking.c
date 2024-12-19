#include "pipe_networking.h"

#define MAX_PP_LEN 32

//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  mkfifo("WKP", 0666);
  //open WKP and block
  printf("created WKP...\n");
  int from_client = open("WKP",O_RDONLY, 0);
  printf("WKP has opened...\n");

  remove("WKP");
  printf("WKP file removed...\n");

  return from_client;
}

/*=========================
  server_handshake 
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  /*============================
  HANDSHAKE: Recieve SYN (PP name)
             Send back SYN-ACK (pid)
             recieve and check ACK (pid - 1)?
             ==================================*/

  int from_client = server_setup();
  printf("obtained server link...\n");

  //read the name of the PP -- this is the SYN
  char pp_name[MAX_PP_LEN];
  read(from_client, pp_name, MAX_PP_LEN);
  printf("read pp_id name...\n");

  //open the pp_name and set to_client
  int to_client_real = open(pp_name, O_WRONLY, 0);
  *to_client = to_client_real;
  printf("oppened the pp_name channel...\n");

  //send back SYN-ACK
  int syn_ack = getpid();
  write(to_client_real, &syn_ack, sizeof(int));

  //recieve and confirm that ACK matches pid-1;
  int ack;
  read(from_client, &ack, sizeof(int));

  if (ack != syn_ack - 1){
    perror("breaking connection attempt - handshake failed");
    exit(1); //err on response!
  }
  
  //Handshake complete
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  *to_server = open("WKP", O_WRONLY, 0);
  if(*to_server == -1){
    perror("err: server has not yet created WKP...");
    exit(1);
  }

  char pp_name[MAX_PP_LEN];
  sprintf(pp_name, "%d", getpid());
  mkfifo(pp_name, 0666);

  int from_server = open(pp_name, O_RDONLY, 0);

  write(*to_server, pp_name, MAX_PP_LEN);

  int syn_ack;
  read(from_server, &syn_ack, sizeof(int));

  int ack = syn_ack - 1;
  write(*to_server, &ack, sizeof(int));

  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  int to_client  = 0;
  return to_client;
}


