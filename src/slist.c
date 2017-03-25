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
    struct slist *new_node = slist_node_new (data);

    new_node->next = list;

    return new_node;
}

struct slist *
slist_sort_insert (struct slist *list,
                   int (*compare)   (void *data1, void *data2),
                   void *data)
{
    assert (compare != NULL);

    struct slist *new_node = slist_node_new (data);

    if (list == NULL || compare (new_node->data, list->data) < 0)
    {
        new_node->next = list;
        return new_node;
    }

    struct slist *cur = list;
    while (cur->next != NULL)
    {
        if (compare (new_node->data, (cur->next)->data) < 0)
            break;
        cur = cur->next;
    }
    new_node->next = cur->next;
    cur->next = new_node;

    return list;
}

struct slist *
slist_append (struct slist *list1, struct slist *list2)
{
    if (list1 == NULL)
        return list2;

    struct slist *cur = list1;
    while (cur->next != NULL)
        cur = cur->next;
    cur->next = list2;

    return list1;
}

struct slist *
slist_find (struct slist *list,
            int (*find) (void *data, void *user_data),
            void *user_data)
{
    assert (find != NULL);

    while (list && !find (list->data, user_data))
        list = list->next;

    return list;
}

void
slist_foreach (struct slist *list,
               int (*func) (void *data, void *user_data),
               void *user_data)
{
    assert (func != NULL);

    while (list)
    {
        func (list->data, user_data);
        list = list->next;
    }
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
slist_merge_sort (struct slist *list1,
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

    struct slist *list1 = list->next, *list2 = list;
    while ((list1 = list1->next) != NULL)
    {
        list1 = list1->next;
        if (list1 == NULL)
            break;
        list2 = list2->next;
    }
    list1 = list2->next;
    list2->next = NULL;

    return slist_merge_sort (slist_sort (list,  compare),
                             slist_sort (list1, compare),
                             compare);
}

