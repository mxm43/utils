#ifndef UTILS_SINGLE_LINKED_LIST_H_
#define UTILS_SINGLE_LINKED_LIST_H_

struct slist
{
    void         *data;
    struct slist *next;
};

struct slist *
slist_insert        (struct slist *slist, void *data);

struct slist *
slist_find          (struct slist *slist, void *data);

struct slist *
slist_foreach       (struct slist *slist,
                     int (*func)      (void *data, void *user_data),
                     void *user_data);

struct slist *
slist_remove_if     (struct slist *slist,
                     int (*will_rm)   (void *data, void *user_data),
                     int (*free_data) (void *data, void *user_data),
                     void * user_data);

struct slist *
slist_sort          (struct slist *list,
                     int (*compare) (void *data1, void *data2));

void
slist_cleanup       (struct slist *slist,
                     int (*free_data) (void *data, void *user_data),
                     void * user_data);

#endif /* UTILS_SINGLE_LIST_H_ */
