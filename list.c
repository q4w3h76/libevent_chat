#include "list.h"
#include <stdlib.h>

void delete_item_list(struct client* head, struct client* del)
{
    struct client* tmp = head->next;
    struct client* buf;
    if(head && head != del)
    {
        while(tmp && tmp->next != del)
            tmp = tmp->next;
        if(tmp)
        {
            buf = tmp->next;
            tmp->next = tmp->next->next;
            free(buf);
        }
    }
    else
    {
        buf = del;
        del = del->next;
        free(buf);
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
