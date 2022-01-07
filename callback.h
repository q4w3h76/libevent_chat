#ifndef CALLBACK_H
#define CALLBACK_H

#include <event2/listener.h>
#include "list.h"

struct client* clients;

void connect_callback(struct evconnlistener* listener, evutil_socket_t fd,
    struct sockaddr* peer, int peerlen, void* arg);
void connect_error_callback(struct evconnlistener* listener, void* arg);

#endif
