#include "callback.h"
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(6000);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//work with libevent
	struct event_base* base = event_base_new();
	struct evconnlistener* listener = evconnlistener_new_bind(base, connect_callback, NULL,
		LEV_OPT_REUSEABLE, -1, (struct sockaddr*)&addr, sizeof(addr));
	if(!listener)
	{
		perror("Couldn't create listener");
		exit(-1);
	}
	evconnlistener_set_error_cb(listener, connect_error_callback);

	event_base_dispatch(base);

	delete_list(clients);
	close(evconnlistener_get_fd(listener));
	exit(0);
}
