/*
 * iterator.c
 *
 *  Created on: 7 нояб. 2019 г.
 *      Author: M3M
 */

#include <cic/iterator.h>
#include <stdlib.h>

#define rg_from i[0]
#define rg_to i[1]
#define rg_step i[2]
#define rg_now i[3]

void itdel(iterator it)
{
	if (it->meta) free(it->meta);
	free(it);
}

iterator itreversed(iterator it)
{
	if (!(it->toEnd && it->prev && it->hasPrev)) return 0;
	iterator new = citerator(it->col);
	new->next = it->prev;
	new->prev = it->next;
	new->toBegin = it->toEnd;
	new->toEnd = it->toBegin;
	new->toBegin(new);
	new->hasNext = it->hasPrev;
	new->hasPrev = it->hasNext;
	return new;
}

int itindex(iterator it)
{
	return it->index? it->index(it): -1;
}

void rgittobegin(iterator it)
{
	int32_t * i = it->meta;
	rg_now = rg_from;
}
void rgittoend(iterator it)
{
	int32_t * i = it->meta;
	rg_now = rg_to;
}
int rgithasprev(iterator it)
{
	int32_t * i = it->meta;
	return rg_now - rg_step >= rg_from;
}
int rgithasnext(iterator it)
{
	int32_t * i = it->meta;
	return rg_now + rg_step < rg_to;
}
void * rgitprev(iterator it)
{
	int32_t * i = it->meta;
	return (void *) (size_t) ((rg_now -= rg_step) + rg_step);
}
void * rgitnext(iterator it)
{
	int32_t * i = it->meta;
	return (void *) (size_t) ((rg_now += rg_step) - rg_step);
}
int rgitindex(iterator it)
{
	int32_t * i = it->meta;
	return rg_now - rg_from;
}

iterator itsteprange(int f, int t, int s)
{
	if (f > t) s = -s;
	iterator it = malloc(sizeof(struct iterator_str));
	it->col = 0;
	it->hasNext = rgithasnext;
	it->hasPrev = rgithasprev;
	it->next = rgitnext;
	it->prev = rgitprev;
	it->toBegin = rgittobegin;
	it->toEnd = rgittoend;
	it->index = rgitindex;

	it->meta = malloc(sizeof(int) * 4);
	int * i = it->meta;
	rg_from = f;
	rg_to = t;
	rg_step = s;
	rg_now = f;
	return it;
}
