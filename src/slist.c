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
slist_foreach (struct slist *list,
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

    while (list != NULL)
    {
        struct slist *next = list->next;
        free_data (list->data, user_data);
        slist_node_free (list);
        list = next;
    }
}

struct slist *
slist_merge (struct slist *list1,
             struct slist *list2,
             int (*compare) (void *data1, void *data2))
{
    assert (compare != NULL);

    struct slist  *ret = NULL;
    struct slist **cur = &ret;

    while (list1 != NULL && list2 != NULL)
    {
        if (compare (list1->data, list2->data) < 0)
        {
            *cur = list1;
            list1 = list1->next;
        }
        else
        {
            *cur = list2;
            list2 = list2->next;
        }
        cur = &((*cur)->next);
    }

    if (list1 == NULL)
        *cur = list2;
    else
        *cur = list1;

    return ret;
}

struct slist *
slist_sort (struct slist *list,
            int (*compare) (void *data1, void *data2))
{
    assert (compare != NULL);

    if (list == NULL || list->next == NULL)
        return list;

    struct slist *list1 = list;
    struct slist *list2 = list;
    struct slist *prev  = NULL;

    for (int i = 0; list1; ++i)
    {
        list1 = list1->next;
        if (i % 2 == 0)
        {
            prev  = list2;
            list2 = list2->next;
        }
    }
    prev->next = NULL;

    return slist_merge (slist_sort (list,  compare),
                        slist_sort (list2, compare),
                        compare);
}

