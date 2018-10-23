#ifndef SLIST_H
#define SLIST_H

#include "typeinfo.h"

typedef struct _SList{
 void* data;
 size_t used;
 size_t alloced;
 TypeInfo* type_info;
}SList;

void slist_set(SList* list, size_t i, void* ptr);
void* slist_get(SList* list, size_t i);
int slist_reserve(SList* list, size_t size);
int slist_get_len(SList* list);
SList* slist_init(TypeInfo* type_info, size_t alloced);
int slist_append_range(SList* list, void* src, int orig, size_t len);
int slist_append(SList* list, void* src, size_t len);
int slist_remove_range(SList* list, int orig, size_t len);

void* slist_serialize(void* list, void* ptr);
void* slist_deserialize(void* list, void* ptr);
void slist_free(void* list);
#endif