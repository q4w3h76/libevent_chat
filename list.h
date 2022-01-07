#ifndef LIST_H
#define LIST_H
#include <event2/util.h>

struct client
{
	char name[19];
	evutil_socket_t fd;
	struct event* evread;
	struct client* next;
};

void delete_item_list(struct client* head, struct client* del);
struct client* search_current_item(struct client* head, evutil_socket_t fd);
void delete_list(struct client* head);

#endif
