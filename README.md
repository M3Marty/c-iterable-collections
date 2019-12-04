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

This code uses new element of **cic** - [iteration.h](https://github.com/M3Marty/c-iterable-collections/include/iteration.h).<br>
That header defines new macroses what imitate `for-each` loops.
```C
#define for_in(__collection, __x)
#define for_in_reversed(__collection, __x)
#define for_for(___it, __x)

#define for_x_in(__collection)
#define for_x_in_reversed(__collection)
#define for_x_for(__it)
```
In turn, they use [iterators](https://github.com/M3Marty/c-iterable-collections/include/iterator.h).<br>

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
	int buf;
};
```
## Extends `cclone`
Besides `cclone` `cic` provides other default function for working with any collection provides standard API.
### Stream API
Stream API is a **Java Stream API**-inspired part of `cic` and provides methods to handle any collection what is registered to `cic`.

| Declaration | Desctiption |
|-|-|
|`csamap(collection * src, collection * to, csafunction f, bool nd)`|Call for each element of `src` and put element to `to`, if `to` is null then creates new collection with type of `src`. If `nd` then delete `src` when finish|
|`csafilter(collection * src, collection * to, csapredicate f, bool nd)`|Call for each element of `src` and if `f` return non null value put to `to`, if `to` is null then creates new collection with type of `src`. If `nd` then delete `src` when finish|
|`csacount(collection * src, csapredicate f, bool nd)`|Call for each element of `src` and if `f` return non null value in count, and return when finish. If `nd` then delete `src` when finish|
