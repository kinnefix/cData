#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "slist.h"

char* slist_get(SList* list, int i){
 int err = slist_reserve(list, i+1);
 if(!err)
  return NULL;
 return list->data+i*list->elem_size;
}

char* slist_get_last_elem(SList* list){
 return slist_get(list, list->used);
}

int slist_reserve(SList* list, int max){
 int size = max*list->elem_size;
 if(list->alloced<max){
  list->data = realloc(list->data, size*2);
  if(list->data ==NULL)
   return 0;
  list->alloced = max*2;
 }
 return 1;
}
int slist_get_len(SList* list){
 return list->used;
}
SList* slist_init(int elem_size, int alloced){
 SList* list = malloc(sizeof(SList));
 list->data = malloc(elem_size*alloced);
 list->elem_size = elem_size;
 list->used = 0;
 list->alloced = alloced;
 return list;
}

int slist_append_range(SList* list, void* src, int orig, int len){
 if(!slist_reserve(list, orig+len)){
  return 0;
 }
 int elem_size = list->elem_size;
 int used = list->used;
 char* ptr0 = list->data+orig*elem_size;
 char* ptr1 = list->data+(orig+len)*elem_size;

 memmove(ptr1, ptr0, (used-orig)*elem_size);
 memcpy(ptr0, src, len*elem_size);
 list->used = orig+len;
 return 1; 
}

int slist_append_last(SList* list, void* src, int len){
 return slist_append_range(list, src, list->used, len);
}

int slist_remove_range(SList* list, int orig, int len){
 int elem_size = list->elem_size;
 int used = list->used;
 char* ptr0 = list->data+orig*elem_size;
 char* ptr1 = list->data+(orig+len)*elem_size;
 memmove(ptr0, ptr1, (used-orig)*elem_size);
 list->used -=len;
 return 1;
}

int slist_write(SList* list, const char* filepath){
 FILE* fp = fopen(filepath, "wb");
 if(fp==NULL){
  return 0;
 }
 int elem_size = list->elem_size;
 int used = list->used;
 fwrite(&elem_size, sizeof(int),1,  fp);
 fwrite(&used, sizeof(int), 1, fp);
 fwrite(list->data, elem_size, used, fp);
 fclose(fp);
 return 1;
}

SList* slist_read(const char* filepath){
 FILE* fp = fopen(filepath, "rb");
 if(fp==NULL){
  return NULL;
 }
 int used;
 int elem_size;
 int alloced;
 SList* list = malloc(sizeof(SList));
 fread(&elem_size , sizeof(int), 1, fp);
 fread(&used , sizeof(int), 1, fp);
 alloced = elem_size*used*2;
 list->data = malloc(alloced);
 fread(list->data, elem_size, used, fp);
 fclose(fp);
 return list;
}
