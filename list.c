#include "list.h"
#include <stdlib.h>

void delete_item_list(struct client* head, struct client* del)
{
    struct client* tmp = head;
    struct client* buf;
    if(tmp->next != NULL)
    {
        while(tmp->next != del)
            tmp = tmp->next;
        buf = tmp->next;
        tmp->next = tmp->next->next;
        free(buf);
	buf = NULL
    }
    else
    {
        free(head);
        head = NULL;
    }
}

struct client* search_current_item(struct client* head, evutil_socket_t fd)
{
    struct client* current = head;
    while(current != NULL)
    {
        if(fd == current->fd)
            return current;
        current = current->next;
    }
    return NULL;
}

void delete_list(struct client* head)
{
	struct client* tmp;
    while(head != NULL)
    {
        tmp = head;
        head = head->next;
        free(tmp);
    }
}
