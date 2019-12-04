/*
 * collection_test.c
 *
 *  Created on: 12 нояб. 2019 г.
 *      Author: M3M
 */

#define test_build
#ifdef test_build

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <cic/collection.h>
#include <cic/stream.h>
#include <cic/iteraition.h>

#include <cic/linkedlist.h>

static uint32_t count;
void test(uint32_t * success, uint32_t * failed, uint32_t * passed, uint32_t * ignored,
		int cond, char * message, int (*log)(const char * template, ...),
		int ignore, char * ignore_msg)
{

	if (success)
		count++;
	else
		count = 0;

	if (!ignore)
	{
		log("IGNORE  (%-3d): %s", count - 1, ignore_msg);
		(*ignored)++;
		return;
	}

	if (cond)
	{
		(*success)++;
		log("SUCCESS (%-3d): %s", count - 1, message);
	}
	else
	{
		(*failed)++;
		log("FAILED  (%-3d): %s", count - 1, message);
	}

	(*passed)++;
}

#define tst(cond, msg, log) tst_ign(cond, msg, log, 1, 0)
#define tst_ign(cond, msg, log, ign, ign_msg)  test(&success, &failed, &passed, &ignored, cond, msg, log, ign, ign_msg)
#define p2i (int) (long long int)
#define p2u (size_t) (long long int)
#define u2i (int)
#define i2p (void *)
#define p2c (collection *)
#define p2l (int64_t)
#define p2lp * (int64_t *)

#include <cic/arraylist.h>

int cmpgt(const void * x, const void * y)
{
	return p2lp x > p2lp y;
}

int cmplt(const void * x, const void * y)
{
	return p2lp x < p2lp y;
}

int32_t is_neg(int64_t x)
{
	return x < 0;
}

int32_t is_pos(int64_t x)
{
	return x >= 0;
}

int inline static testCollections(int (*log)(const char * template, ...))
{
	clock_t ck = clock();

	uint32_t success = 0;
	uint32_t failed = 0;
	uint32_t passed = 0;
	uint32_t ignored = 0;

	size_t buffer;
	char bool;

	log("<-------- Test ArrayList:\n");

	log("\n<(%d)----- Create new array list with cnew;\n", count);
	collection * col = cnew(COL_ARRAY_LIST);
	tst(p2i col, "collection pointer is ", log);
	log("%p", col);

	log("\n<(%d)----- Create new array list with alnewsized;\n", count);
	list * list = alnewsized(20);
	tst(p2i list, "list pointer is ", log);
	log("%p", list);

	log("\n<(%d)----- Append first list to second with cput;\n", count);
	tst_ign(u2i cput(p2c list, col), "", log, list && col, "List or collection is NULL");

	log("\n<(%d)----- Append first list to second with cput second time;\n", count);
	tst_ign(u2i cput(p2c list, col), "", log, list && col, "List or collection is NULL");

	log("\n<(%d)----- Get element 2 of list with alget;\n", count);
	tst_ign(!(buffer = p2u alget(list, 2)), "Content is ", log, list && col, "List or collection is NULL");
	log("0x%X", buffer);

	log("\n<(%d)----- Get element 1 of list with alget;\n", count);
	tst_ign((buffer = p2u alget(list, 1)) == p2u col, "Content is ", log, list && col, "List or collection is NULL");
	log("0x%X", buffer);

	log("\n<(%d)----- Delete first collection with cdel;\n", count);
	cdel(col);
	tst_ign(1, "", log, list && col, "List or collection is NULL");

	log("\n<(%d)----- Put element (123) at position 1 with alput;\n", count);
	tst_ign(alput(list, i2p 123, 1), "", log, p2i list, "List is NULL");

	log("\n<(%d)----- Print elements with for loop and get length with csize;\n", count);
	buffer = 1;
	for (uint32_t i = 0; i < csize(i2p list); i++) buffer = buffer && ((size_t[]) { p2i col, 123, p2i col } [i] == p2i alget(list, i));
	tst_ign(buffer, "Duplicate values:", log, p2i list, "List is NULL");
	for (uint32_t i = 0; i < csize(i2p list); i++)
		log (" 0x%X(%d)", alget(list, i), alget(list, i));

	log("\n<(%d)----- Compact list;\n", count);
	tst_ign(alcompact(list), "", log, p2i list, "List is NULL");

	log("\n<(%d)---- Clone list to collection;\n", count);
	tst_ign(p2i (col = cclone(i2p list)), "", log, p2i list, "List is NULL");

	log("\n<(%d)---- Extend collection to 15;\n", count);
	tst_ign(alextend(i2p col, 15), "", log, p2i col, "Collection is NULL");
	for_x_in(col)
		log (" 0x%X(%d)", p2i x, p2i  x);
	end

	log("\n<(%d)---- Sort collection and print with iterator;\n", count);
	tst_ign(alsort(i2p col, cmpgt), "Data: ", log, p2i col, "Collection is NULL");
	for_x_in(col)
		log (" 0x%X(%d)", p2i x, p2i  x);
	end

	log("\n<(%d)---- Search index of 123 in collection;\n", count);
	tst_ign((buffer = alindex(i2p col, i2p 123)) == 12, "At index", log, p2i col, "Collection is NULL");
	log("[%d] = %d", p2i buffer, buffer >= 0? p2i alget(i2p col, buffer): -1);

	log("\n<(%d)---- Iteration reversed iterator;\n", count);
	tst_ign(1, "Print:", log, p2i col, "Collection is NULL");
	for_x_in_reversed(col)
		log (" 0x%X(%d)", p2i x, p2i  x);
	end

	log("\n<(%d)---- Clear list;\n", count);
	tst_ign(cclear(i2p list), "Complete. Now Size: ", log, p2i list, "List is NULL");
	log("%d", alsize(list));

	log("\n\n\n<-------- High Load Test ArrayList:\n");
	log("\n<(%d)---- Fill list with values [-1000000;1000000] with f(x)=564x^3+143x^2-931x+289;\n", count);
	tst_ign(1, "Complete. Now Size: ", log, p2i list, "List is NULL");
	for_x_for(itxrange(-1000000, 1000001))
		cput(i2p list, i2p (x * x * x * 564 + x * x * 143 - x * 931 + 289))
	end
	log("%d", alsize(list));

	log("\n<(%d)---- Search index of 289 in collection;\n", count);
	tst_ign((buffer = alindex(i2p list, i2p 289)) == 1000000, "At index", log, p2i list, "List is NULL");
	log("[%d] = %d", p2i buffer, buffer >= 0? p2i alget(i2p list, buffer): -1);

	log("\n<(%d)---- Clone list to collection;\n", count);
	cdel(col);
	tst_ign(p2i (col = cclone(i2p list)), "Collection: ", log, p2i list, "List is NULL");
	log("%p", col);

	log("\n<(%d)---- Remove all negative values in %s with stream api;\n", count, "list");
	buffer = csacount(i2p list, i2p is_neg, 0);
	list = i2p sfilter(i2p list, i2p is_pos);
	bool = csanonematch(i2p list, is_neg, 0);
	tst_ign(!bool, "Remove ", log, p2i list, "List is NULL");
	log("%d negative numbers", p2i buffer);

	log("\n<(%d)---- Sort collection;\n", count);
	tst_ign(alsort(i2p col, cmplt), "Complete: ", log, p2i list, "List is NULL");
	log("col[0]=%d; col[%d]=%d;", alget(i2p col, 0), alsize(i2p col) - 1, alget(i2p col, alsize(i2p col) - 1));

	log("\n<(%d)---- Remove all negative values in %s with stream api;\n", count, "collection");
	buffer = csacount(i2p col, i2p is_neg, 0);
	col = i2p sfilter(i2p col, i2p is_pos);
	bool = 0;
	for_x_in(i2p list)
		if (x >= 0) continue;
		printf("Err: %I64d at %d\n", x, itindex(__it));
		bool = 1;
	end
	tst_ign(!bool, "Remove ", log, p2i col, "Collection is NULL");
	log("%d negative numbers", p2i buffer);

	cdel(i2p list);
	cdel(col);




	log("\n\n<-------- Test LinkedList:\n");

	log("\n<(%d)----- Create new linked list with cnew;\n", count);
	col = cnew(COL_LINKED_LIST);
	tst(p2i col, "collection pointer is ", log);
	log("%p", col);

	log("\n<(%d)----- Create new linked list with llnew;\n", count);
	llist * ll = llnew();
	tst(p2i ll, "list pointer is ", log);
	log("%p", ll);

	log("\n<(%d)----- Append first list to second with cput;\n", count);
	tst_ign(u2i cput(p2c ll, col), "", log, ll && col, "List or collection is NULL");

	log("\n<(%d)----- Append first list to second with cput second time;\n", count);
	tst_ign(u2i cput(p2c ll, col), "", log, ll && col, "List or collection is NULL");

	log("\n<(%d)----- Get element 2 of list with llget (expect 0x0);\n", count);
	tst_ign(!(buffer = p2u llget(ll, 2)), "Content is ", log, ll && col, "List or collection is NULL");
	log("0x%X", buffer);

	log("\n<(%d)----- Get element 1 of list with llget;\n", count);
	tst_ign((buffer = p2u llget(ll, 1)) == p2u col, "Content is ", log, ll && col, "List or collection is NULL");
	log("0x%X", buffer);

	log("\n<(%d)----- Delete first collection with cdel;\n", count);
	cdel(col);
	tst_ign(1, "", log, ll && col, "List or collection is NULL");

	log("\n<(%d)----- Put element (123) at position 1 with llput (size %d);\n", count, llsize(ll));
	tst_ign(llput(ll, i2p 123, 1), "", log, p2i ll, "List is NULL");

	log("\n<(%d)----- Print elements with for loop and get length with csize;\n", count);
	buffer = 1;
	for (uint32_t i = 0; i < csize(i2p ll); i++) buffer = buffer && ((size_t[]) { p2l col, 123, p2l col } [i] == p2i llget(ll, i));
	tst_ign(buffer, "Duplicate values:", log, p2i ll, "List is NULL");
	for (uint32_t i = 0; i < csize(i2p ll); i++)
		log (" 0x%X(%d)", llget(ll, i), llget(ll, i));

	log("\n<(%d)---- Clone list to collection;\n", count);
	tst_ign(p2i (col = cclone(i2p ll)), "", log, p2i ll, "List is NULL");

	log("\n<(%d)---- Sort collection and print with iterator;\n", count);
	tst_ign(llsort(i2p col, cmpgt), "Data: ", log, p2i col, "Collection is NULL");
	for_x_in(col)
		log (" 0x%X(%d)", p2i x, p2i  x);
	end

	log("\n<(%d)---- Search index of 123 in collection;\n", count);
	tst_ign((buffer = llindex(i2p col, i2p 123)) == 0, "At index", log, p2i col, "Collection is NULL");
	log("[%d] = %d", p2i buffer, buffer >= 0? p2i llget(i2p col, buffer): -1);

	log("\n<(%d)---- Iteration reversed iterator;\n", count);
	tst_ign(1, "Print:", log, p2i col, "Collection is NULL");
	for_x_in_reversed(col)
		log (" 0x%X(%d)", p2i x, p2i  x);
	end

	log("\n<(%d)---- Clear list;\n", count);
	tst_ign(cclear(i2p ll), "Complete. Now Size: ", log, p2i ll, "List is NULL");
	log("%d", llsize(ll));

	log("\n\nTest Finish (%.2fs):\nSUCCESS: %d\nFAILED:  %d\nPASSED:  %d\nIGNORED: %d\n", ((float)(clock() - ck) / 1000000.0F ) * 1000, success, failed, passed, ignored);
	return !(failed || ignored);
}

int main(void)
{
	testCollections(printf);
	system("pause");
}

#endif
