/*
 * list.h
 *
 *  Created on: 7 нояб. 2019 г.
 *      Author: M3M
 */

#ifndef _COLLECTION_H_
#define _COLLECTION_H_

#include <inttypes.h>

#define COL_ARRAY_LIST	0x00
#define COL_LINKED_LIST	0x01
#define COL_MAP			0x02
#define COL_SET			0x03
#define COL_RANGE		0x04

typedef struct collection_prototype { uint32_t type; } collection;
typedef struct iterator_str * iterator;

#ifndef CIC_CLASS_DEFINED
#define CIC_CLASS_DEFINED
typedef struct collection_class_str cicCollectionClass;
#endif

collection * cnew(uint32_t type);
collection * cclone(collection * src);
collection * cof(uint32_t size, ...);

uint32_t csize(const collection * col);
uint32_t cput(const collection * col, void * toAdd);
uint32_t cremove(const collection * col, void * remove);
uint32_t ccontains(const collection * col, void * search);
uint32_t cclear(const collection * col);

iterator citerator(const collection * col);
void ** casarray(const collection * col, uint32_t * size);

void cdel(collection * col);

#endif /* _COLLECTION_H_ */
