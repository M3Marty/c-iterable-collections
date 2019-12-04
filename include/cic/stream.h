/*
 * stream.h
 *
 *  Created on: 14 нояб. 2019 г.
 *      Author: M3M
 */

#ifndef _COLLECTION_STREAM_
#define _COLLECTION_STREAM_

#include <cic/collection.h>

typedef _Bool bool;

typedef void * (* csafunction)(void *);
typedef void * (* csabifunction)(void *, void *);
typedef bool (* csapredicate)(void *);
typedef void (* csaconsumer)(void *);

collection * csamap(collection * src, collection * to, csafunction, bool need_delete);
collection * csafilter(collection * src, collection * to, csapredicate, bool need_delete);

uint32_t csacount(collection * src, csapredicate, bool need_delete);
void csaforeach(collection * src, csaconsumer, bool need_delete);
void * csafindfirst(collection * src, csapredicate, bool need_delete);

bool csaanymatch(collection * src, csapredicate, bool need_delete);
bool csanonematch(collection * src, csapredicate, bool need_delete);
bool csaallmatch(collection * src, csapredicate, bool need_delete);

void * csamax(collection * src, csafunction, bool need_delete);
void * csamin(collection * src, csafunction, bool need_delete);
void * csareduce(collection * src, void * startValue, csabifunction, bool need_delete);

#define smap(src, f) csamap(src, 0, f, 1)
#define sfilter(src, f) csafilter(src, 0, f, 1)

#define scount(src, f) csacount(src, f, 1)
#define sforeach(src, f) csaforeach(src, f, 1)
#define sfindfirst(src, f) csafindfirst(src, f, 1)

#define sanymatch(src, f) csaanymatch(src, f, 1)
#define snonematch(src, f) csanonematch(src, f, 1)
#define sallmatch(src, f) csaallmatch(src, f, 1)

#define smax(src, f) csamax(src, f, 1)
#define smin(src, f) csamin(src, f, 1)
#define sreduce(src, startValue, f) csareduce(src, startValue, f, 1)

#endif /* _COLLECTION_STREAM_ */
