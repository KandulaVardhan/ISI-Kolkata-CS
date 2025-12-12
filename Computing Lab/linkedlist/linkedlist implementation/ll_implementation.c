#include "ll_alt.h"

void init_list(LIST *l, int n) {
    int i;

    if (NULL == (l->elements = Malloc(n, NODE)))  // Malloc is assumed to be defined elsewhere
        ERR_MESG("init_list: out of memory");
    
    l->head = l->tail = -1;
    l->free = l->length = 0;
    l->capacity = n;

    for (i = 1; i < n - 1; i++) {
        l->elements[i].prev = i - 1;
        l->elements[i].next = i + 1;
    }
    l->elements[0].prev = l->elements[n - 1].next = -1;
    l->elements[0].next = 1;
    l->elements[n - 1].prev = n - 2;
    return;
}

int insert(LIST *l, unsigned int index, DATA d) {
    int nptr1, nptr2 = -1, i;
    if (index < 0 || index > l->length)
        return -1;
    if (-1 == l->free) {
        /* array is full, need to realloc */
        l->capacity *= 2;
        if (NULL == (l->elements = Realloc(l->elements, l->capacity, NODE)))  // Realloc is assumed to be defined elsewhere
            ERR_MESG("insert: out of memory");
        l->free = l->capacity / 2;  /* added part (2nd half) is free */
        for (i = l->free; i < l->capacity; i++) {
            l->elements[i].prev = i - 1;
            l->elements[i].next = i + 1;
        }
        l->elements[l->free].prev = l->elements[l->capacity - 1].next = -1;
    }

    int position = l->free;  // d will be stored in first free node
    l->elements[position].data = d;
    l->free = l->elements[l->free].next;  // remove first free node to hold d

    for (i = 0, nptr1 = l->head; i != index; i++, nptr1 = l->elements[nptr1].next)
        nptr2 = nptr1;
    
    l->elements[position].prev = nptr2;
    l->elements[position].next = nptr1;

    if (nptr2 == -1)  // inserted at head
        l->head = position;
    else
        l->elements[nptr2].next = position;
    
    if (nptr1 == -1)  // inserted at tail
        l->tail = position;
    else
        l->elements[nptr1].prev = position;
    
    l->length++;
    return 1;
}

int Delete(LIST *l, unsigned int index, DATA *d) {
    if (index < 0 || index >= l->length)
        return -1;

    int deletednode, nptr1, nptr2 = -1, i;
    for (i = 0, nptr1 = l->head; i != index; i++, nptr1 = l->elements[nptr1].next)
        nptr2 = nptr1;
    assert(nptr1 != -1);

    if (nptr2 == -1) /* head was deleted */
        l->head = l->elements[nptr1].next;
    else
        l->elements[nptr2].next = l->elements[nptr1].next;
    if (l->elements[nptr1].next == -1) /* tail was deleted */
        l->tail = nptr2;
    else
        l->elements[l->elements[nptr1].next].prev = nptr2;
    l->length--;

    deletednode = nptr1;
    *d = l->elements[deletednode].data;
    l->elements[deletednode].prev = -1;
    l->elements[deletednode].next = l->free;
    l->free = deletednode;
    return 1;
}

int find_index_of(LIST *l, DATA d) {
    int nptr, i;
    for (i = 0, nptr = l->head; nptr != -1 && l->elements[nptr].data != d; i++, nptr = l->elements[nptr].next);
    if (nptr == -1)
        return -1;
    else /* nptr->data == d */
        return i;
}

int find_value_at(LIST *l, unsigned int index, DATA *d) {
    int nptr, i;
    if (index < 0 || index >= l->length)
        return -1;
    for (i = 0, nptr = l->head; i != index; i++, nptr = l->elements[nptr].next);
    *d = l->elements[nptr].data;
    return 1;
}

void print_list(LIST *l) {
    int nptr, i;
    for (i = 1, nptr = l->head; i <= l->length; i++, nptr = l->elements[nptr].next) {
        printf("%6d %6d\t", i-1, l->elements[nptr].data);
        if (i%5 == 0) putchar('\n');
    }
    if ((i-1)%5) putchar('\n');
    return;
}

void free_list(LIST *l) {
    free(l->elements);
    return;
}

void dump_table(LIST *l) {
    int i;
    printf("head: %d\ntail: %d\nfree: %d\nlength: %u\ncapacity: %u\n",
           l->head, l->tail, l->free, l->length, l->capacity);
    for (i = 0; i < l->capacity; i++)
        printf("%6d %6d %6d %6d\n", i, l->elements[i].data,
               l->elements[i].prev, l->elements[i].next);
    return;
}