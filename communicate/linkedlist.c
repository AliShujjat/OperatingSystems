//https://www.tutorialspoint.com/data_structures_algorithms/linked_list_program_in_c.htm
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct node {
   int key;
   char name[256];
   struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

//display the list
void printList() {
   struct node *ptr = head;
   printf("\n[");
    
   //start from the beginning
   while(ptr != NULL) 
   {
      printf("(%d,%s)",ptr->key,ptr->name);
      ptr = ptr->next;
   }
    
   printf("]\n");
}

//insert link at the first location
void insertFirst(int key, char* name) {
   //create a link
   struct node *link = (struct node*) malloc(sizeof(struct node));
    
   link->key = key;
   memcpy(link->name,name,256);
   //point it to old first node
   link->next = head;

   //point first to new first node
   head = link;
}

//delete first item
struct node* deleteFirst() {

   //save reference to first link
   struct node *tempLink = head;
    
   //mark next to first link as first 
   head = head->next;
    
   //return the deleted link
   return tempLink;
}

int length() {
   int length = 0;
   struct node *current;
    
   for(current = head; current != NULL; current = current->next) {
      length++;
   }
    
   return length;
}

//find a link with given key
struct node* find(int key) {

   //start from the first link
   struct node* current = head;

   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->key != key) {
    
      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;
      }
   }      
    
   //if data found, return the current Link
   return current;
}

//delete a link with given key
struct node* delete(int key) {

   //start from the first link
   struct node* current = head;
   struct node* previous = NULL;
    
   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->key != key) {

      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //store reference to current link
         previous = current;
         //move to next link
         current = current->next;
      }
   }

   //found a match, update the link
   if(current == head) {
      //change first to point to next link
      head = head->next;
   } else {
      //bypass the current link
      previous->next = current->next;
   }    
    
   return current;
}