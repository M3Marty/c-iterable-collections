/*
 * arraylist.h
 *
 *  Created on: 7 нояб. 2019 г.
 *      Author: M3M
 */

#ifndef _COLLECTION_ARRAYLIST_H_
#define _COLLECTION_ARRAYLIST_H_

#include <cic/collection.h>

typedef struct array_list_str array_list, list;

list * alnew();
list * alnewsized(uint32_t init_size);

uint32_t alsize(list * col);
uint32_t alset(list * col, void * toAdd, uint32_t id);
uint32_t alput(list * col, void * toAdd, uint32_t id);
uint32_t aladd(list * col, void * toAdd);
uint32_t aldelete(list * col, uint32_t toDel);
uint32_t alremove(list * col, void * toDel);
uint32_t alcontains(list * col, void * search);
uint32_t alclear(list * col);
uint32_t alsort(list * l, int(*c)(const void *, const void *));
uint32_t alextend(list * col, uint32_t min_size);
int32_t alindex(list * col, void * search);
uint32_t alcompact(list * col);

iterator aliterator(list * col);
void ** alasarray(list * col, uint32_t * size);
void * alget(list * col, uint32_t id);
void aldel(list * col);

#endif /* _COLLECTION_ARRAYLIST_H_ */
