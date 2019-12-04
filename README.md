# C Iterable Collection
**C iterable collection** is library provides set of collections with single extendable API.
Also collection can has self API to specific methods.

### General structure of collection
First field of collection is **ID** that means a type of collection and used by standart API to identify and select with dynamic dispathing specific function.
```C
struct specific_collection
{
  uint32_t type;
  int8_t meta[];
};
```

For get type in [collection.c](https://github.com/M3Marty/src/c-iterable-collections/collection.c) defined macros `type`
that just cast collection pointer to uint32_t pointer and dereferencing one.
```C
#define type(x) (* (uint32_t *) x)
```

## Standard API

Standart API including not much long list of<br>function what must be implemented with developer added new collection

| Declaration | Desctiption |
|-|-|
| `cnew(uint32_t type)` | Create new collection of type `type` and return reference |
| `cdel(collection * col)` | Delete collection |
| `csize(collection * col)` | Return size of `col` |
| `ccontains(collection * col, void * x)` | If collection contains element `x` return non null value |
| `cput(collection * col, void * x)` | Put element to collection |
| `cremove(collection * col, void * x)` | Remove element from collection |
| `casarray(collection * col)` | Return array represents content of collection (this array can used inside of collection check docs to know) |
| `citerator(collection * col)` | Return iterator for collection |

Also `collection.c` provides function `cclone` but it's have default implementaition and mustn't implemented.
```C
collection * cclone(collection * src)
{
  collection * new = cnew(type(src));
  for_x_in (src)
    cput(new, (void *) x);
  end
  return new;
}

```

This code uses new element of **cic** - [iteration.h](https://github.com/M3Marty/c-iterable-collections/include/cic/iteration.h).<br>
That header defines new macroses what imitate `for-each` loops.
```C
#define for_in(__collection, __x)
#define for_in_reversed(__collection, __x)
#define for_for(___it, __x)

#define for_x_in(__collection)
#define for_x_in_reversed(__collection)
#define for_x_for(__it)
```
In turn, they use [iterators](https://github.com/M3Marty/c-iterable-collections/include/cic/iterator.h).<br>

> Also exists `cof` and `cgenerate` that creates new (default is array list) collection.
> First requires: count of elements and _rest_, second: count of elements and generator-function.

### Iterators
Iterator is public structure what contains references to functions and collection. Only collection can create iterator and it's get rights to block writing to collection while iterator used. Iterator also has function `onDel(it)` what called by `itdel` before delete iterator. In spite of this blocking collection is not sounds like good idea (only if not you are creating multy-threading collection). Also `iterator.h` provides function `itsteprange` that creates iterator for nums in range with step and function `itreversed` that creates new iterator with reversed flow of elements (if collection supports negate functions by default or iterator provides `reverseThis`)

For function `itsteprange` also exists two macroses `itrange` [0; x) and `itxrange` [x; y).

#### Structure of iterator
```C
struct iterator_str
{
	collection * col;
	void * meta;
	void (*toBegin) (iterator it);
	void (*toEnd) (iterator it);
	void *(*prev) (iterator it);
	void *(*next) (iterator it);
	int (*hasNext) (iterator it);
	int (*hasPrev) (iterator it);
	int (*index) (iterator it);
	int (*onDel) (iterator it);
	int buf;
};
```
## Extends `cclone`
Besides `cclone` **cic** provides other default function for working with any collection provides standard API.
### [Stream API](https://github.com/M3Marty/c-iterable-collections/include/cic/stream.h)
Stream API is a **Java Stream API**-inspired part of **cic** and provides methods to handle any collection what is registered to **cic**.

| Declaration | Desctiption |
|-|-|
|`csamap(collection * src, collection * to, csafunction f, bool nd)`|Call for each element of `src` and put returned value to `to`, if `to` is null then creates new collection with type of `src`. If `nd` then delete `src` when finished|
|`csafilter(collection * src, collection * to, csapredicate f, bool nd)`|Call for each element of `src` and if `f` returns non null value put to `to`, if `to` is null then creates new collection with type of `src`. If `nd` then delete `src` when finished|
|`csacount(collection * src, csapredicate f, bool nd)`|Call for each element of `src` and if `f` returns non null value in count, and return when finish. If `nd` then delete `src` when finished|
|`csaforeach(collection * src, csaconsumer f, bool nd)`|Call for each element of `src`. If `nd` then delete `src` when finished|
|`csafindfirst(collection * src, csapredicate f, bool nd)`|Call for each element of `src` while `f` not returns non null value and return last. If `nd` then delete `src` when finished|
|`csaanymatch(collection * src, csapredicate f, bool nd)`|Return non null value if one of elements for `f` returns non null value. If `nd` then delete `src` when finished|
|`csanonematch(collection * src, csapredicate f, bool nd)`|Return non null value if for each element for `f` returns null value. If `nd` then delete `src` when finished|
|`csaallmatch(collection * src, csapredicate f, bool nd)`|Return non null value if for each element for `f` returns non null value. If `nd` then delete `src` when finished|
|`csamax(collection * src, csafunction f, bool nd)`|Return element for one `f` returns max value. If `nd` then delete `src` when finished|
|`csamin(collection * src, csafunction f, bool nd)`|Return element for one `f` returns min value. If `nd` then delete `src` when finished|
|`csareduce(collection * src, void * v, csabifunction f, bool nd)`|Call for element `f` and puts as first argument accumulator (start value is `v`) and as second element of `src`. Returned value is new value of accumulator. Returns last state of accumulator|

Each function of Stream API has variant of conveyer. That one creates new collection (if returns collection) and delete previous collection.
That functions in truth macroses and returns view as `s*` for function `csa*`.
For example conveyer variant of `csamap` is `smap`.

## Example of use
For example we are wants to read `n` numbers and compute how much dividing by three and sum this.
```C

/* Also including "inttypes.h" */
#include <collection/collection.h>

/* Also including "collection/iterator.h" */
#include <collection/iteration.h>

/* Default input/output */
#include <stdio.h>

/* Define prototypes */
int isDivFor3(long x);
long sum(long x, long y);
long readInt();

int main(void)
{
  uint32_t n;
  scanf("%u", &n);
  
  /* Create with generator new collection and filter one with filter 'isDivFor3' */
  collection * x = sfilter(ccgenerate(n, readInt), isDivFor3);
  
  /* Get count of element in 'x' */
  uint32_t count = csize(x);
  
  /* Compute sum with "iteration.h" */
  
  long itsum = 0;
  
  /* x in this loop is not x in iuter block.
     Iteration header uses do {} while blocks
     for create new scope.
     That's why need to use 'end' */
  for_x_in (x)
    itsum += x;
  end
  
  /* Compute sum of elements in 'x' with start value '0' and functoin 'sum' */
  long sum = sreduce(x, 0, sum);
  
  /* Print results */
  printf("Div for 3 %u/%u\nSum of this is %d-%d (%s)", count, n
      itsum, sum, itsum-sum? "false": "true");
      
  return 0;
}

int isDivFor3(long x)
{
  return !(x % 3);
}

int sum(long x, long y)
{
  return prev + x;
}

long readInt()
{
  long n;
  fscanf("%I64d", &n);
  return n;
}

```

## Default collections
By default accessed [ArrayList](https://github.com/M3Marty/c-iterable-collections/include/cic/arraylist.h), [LinkedList](https://github.com/M3Marty/c-iterable-collections/include/cic/linkedlist.h), [HashSet](https://github.com/M3Marty/c-iterable-collections/include/cic/set.h) and [HashMap](https://github.com/M3Marty/c-iterable-collections/include/cic/map.h).
Collections has name prefix. `al` for ArrayList, `ll` for LinkedList, `set` for HashSet, `hmp` for HashMap.
That collection have self set of functions what not included to standatd API.

|Prefix|Declaration|Description|
|-|-|-|
|al|`alnewsized(uint32_t init_size)`|Create new ArrayList with start size (default 10)|
||`alset(list * col, void * toAdd, uint32_t id)`|Set value to element by index|
||`alput(list * col, void * toAdd, uint32_t id)`|Insert value by index (std API cputs implemented with aladd)|
||`aldelete(list * col, uint32_t toDel)`|Remove element by ID|
||`alsort(list * l, int(*c)(const void *, const void *))`|Sort array with `qsort`|
||`alextend(list * col, uint32_t min_size)`|Extends array list to `min_size`|
||`alindex(list * col, void * search)`|Return index of `search`|
||`alcompact(list * col)`|Set capacity to current size|
||`alget(list * col, uint32_t id)`|Get valued by index|
|ll|`llput(llist * col, void * toAdd, uint32_t id)`|Insert value by index (std API cputs implemented with lladd)|
||`lldelete(llist * col, uint32_t toDel)`|Remove element by ID|
||`llsort(llist * l, int(*c)(const void *, const void *))`|Sort array with `qsort` (transfer to simple array, sort and sets values)|
||`llindex(list * col, void * search)`|Return index of `search`|
||`llget(list * col, uint32_t id)`|Get valued by index|

## Creating new collections
For store collection function used `cicCollectionClass`
```C
typedef struct collection_class_str
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
} cicCollectionClass;
```
To manage classes in `collection.c` exists functions
```C
uint32_t cloadcollection(const cicCollectionClass class);
void cdelcollection(uint32_t type);
```
`cloadcollection` compares new `class` with other loaded classes and if class not loaded return index of class.
Recomended to define in `*new` function of collection make check by index is not zero and load class to static variable inside this fucntion if it's required.

`cdelcollection` delete collection by index.

## Iterator implementaition
How you can remember iterator is public structure.
Iterator must implements not all functions. Required only `hasNext` and `next`.
Other are extensions.
Iterator API used by **iteration.h**. For example impl of `for_in`, `for_in_reversed` and `end`:
```C
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
		
		
#define end ;} if (__it) itdel(__it); } while (0);
```

Implementaition of iterator requires using clear functions. For this two fields are provided in iterator structure:
`int buf` and `void * meta`. For example watch [ArrayList iterator](https://github.com/M3Marty/c-iterable-collections/src/arraylist.c). This iterator have not handlers `onDel` and `reversed`. That's cause iterator not blocking collection and default implementaition of `itreversed` works for it.

## In end
That's all what you need to use and contribute **cic**.
