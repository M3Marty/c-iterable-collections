/*
 * class.h
 *
 *  Created on: 4 дек. 2019 г.
 *      Author: M3M
 */

#ifndef _COLLECTION_CLASS_H_
#define _COLLECTION_CLASS_H_

struct collection_class_str
{
	collection * (*new)();

	uint32_t (*csize)(const collection * this);
	uint32_t (*cput)(const collection * this, void * toAdd);
	uint32_t (*cremove)(const collection * this, void * remove);
	uint32_t (*ccontains)(const collection * this, void * search);
	uint32_t (*cclear)(const collection * this);

	iterator (*citerator)(const collection * this);
	void ** (*casarray)(const collection * this, uint32_t * size);

	void cdel(collection * col);
};

#ifndef CIC_CLASS_DEFINED
#define CIC_CLASS_DEFINED
typedef struct collection_class_str cicCollectionClass;
#endif

uint32_t cloadcollection(const cicCollectionClass class);
void cdelcollection(uint32_t type);

#endif /* _COLLECTION_CLASS_H_ */
