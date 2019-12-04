/*
 * iterator.h
 *
 *  Created on: 7 нояб. 2019 г.
 *      Author: M3M
 */

#ifndef _COLLECTION_ITERATOR_H_
#define _COLLECTION_ITERATOR_H_

#include <cic/collection.h>

struct iterator_str
{
	collection * col;
	void * meta;
	void (*toBegin) (iterator it);
	void (*toEnd) (iterator it);
	void (*onDel) (iterator it);
	void *(*prev) (iterator it);
	void *(*next) (iterator it);
	int (*hasNext) (iterator it);
	int (*hasPrev) (iterator it);
	int (*index) (iterator it);
	int buf;
};

int itindex(iterator);
void itdel(iterator);

iterator itreversed(iterator);
iterator itsteprange(int, int, int);
#define itxrange(x, y) itsteprange(x, y, 1)
#define itrange(x) itxrange(0, x)

#endif /* _COLLECTION_ITERATOR_H_ */
