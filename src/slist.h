#ifndef UTILS_SINGLE_LINKED_LIST_H_
#define UTILS_SINGLE_LINKED_LIST_H_

struct slist
{
    void         *data;
    struct slist *next;
};

struct slist *
slist_insert        (struct slist *list, void *data);

struct slist *
slist_sort_insert   (struct slist *list,
                     int (*compare)   (void *data1, void *data2),
                     void *data);

struct slist *
slist_append        (struct slist *list1, struct slist *list2);

struct slist *
slist_find          (struct slist *list,
                     int (*find)      (void *data, void *user_data),
                     void *data);

struct slist *
slist_remove_if     (struct slist *list,
                     int (*will_rm)   (void *data, void *user_data),
                     int (*free_data) (void *data, void *user_data),
                     void * user_data);

struct slist *
slist_sort          (struct slist *list,
                     int (*compare)   (void *data1, void *data2));

void
slist_foreach       (struct slist *list,
                     int (*func)      (void *data, void *user_data),
                     void *user_data);

void
slist_cleanup       (struct slist *list,
                     int (*free_data) (void *data, void *user_data),
                     void * user_data);

#endif /* UTILS_SINGLE_LIST_H_ */
