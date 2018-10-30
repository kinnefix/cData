#include <stdlib.h>

#include "slist.h"
#include "typeinfo.h"

SList* type_info_list;

TypeInfo* typeinfo_init(void* (*init)(void*), void* (*serialize)(void*,void*), void*(*deserialize)(void*,void*), void (*free)(void*), int data_size, char* tag){
 TypeInfo* typeinfo = malloc(sizeof(TypeInfo));
 typeinfo->init= init;
 typeinfo->serialize=serialize;
 typeinfo->deserialize=deserialize;
 typeinfo->free=free;
 typeinfo->data_size=data_size ;
 int idx = slist_append(type_info_list, &typeinfo, 1);
 typeinfo->type_idx = idx;
 strcpy(typeinfo->tag, tag);
 return typeinfo;
}