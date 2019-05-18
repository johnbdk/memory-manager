#ifndef STREAMFLOW_H
#define STREAMFLOW_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include "queue.h"

#define OBJECT_CLASS 9
#define PAGE sysconf(_SC_PAGE_SIZE)
#define PAGEBLOCK_SIZE PAGE*8
#define CACHE_LINE sysconf(_SC_LEVEL1_DCACHE_LINESIZE)
#define max(x, y) (((x) > (y)) ? (x) : (y))

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

struct object_class {
	struct pageblock *active_head;
	struct pageblock *active_tail;
};

struct local_heap {
	struct object_class obj[OBJECT_CLASS];
};

struct pageblock {
	int id;
	unsigned pageblock_size;
	unsigned object_size;
	unsigned num_free_objects;
	void *freed_list;				// start of free list
	void *unallocated;				// points to the next unallocated space within a pageblock
	void *remotely_freed_list;
	struct pageblock *next;
	struct pageblock *prev;
	struct local_heap *heap;
};

extern queue_t *table[9];

typedef struct object_class object_class_t;
typedef struct local_heap local_heap_t;
typedef struct pageblock pageblock_t;

int my_free(void *address);
int get_object_class(size_t obj_size);
int object_class_exists(size_t obj_size);
void *my_malloc(size_t size);
void allocate_memory(size_t obj_size);

#endif
