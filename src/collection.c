/*
 * collection.c
 *
 *  Created on: 12 нояб. 2019 г.
 *      Author: M3M
 */

#include <cic/collection.h>
#include <cic/iteraition.h>

#include <cic/arraylist.h>
#include <cic/linkedlist.h>
#include <limits.h>


#define type(x) (* (uint32_t *) x)

static list * cicclasslist = 0;

void static inline ccheckstdcollection()
{
	if (cicclasslist) return;

	cicclasslist = alnewsized(4);

	cloadcollection((cicCollectionClass) {
			alnew,
			alsize, aladd, alremove, alcontains, alclear,
			aliterator, alasarray,
			aldel
		});
	cloadcollection((cicCollectionClass) {
			llnew,
			llsize, lladd, llremove, llcontains, llclear,
			lliterator, llasarray,
			lldel
		});
}

collection * cnew(uint32_t type)
{
	ccheckstdcollection();
	return (void * (*[])()) {
		(void * (*)()) alnew,
		(void * (*)()) llnew,
	} [type]();
}
collection * cof(uint32_t size, ...)
{
	ccheckstdcollection();
	uint64_t * elem = (uint64_t *) &size + 1;
	// Use array list as default
	list * l = alnewsized(size);
	while (size--)
	{
		aladd(l, (void *) *elem);
		elem++;
	}
	return (collection *) l;
}
collection * cgenerate(uint32_t size, void *(*gen)())
{
	ccheckstdcollection();
	// Use array list as default
	list * l = alnewsized(size);
	while (size--)
		aladd(l, gen());

	return (collection *) l;
}
collection * cclone(collection * src)
{
	collection * new = cnew(type(src));
	for_x_in (src)
		cput(new, (void *) x);
	end
	return new;
}

uint32_t csize(const collection * col)
{
	return (uint32_t (*[])()) {
		alsize,
		llsize,
	} [type(col)](col);
}
uint32_t cput(const collection * col, void * toAdd)
{
	return (uint32_t (*[])()) {
		aladd,
		lladd,
	} [type(col)](col, toAdd);
}
uint32_t cremove(const collection * col, void * remove)
{
	return (uint32_t (*[])())  {
		alremove,
		llremove,
	} [type(col)](col, remove);
}
uint32_t ccontains(const collection * col, void * search)
{
	return (uint32_t (*[])())  {
		alcontains,
		llcontains,
	} [type(col)](col, search);
}
uint32_t cclear(const collection * col)
{
	return (uint32_t (*[])()) {
		alclear,
		llclear,
	} [type(col)](col);
}

iterator citerator(const collection * col)
{
	return (iterator (*[])()) {
		aliterator,
		lliterator,
	} [type(col)](col);
}
void ** casarray(const collection * col, uint32_t * size)
{
	return (void **(*[])()) {
		alasarray,
		llasarray,
	} [type(col)](col, size);
}

void cdel(collection * col)
{
	return (void (*[])()) {
		aldel,
		lldel,
	} [type(col)](col);
}

#include <cic/stream.h>

collection * csamap(collection * src, collection * to, csafunction f, bool need_delete)
{
	if (!to) to = cnew(type(src));
	for_x_in(src)
		cput(to, f((void *) x))
	end
	if (need_delete)
		cdel(src);

	return to;
}
collection * csafilter(collection * src, collection * to, csapredicate f, bool need_delete)
{
	if (!to) to = cnew(type(src));
	for_x_in(src)
		if (f((void *) x))
			cput(to, (void *) x)
	end
	if (need_delete)
		cdel(src);

	return to;
}

uint32_t csacount(collection * src, csapredicate f, bool need_delete)
{
	uint32_t count = 0;
	for_x_in(src)
		if (f((void *) x))
			count++
	end
	if (need_delete)
		cdel(src);
	return count;
}

void csaforeach(collection * src, csaconsumer f, bool need_delete)
{
	for_x_in(src)
		f((void *) x)
	end
	if (need_delete)
		cdel(src);
}

void * csafindfirst(collection * src, csapredicate f, bool need_delete)
{
	void * res = 0;
	for_x_in(src)
		if (f((void *) x))
		{
			res = (void *) x;
			break;
		}
	end
	if (need_delete)
		cdel(src);
	return res;
}

bool csaanymatch(collection * src, csapredicate f, bool need_delete)
{
	return (size_t) csafindfirst(src, f, need_delete) != -1;
}
bool csanonematch(collection * src, csapredicate f, bool need_delete)
{
	return (size_t) csafindfirst(src, f, need_delete) == -1;
}
bool csaallmatch(collection * src, csapredicate f, bool need_delete)
{
	bool res = 1;
	for_x_in(src)
		if (!f((void *) x))
		{
			res = 0;
			break;
		}
	end
	if (need_delete)
		cdel(src);
	return res;
}

void * csamax(collection * src, csafunction f, bool need_delete)
{
	int64_t buf;
	int64_t max = INT64_MIN;
	void * res = 0;
	for_x_in(src)
		if (max < (buf = (int64_t) f((void *) x)))
		{
			res = (void *) x;
			max = buf;
		}
	end
	if (need_delete)
		cdel(src);
	return res;
}
void * csamin(collection * src, csafunction f, bool need_delete)
{
	int64_t buf;
	int64_t min = INT64_MAX;
	void * res = 0;
	for_x_in(src)
		if (min > (buf = (int64_t) f((void *) x)))
		{
			res = (void *) x;
			min = buf;
		}
	end
	if (need_delete)
		cdel(src);
	return res;
}
void * csareduce(collection * src, void * startValue, csabifunction f, bool need_delete)
{
	void * reduced = startValue;
	for_x_in(src)
		reduced = f(reduced, (void *) x)
	end
	return reduced;
}

#include <cic/class.h>

int ccmpcollections(cicCollectionClass * x, cicCollectionClass * y)
{
	return x->casarray - y->casarray	||
		x->cclear - y->cclear			||
		x->ccontains - y->ccontains 	||
		x->citerator - y->citerator 	||
		x->cput - y->cput				||
		x->cremove - y->cremove			||
		x->csize - y->csize				||
		x->new - y->new;
}

uint32_t cloadcollection(const cicCollectionClass class)
{
	uint32_t freeID = -1;

	for_x_in (cicclasslist)
		if (x == 0) freeID = itindex(__it);
		if (ccmpcollections(x, &class))
		{
			/* Use native __it */
			uint32_t index = itindex(__it);
			itdel(__it);
			return index;
		}
	end

	uint32_t index = freeID+1? alsize(cicclasslist): freeID;
	aladd(cicclasslist, class);
	return index;
}
void cdelcollection(uint32_t type)
{
	cicCollectionClass * x = alget(cicclasslist, type);
	free(x);
	alset(cicclasslist, 0, type);
}
