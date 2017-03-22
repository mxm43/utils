#include <assert.h>
#include <stdlib.h>

#include "slist.h"

struct slist *
slist_node_new (void *data)
{
    struct slist *new_node = (struct slist *)malloc(sizeof(struct slist));
    if (new_node == NULL)
        return NULL;

    new_node->data = data;
    new_node->next = NULL;

    return new_node;
}

void
slist_node_free (struct slist *node)
{
    assert (node != NULL);

    free (node);
}

struct slist *
slist_insert (struct slist *list, void *data)
{
    struct slist *new_node  = slist_node_new (data);

    new_node->next = list;

    return new_node;
}

struct slist *
slist_find (struct slist *list, void *data)
{
    while (list && list->data != data)
        list = list->next;

    return list;
}

struct slist *
slist_foreach      (struct slist *list,
                    int (*func)      (void *data, void *user_data),
                    void *user_data)
{
    assert (func != NULL);

    struct slist *head = list;

    while (list)
    {
        func (list->data, user_data);
        list = list->next;
    }

    return list;
}

struct slist *
slist_remove_if (struct slist *list,
                 int (*will_rm)   (void *data, void *user_data),
                 int (*free_data) (void *data, void *user_data),
                 void * user_data)
{
    assert (will_rm != NULL && free_data != NULL);

    struct slist  *head = list;
    struct slist **cur  = &head;

    while (*cur)
    {
        struct slist *entry = *cur;
        if (will_rm(entry->data, user_data))
        {
            *cur = entry->next;
            free_data (entry->data, user_data);
            slist_node_free (entry);
        }
        else
            cur = &(entry->next);
    }

    return head;
}

void
slist_cleanup (struct slist *list,
               int (*free_data) (void *data, void *user_data),
               void * user_data)
{
    assert (free_data != NULL);

    while (list)
    {
        struct slist *next = list->next;
        free_data (list->data, user_data);
        slist_node_free (list);
        list = next;
    }
}

