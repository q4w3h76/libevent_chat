#include "callback.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>// for close();

unsigned int active_connections = 0;

void message_processing_callback(evutil_socket_t fd, short what, void* arg)
{
	size_t length = 1024;
    char* message = malloc(length+sizeof(char)*20);
    char* buffer = malloc(length);

	//search for current client
    struct client* current;
    current = search_current_item(clients, fd);
    //adding a name to string
    strcpy(message, current->name);
	//reading message
	int nbytes = recv(current->fd, buffer, length, 0);
	if(nbytes <= 0)
		perror("error call of recv");
    strcpy(&message[strlen(message)-1], ": ");
    strcat(message, buffer);
    //sending message all clients
    struct client* loop = clients;
    while(loop != NULL)
    {
        if(loop->fd != fd)
			send(loop->fd, message, strlen(message), 0);
		loop = loop->next;
    }
    loop = NULL;
    printf("%s", message);
    //checking for disconnection
    if(!strcmp(buffer, "bye\n"))
    {
		event_free(current->evread);
		close(fd);
        delete_item_list(clients, current);
        printf("One of a clients disconnected, currently active connections: %d\n",
            --active_connections);
    }
	else //if client has not disconnected
		event_add(current->evread, NULL);
	free(buffer);
	free(message);
}

void connect_callback(struct evconnlistener* listener, evutil_socket_t fd,
    struct sockaddr* peer, int peerlen, void* arg)
{
    char* buffer = malloc(sizeof(char)*19);
    char* message = malloc(sizeof(char)*50);
	struct event* evread;

    while(1)
        if(recv(fd, buffer, 19, 0) > 0)
        {
            //init info about client
            if(clients == NULL)
            {
                clients = (struct client*)malloc(sizeof(struct client));
                clients->fd = fd;
				clients->evread = evread;
                strcpy(clients->name, buffer);
                clients->next = NULL;
            }
            else
            {
                struct client* tmp;
                tmp = (struct client*)malloc(sizeof(struct client));
                tmp->fd = fd;
				tmp->evread = evread;
                strcpy(tmp->name, buffer);
				tmp->next = clients;
				clients = tmp;
				tmp = NULL;
            }
            printf("New connection under the name: %s", buffer);
            printf("Currently active connections: %d\n", ++active_connections);
            break;
        }
    strcpy(message, "New connection under the name: ");
    strncat(message, buffer, 19);
    //notify everyone about a new connection
    struct client* loop = clients;
    while(loop != NULL)
    {
        if(loop->fd != fd)
            send(loop->fd, message, strlen(message), 0);
        loop = loop->next;
    }
    loop = NULL;
    //deallocate memory
    free(message);
    free(buffer);
	//event registration
	struct event_base* base = evconnlistener_get_base(listener);
	clients->evread = event_new(base, fd, EV_READ, message_processing_callback, NULL);
	event_add(clients->evread, NULL);
}

void connect_error_callback(struct evconnlistener* listener, void* arg)
{
    struct event_base* base = evconnlistener_get_base(listener);

    int err = EVUTIL_SOCKET_ERROR();
    fprintf(stderr, "error = %d = \"%s\"\n", err, evutil_socket_error_to_string(err));

    event_base_loopexit(base, NULL);
}
