#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

typedef struct __node_t
{
	int size;
	struct __node_t* next;
} node_t;

typedef struct header_t
{
	int magic;
	int size;
} header_t;


int my_init();
void* my_malloc(int size);
void my_free(void* ptr);
void* my_calloc(int num, int size);
void* my_realloc(void *, int size);
void my_coalesce();
void my_showfreelist();
void my_uninit();
void my_uninit();



#define pageSize 1048576
#ifndef pageSize
#endif

node_t* myHeap;
node_t* head;
int totalSpace;
int fragmentation;