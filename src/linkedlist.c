/*
 * linkedlist.c
 *
 *  Created on: 14 нояб. 2019 г.
 *      Author: M3M
 */

#include <cic/arraylist.h>
#include <cic/collection.h>
#include <cic/iteraition.h>
#include <cic/iterator.h>
#include <cic/linkedlist.h>
#include <stdlib.h>
#include <inttypes.h>


typedef struct linked_list_node
{
	size_t val;
	struct linked_list_node * next;
	struct linked_list_node * prev;
} node;

struct linked_list_str
{
	uint32_t type;
	uint32_t size, last_used_id;
	node * first;
	node * last;
	node * last_used;
};

int llithasprev(iterator it)
{
	char bool = (char) (int) (size_t) ((node *) it->meta)->prev || it->buf < 0;
	it->buf = 0;
	return bool;
}
int llithasnext(iterator it)
{
	char bool = (char) (int) (size_t) ((node *) it->meta)->next || it->buf > 0;
	it->buf = 0;
	return bool;
}
void * llitnext(iterator it)
{
	node * current = it->meta;
	it->buf = 1;
	if (((node *) it->meta)->next)
	{
		it->meta = current = current->next;
		return (void *) current->prev->val;
	}
	else
	{
		it->buf = 0;
		return (void *) current->val;
	}
}
void * llitprev(iterator it)
{
	node * current = it->meta;
	it->buf = -1;
	if (((node *) it->meta)->prev)
	{
		it->meta = current = current->prev;
		return (void *) current->next->val;
	}
	else
	{
		it->buf = 0;
		return (void *) current->val;
	}
}
void llittobegin(iterator it)
{
	it->meta = ((llist *) it->col)->first;
}
void llittoend(iterator it)
{
	it->meta = ((llist *) it->col)->last;
}

node * prev_node(node * n)
{
	return n->prev;
}
node * next_node(node * n)
{
	return n->next;
}

typedef struct node_distance_str
{
	node * node;
	uint32_t count;
	node *(*next_node)(node *);
} ndist;
node * getListNode(llist * l, uint32_t i)
{
	if (i >= l->size) return 0;
	ndist d[3] = {
			{ l->first, i, next_node },
			{ l->last, l->size - i, prev_node },
			{ l->last_used, abs(l->last_used_id - i), l->last_used_id - i > 0? next_node: prev_node }
	};

	if (d[2].count < 0)
		d[2].count = -d[2].count;

	ndist m = d[0];
	for (int i = 1; i < 3; i++)
		if (m.count > d[i].count)
			m = d[i];

	for (int i = 0; i < m.count; i++)
		m.node = m.next_node(m.node);

	if (i != 0 && i != l->size - 1)
	{
		l->last_used = m.node;
		l->last_used_id = i;
	}

	return m.node;
}

//@Unused
node * sortedMerge(node * a, node * b, int(*c)(const void *, const void *))
{
	node * r;

	if (!a)
		return (b);
	if (!b)
		return (a);

	if (c((void *) a->val, (void *) b->val))
		(r = a)->next = sortedMerge(a->next, b, c);
	else
		(r = b)->next = sortedMerge(a, b->next, c);
	return r;
}

//@Unused
void frontBackSplit(node * source, node ** frontRef, node ** backRef)
{
    node * fast;
    node * slow;
    slow = source;
    fast = source->next;

    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *frontRef = source;
    *backRef = slow->next;
    slow->next = 0;
}

//@Unused
void llmergesort(node ** headRef, int(*c)(const void *, const void *))
{
	node * head = *headRef;
	node * a, * b;

	if (!head || !head->next)
		return;

	frontBackSplit(head, &a, &b);

	llmergesort(&a, c);
	llmergesort(&b, c);

	*headRef = sortedMerge(a, b, c);
}

//@RequiredOptimize
void llintermediarysort(llist * l, int(*c)(const void *, const void *))
{
	list * lInter = alnewsized(l->size);
	size_t register * array = ((size_t *) lInter) + 1; // Using hack with align in structure
	for_x_in (l)
		*array = x;
		array++
	end

	alsort(lInter, c);
	node * n = l->first;

	for_x_in (lInter)
		n->val = x;
		n = n->next
	end
	aldel(lInter);
}

llist * llnew()
{
	llist * l = calloc(1, sizeof(llist));
	l->type = COL_LINKED_LIST;
	return l;
}

uint32_t llsize(llist * col)
{
	return col->size;
}
uint32_t lladd(llist * list, void * toAdd)
{
	if (list->size)
	{
		node * new = calloc(1, sizeof(node));
		new->prev = list->last;
		list->last = list->last->next = new;
		new->val = (size_t) toAdd;
	}
	else
	{
		list->last = list->first = list->last_used = calloc(1, sizeof(node));
		list->first->val = (size_t) toAdd;
	}
	list->size++;

	return 1;
}
uint32_t llset(llist * list, void * toAdd, uint32_t id)
{
	if (id >= list->size) return 0;
	getListNode(list, id)->val = (size_t) toAdd;
	return 1;
}
uint32_t llput(llist * list, void * toAdd, uint32_t id)
{
	if (id >= list->size) return 0;
	node * shifted = getListNode(list, id);
	node * new = calloc(1, sizeof(node));
	new->val = (size_t) toAdd;
	shifted->prev->next = new;
	new->prev = shifted->prev;
	shifted->prev = new;
	new->next = shifted;
	list->size++;

	return 1;
}
uint32_t lldelete(llist * list, uint32_t toDel)
{
	if (toDel >= list->size) return 0;
	node * shifted = getListNode(list, toDel);

	node * prev = shifted->prev;
	node * next = shifted->next;

	prev->next = next;
	next->prev = prev;

	list->size--;

	free(shifted);

	return 1;
}
uint32_t llremove(llist * list, void * toDel)
{
	return lldelete(list, llindex(list, toDel));
}
uint32_t llcontains(llist * list, void * search)
{
	return llindex(list, search) >= 0;
}
uint32_t llclear(llist * list)
{
	if (!list->size) return 0;
	node * n = list->first;
	while(n != list->last)
	{
		n = n->next;
		free(n->prev);
	}
	free(n);
	list->size = list->last_used_id = 0;
	list->first = list->last = list->last_used = 0;
	return 1;
}
uint32_t llsort(llist * list, int(*c)(const void *, const void *))
{
	// llmergesort(&list->first, c);
	llintermediarysort(list, c);
	return 1;
}
uint32_t llindex(llist * list, void * search)
{
	uint32_t id = 0;
	for_x_in(list)
		if (search == (void *) x)
		{
			itdel(__it);	// Use native __it variable of for-each expression
			return id;
		}
		id++
	end
	return -1;
}

iterator lliterator(llist * list)
{
	iterator it = malloc(sizeof(struct iterator_str));
	it->col = (collection *) list;
	it->hasNext = llithasnext;
	it->hasPrev = llithasprev;
	it->index = 0;
	it->meta = list->first;
	it->buf = 0;
	it->next = llitnext;
	it->prev = llitprev;
	it->toBegin = llittobegin;
	it->toEnd = llittoend;
	return it;
}
void ** llasarray(llist * col, uint32_t * size)
{
	list * l = alnewsized(col->size);
	for_x_in((collection *) col)
		aladd(l, (void *) x)
	end

	void ** arr = alasarray(l, size);
	free(l);
	return arr;
}
void * llget(llist * col, uint32_t id)
{
	node * n = getListNode(col, id);
	if (!n) return 0;
	return (void *) n->val;
}
void lldel(llist * col)
{
	llclear(col);
	free(col);
}
