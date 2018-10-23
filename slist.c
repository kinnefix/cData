#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "slist.h"
#include "typeinfo.h"

extern SList* type_info_list;

void slist_set(SList* list, size_t i, void* ptr){
 int data_size = list->type_info->data_size;
 int err = slist_reserve(list, i+1);
 if(!err)
  return;
 memcpy(((char*)list->data)+i*data_size, ptr, data_size);
}
void* slist_get(SList* list, size_t i){
 return ((char*)list->data)+i*list->type_info->data_size;
}

void* slist_get_last_elem(SList* list){
 return slist_get(list, list->used);
}

int slist_reserve(SList* list, size_t max){
 size_t size;
 if(list->type_info)
  size = max*list->type_info->data_size;
 else
  size = sizeof(TypeInfo);
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
SList* slist_init(TypeInfo* type_info, size_t alloced){
 int elem_size;
 if(type_info==NULL)
  elem_size = sizeof(TypeInfo);
 else
  elem_size = type_info->data_size;
 SList* list = malloc(sizeof(SList));
 list->data = malloc(elem_size*alloced);
 list->type_info = type_info;
 list->used = 0;
 list->alloced = alloced;
 return list;
}

int slist_append_range(SList* list, void* src, int orig, size_t len){
 size_t elem_size;
 if(!slist_reserve(list, orig+len)){
  return 0;
 }
 if(list->type_info)
  elem_size = list->type_info->data_size;
 else
  elem_size = sizeof(TypeInfo);
 size_t used = list->used;
 char* ptr0 = ((char*)list->data)+orig*elem_size;
 char* ptr1 = ((char*)list->data)+(orig+len)*elem_size;

 memmove(ptr1, ptr0, (used-orig)*elem_size);
 memcpy(ptr0, src, len*elem_size);
 list->used = orig+len;
 return list->used; 
}

int slist_append(SList* list, void* src, size_t len){
 return slist_append_range(list, src, list->used, len);
}

int slist_remove_range(SList* list, int orig, size_t len){
 int elem_size = list->type_info->data_size;
 size_t used = list->used;
 char* ptr0 = ((char*)list->data)+orig*elem_size;
 char* ptr1 = ((char*)list->data)+(orig+len)*elem_size;
 memmove(ptr0, ptr1, (used-orig)*elem_size);
 list->used -=len;
 return 1;
}

void* slist_serialize(SList* list, void* ptr){
 size_t used = list->used;
 TypeInfo* type_info = list->type_info;
 memcpy(ptr, &type_info->type_idx, sizeof(int)); ((char*)ptr)+=sizeof(int);
 memcpy(ptr, &used, sizeof(size_t)); ((char*)ptr)+=sizeof(size_t);
 for(size_t i=0; i<used; i++){
  ptr= type_info->serialize(list->data, ptr);
 }
 return ptr;
}

void* slist_deserialize(SList* list, void* ptr){
 int type_idx;
 size_t used; 
 memcpy(&type_idx, ptr, sizeof(int)); ((char*)ptr)+=sizeof(int);
 TypeInfo* type_info = list->type_info = slist_get(type_info_list, type_idx);
 memcpy(&used, ptr, sizeof(size_t)); ((char*)ptr)+=sizeof(size_t);
 list->used = used;
 slist_reserve(list, used*2); 
 for(size_t i=0; i<used; i++){
  void* data_ptr = type_info->init();
  ptr = type_info->deserialize(data_ptr, ptr);
  slist_set(list, i, data_ptr);
 }
 return ptr;
}
void slist_free(SList* list){
 size_t used = list->used;
 TypeInfo* type_info = list->type_info;
 for(size_t i=0; i<used; i++){
  type_info->free(slist_get(list, i));
 }
 free(list);
}