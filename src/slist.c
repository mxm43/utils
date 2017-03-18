#include <assert.h>
#include <stdlib.h>

#include "slist.h"

slist_t * slist_node_new (void * data)
{
    slist_t * new_node = (slist_t *) malloc (sizeof (slist_t));
    if (new_node == NULL)
        return NULL;

    new_node->data = data;
    new_node->next = NULL;

    return new_node;
}

void slist_node_free (slist_t * node)
{
    assert (node != NULL);

    free (node);
}

slist_t * slist_insert (slist_t * slist, void * data)
{
    slist_t * new_node  = slist_node_new (data);

    new_node->next = slist;

    return new_node;
}

slist_t * slist_find (slist_t * slist, void * data)
{
    while (slist && slist->data != data)
        slist = slist->next;

    return slist;
}

slist_t * slist_foreach (slist_t * slist, func f, void * user_data)
{
    assert (f != NULL);

    slist_t * head = slist;

    while (slist)
    {
        f (slist->data, user_data);
        slist = slist->next;
    }

    return slist;
}

void slist_cleanup (slist_t * slist, func f, void * user_data)
{
    assert (f != NULL);

    while (slist)
    {
        slist_t * next = slist->next;
        f (slist->data, user_data);
        slist_node_free (slist);
        slist = next;
    }
}

slist_t * slist_remove_if (slist_t * slist, func f, void * data, void * user_data)
{
    assert (f != NULL);

    slist_t *  head = slist;
    slist_t ** cur  = &head;

    while (*cur)
    {
        slist_t * entry = *cur;
        if (data == (*cur)->data)
        {
            *cur = entry->next;
            f (entry->data, user_data);
            slist_node_free (entry);
        }
        else
            cur = &(entry->next);
    }

    return head;
}
