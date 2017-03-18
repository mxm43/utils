#ifndef SINGLE_LIST_H_
#define SINGLE_LIST_H_

typedef struct slist_t_
{
    void            * data;
    struct slist_t_ * next;
} slist_t;

typedef int (* func) (void * data, void * user_data);

slist_t *
slist_insert      (slist_t * slist, void * data);

slist_t *
slist_find        (slist_t * slist, void * data);

slist_t *
slist_foreach     (slist_t * slist, func f, void * user_data);

slist_t *
slist_remove_if   (slist_t * slist, func f, void * data, void * user_data);

void
slist_cleanup     (slist_t * slist, func f, void * user_data);

#endif
