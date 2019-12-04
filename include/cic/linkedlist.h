/*
 * linkedlist.h
 *
 *  Created on: 7 нояб. 2019 г.
 *      Author: M3M
 */

#ifndef _COLLECTION_LINKEDLIST_H_
#define _COLLECTION_LINKEDLIST_H_

typedef struct linked_list_str linkedlist, llist;

llist * llnew();

uint32_t llsize(llist * col);
uint32_t lladd(llist * list, void * toAdd);
uint32_t llset(llist * list, void * toAdd, uint32_t id);
uint32_t llput(llist * list, void * toAdd, uint32_t id);
uint32_t llremove(llist * list, void * toDel);
uint32_t lldelete(llist * list, uint32_t toDel);
uint32_t llcontains(llist * list, void * search);
uint32_t llclear(llist * list);
uint32_t llsort(llist * list, int(*c)(const void *, const void *));
uint32_t llindex(llist * list, void * search);

iterator lliterator(llist * list);
void ** llasarray(llist * col, uint32_t * size);
void * llget(llist * col, uint32_t id);
void lldel(llist * col);

#endif /* _COLLECTION_LINKEDLIST_H_ */
