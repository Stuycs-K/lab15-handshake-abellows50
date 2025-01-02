all: client server
client: basic_client.o pipe_networking.o
	gcc -o -g client basic_client.o pipe_networking.o

server: basic_server.o pipe_networking.o
	gcc -o -g server basic_server.o pipe_networking.o

basic_client.o: basic_client.c pipe_networking.h
	gcc -c -g basic_client.c

basic_server.o: basic_server.c pipe_networking.h
	gcc -c -g basic_server.c

pipe_networking.o: pipe_networking.c pipe_networking.h
	gcc -c -g pipe_networking.c

clean:
	rm *.o
	rm *~
