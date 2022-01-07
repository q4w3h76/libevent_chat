#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>

void recv_loop(int sock);
void send_loop(int sock);

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		printf("enter ip of server\n");
		exit(-1);
	}
	struct sockaddr_in peer;
	int sock;
	memset(&peer, 0, sizeof(peer));
	peer.sin_family = AF_INET;
	peer.sin_port = htons(6000);
	peer.sin_addr.s_addr = inet_addr(argv[1]);
	char name[19];
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if(connect(sock, (struct sockaddr*)&peer, sizeof(peer)) == 0)
	{
		printf("connected to server\nenter yout name: ");
		fgets(name, 19, stdin);
		if(send(sock, name, 19, 0) < 0)
		{
			perror("error call of send");
			exit(-2);
		}
	}
	else
	{
		perror("error call of connect");
		exit(-3);
	}

	pid_t pid;
	switch(pid = fork())
	{
	case 0:
		//child
		recv_loop(sock);
		break;
	case -1:
		//error
		perror("error call of fork");
		exit(-3);
	default:
		//parent
		send_loop(sock);
		kill(pid, SIGTERM);
	}
	close(sock);
	exit(0);
}

void recv_loop(int sock)
{
	char buffer[1024];
	int nbytes;
	do
	{
		nbytes = recv(sock, &buffer, 1024, 0);
		if(nbytes > 0)
		{
			printf("%s", buffer);
			memset(buffer, 0, 1024);
		}
	} while(1);
}

void send_loop(int sock)
{
	char buffer[1024];
	while(strcmp(buffer, "bye\n"))
	{
		memset(buffer, 0, 1024);
		fgets(buffer, 1024, stdin);
		send(sock, buffer, strlen(buffer), 0);
	}
}
