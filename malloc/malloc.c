#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include "Lec2_st03856_A3_malloc.h"

int my_init()
{
	myHeap = mmap((void*) (pageSize * (1 << 10)), pageSize, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1 , 0);

	if (myHeap == MAP_FAILED)
	{
		printf("Oops! There was an error. mmap() failed.\n");
    	printf("Error: %s\n", strerror(errno));
    	return 0;
	}

	myHeap->size = pageSize - sizeof(node_t);
	myHeap->next = NULL;
	head = myHeap;
	fragmentation = 0;
	totalSpace = myHeap->size;
	return 1;
}

void* my_malloc(int size)
{
	if (myHeap == NULL)
	{
		printf("Memory must be allocated before being deallocated \n");
		exit(0);
	}

	//printf("\n Malloc Begins \n");
	if (size + sizeof(header_t) >= pageSize - sizeof(node_t) || size + sizeof(header_t) > totalSpace)
	{
		printf("The size you've given is too large to be allocated. Sorry!\n");
		exit(0);
	}

	// Checking free space at the head
	if (head->size >= size + sizeof(header_t))
	{
		header_t* temp = (header_t*)head;
		int oldHeadSize = head->size;

		head = (node_t*)((char*)head + size + sizeof(header_t));
		totalSpace = totalSpace - size - sizeof(header_t);
		head->size = oldHeadSize - size - sizeof(header_t);
		head->next = NULL;

		temp->size = size;
		temp->magic = 729512;
		return temp;
	}

	//If not found at head, it will traverse the free space linked list
	node_t* prevNode = head;
	node_t* currentNode = head->next;

	while (prevNode->size  < size + sizeof(header_t) && currentNode != NULL)
	{
		if (currentNode->next == NULL)
		{
			// if the last node in the free list has enough space
			if (currentNode->size > size + sizeof(header_t))
			{
				header_t* temp = (header_t*)currentNode;
				int oldCurrentSize = currentNode->size;

				if (oldCurrentSize - size - sizeof(header_t) == 0) 
				{
					prevNode->next = NULL;
				}
				else
				{
					currentNode = (node_t*)((char*)currentNode + size + sizeof(header_t));
					currentNode->size = oldCurrentSize - size - sizeof(header_t);
					currentNode->next = NULL;
					totalSpace = totalSpace - size - sizeof(header_t);
				}
				temp->size = size;
				temp->magic = 729512;
				return temp;
			}
			else // No space found in the entire list
			{
				printf("Not enough memory available. Please try a different amount.");
				exit(0);
			}
		}
		// if it hasn't reached the last node yet, it will keep traversing
		if (currentNode->size < size + sizeof(header_t))
		{
			prevNode = currentNode;
			currentNode = currentNode->next;
		}
	}
	// at this point, it will only exit the loop if its found a node in the middle with
	// an appropriate size
	
	header_t* temp = (header_t*)currentNode; 
	int oldCurrentSize = currentNode->size;
	// if the entire node is being used up
	if (oldCurrentSize - size - sizeof(header_t) == 0)
	{
		// connect the previous node to the current's next
		prevNode->next = currentNode->next;		
	} 
	else
	{
		currentNode = (node_t*)((char*)currentNode + size + sizeof(header_t));
		currentNode->size = oldCurrentSize - size - sizeof(header_t);
		currentNode->next = NULL;
		totalSpace = totalSpace - size - sizeof(header_t);
	}

	temp->size = size;
	temp->magic = 729512;

	return temp;

}

void my_free(void* ptr)
{
	if (myHeap == NULL)
	{
		printf("Memory must be allocated before being deallocated \n");
		exit(0);
	}
	if (ptr == NULL)
	{
		printf("Error. No such pointer exists.");
		printf("Error Type: %s \n", strerror(errno));
		exit(0);
	}
	if (fragmentation == 5)
	{
		my_coalesce();
	}
	
	int oldHeadSize = head->size;

  	header_t* hptr = (header_t*)ptr;
  	assert(hptr->magic == 729512);

  	int ptrSize = hptr->size;
  	totalSpace = totalSpace + ptrSize;
  	node_t* newHead = (node_t*)hptr; 

  	newHead->size = oldHeadSize;
  	newHead->next = head;
  	head = newHead;

  	head->size = ptrSize;
  	fragmentation++;

  	if (fragmentation == 10)
	{
		my_coalesce();
	}
 }


void my_showfreelist()
{
	if (myHeap == NULL)
	{
		printf("Sorry! No memory has been allocated yet!\n");
		exit(0);
	}

	node_t* temp = head;
	int counter = 0;

    while (temp != NULL) 
    {
		printf("node_no:%d node_size:%d node_start_address:%p\n", counter, temp->size, temp);
	  	temp = temp->next;
	  	counter++;
	}
}

void my_coalesce()
{
	if (myHeap == NULL)
	{
		printf("Sorry! No memory has been allocated yet!\n");
		exit(0);
	}

	node_t* temp = head->next;
	while (temp != NULL) 
    {
		if (((char*)head - (temp->size + sizeof(header_t))) == (char *) temp)
		{
			int a = temp->size;
			int b = head->size;

			node_t * merge = temp;
			merge->size = a + b;
			head = merge;
			temp = NULL;
		}

		else
		{
		  	head = head->next;
		  	temp = temp->next;
		}
	}
}

void* my_calloc(int num, int size)
{
	if (myHeap == NULL)
	{
		printf("Sorry! No memory has been allocated yet!\n");
		exit(0);
	}
	
	else
	{

		header_t * temp = my_malloc(num*size);
		memset(temp, 0, num*size);

		temp->size = size;
  		temp->magic = 729512;

		return temp;
	}
}

void * my_realloc(void * ptr, int size)
{
	if (myHeap == NULL)
	{
		printf("Sorry! No memory has been allocated yet!\n");
		exit(0);
	}

	if (ptr == 0 | ptr == NULL)
	{
		printf("Oops! There was an error. Pointer passed is invalid\n");
    	printf("Error: %s\n", strerror(errno));
    	exit(0);
	}

	else
	{
		header_t * temp = my_malloc(size);
		for (int i = sizeof(header_t); i < size; i++)
		{
			*(((char*) temp) + i) = *(((char*) ptr) + i);
		}
		my_free(ptr);

		temp->size = size;
  		temp->magic = 729512;

		return temp;
	}
}

void my_uninit()
{
	if (myHeap == NULL)
	{
		printf("Memory must be allocated before being deallocation.\n");
		exit(0);
	}

	int unmap = munmap(myHeap, pageSize);
	printf("Heap has been successfully deallocated!\n");

	if (unmap != 0)
	{
		printf("Oops! There was an error. Deallocation failed.\n");
    	printf("Error: %s\n", strerror(errno));
	}
}
