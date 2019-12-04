/*
 * arraylist.c
 *
 *  Created on: 7 нояб. 2019 г.
 *      Author: M3M
 */

#include <cic/arraylist.h>
#include <cic/collection.h>
#include <cic/iterator.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>


struct array_list_str
{
	uint32_t type;
	uint32_t size;
	size_t * array;
	uint32_t capacity;
};

void alittobegin(iterator it)
{
	it->buf = 0;
}
void alittoend(iterator it)
{
	it->buf = alsize((list *) it->col) - 1;
}
int alithasprev(iterator it)
{
	return it->buf > 0;
}
int alithasnext(iterator it)
{
	return it->buf < alsize((list *) it->col);
}
void * alitprev(iterator it)
{
	return alget((list *) it->col, it->buf--);
}
void * alitnext(iterator it)
{
	return alget((list *) it->col, it->buf++);
}
int alitindex(iterator it)
{
	return it->buf;
}

list * alnew()
{
	return alnewsized(10);
}

list * alnewsized(uint32_t init_size)
{
	list * l = malloc(sizeof(list));
	l->type = COL_ARRAY_LIST;
	l->size = 0;
	l->capacity = init_size;
	l->array = malloc(sizeof(size_t) * init_size);
	return l;
}

uint32_t alsize(list * l)
{
	return l->size;
}
uint32_t alset(list * l, void * toAdd, uint32_t id)
{
	if (l->size <= id) return 0;
	l->array[id] = (size_t) toAdd;
	return 1;
}
uint32_t alput(list * l, void * toAdd, uint32_t id)
{
	if (++l->size >= l->capacity) aladd(l, 0);
	for (uint32_t i = l->size; i > id; i--)
		l->array[i] = l->array[i - 1];
	l->array[id] = (size_t) toAdd;
	return 1;
}
uint32_t aladd(list * l, void * toAdd)
{
	if (l->capacity <= l->size)
	{
		l->capacity *= 2;
		l->array = realloc(l->array, l->capacity * sizeof(size_t));
	}

	l->array[l->size++] = (size_t) toAdd;
	return 1;
}
uint32_t aldelete(list * l, uint32_t toDel)
{
	if (l->size <= toDel) return 0;
	l->size--;
	for (uint32_t i = toDel; i < l->size; i++)
		l->array[i] = l->array[i + 1];
	return 1;
}
uint32_t alremove(list * l, void * toDel)
{
	return aldelete(l, alindex(l, toDel));
}
uint32_t alcontains(list * l, void * search)
{
	return alindex(l, search) >= 0;
}
uint32_t alclear(list * l)
{
	if (!l->size) return 0;
	memset(l->array, 0, l->size);
	l->size = 0;
	return 1;
}
uint32_t alsort(list * l, int(*c)(const void *, const void *))
{
	qsort(l->array, l->size, sizeof(size_t), c);
	return 1;
}
uint32_t alextend(list * l, uint32_t min_size)
{
	if (l->size >= min_size) return 0;
	while (l->size < min_size) aladd(l, 0);
	return 1;
}
int32_t alindex(list * l, void * search)
{
	size_t s = (size_t) search;
	for (uint32_t i = 0; i < l->size; i++)
		if (s == l->array[i]) return i;
	return -1;
}
uint32_t alcompact(list * l)
{
	l->array = realloc(l->array, l->size * sizeof(size_t));
	l->capacity = l->size;
	return 1;
}

iterator aliterator(list * l)
{
	if (!l) return 0;
	iterator it = malloc(sizeof(struct iterator_str));
	it->col = (collection *) l;
	it->hasNext = alithasnext;
	it->hasPrev = alithasprev;
	it->meta = 0;
	it->buf = 0;
	it->next = alitnext;
	it->prev = alitprev;
	it->toBegin = alittobegin;
	it->toEnd = alittoend;
	it->index = alitindex;
	return it;
}
void ** alasarray(list * l, uint32_t * size)
{
	if (size) *size = l->size;
	return (void **) l->array;
}
void * alget(list * l, uint32_t id)
{
	if (id > l->size) return 0;
	return (void *) l->array[id];
}
void aldel(list * l)
{
	free(l->array);
	free(l);
}

