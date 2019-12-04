/*
 * iteraition.h
 *
 *  Created on: 7 нояб. 2019 г.
 *      Author: M3M
 */

#ifndef _COLLECTION_ITERAITION_H_
#define _COLLECTION_ITERAITION_H_

#include <cic/iterator.h>

#define for_in(__collection, __x) do {\
	iterator __it = citerator(__collection);\
	int64_t __x;\
	while (__it->hasNext(__it)) {\
		__x = (int64_t) __it->next(__it);

#define for_in_reversed(__collection, __x) do {\
	iterator __it_b = citerator(__collection);\
	iterator __it = itreversed(__it_b);\
	itdel(__it_b);\
	int64_t __x;\
	while (__it->hasNext(__it)) {\
		__x = (int64_t) __it->next(__it);

#define for_for(___it, __x) do {\
	int64_t __x;\
	iterator __it = ___it;\
	while (__it->hasNext(__it)) {\
		__x = (int64_t) __it->next(__it);

#define for_x_in(__collection) for_in((void *) __collection, x)
#define for_x_in_reversed(__collection) for_in_reversed((void *) __collection, x)
#define for_x_for(__it) for_for(__it, x)

#define end ;} if (__it) itdel(__it); } while (0);

#endif /* _COLLECTION_ITERAITION_H_ */
