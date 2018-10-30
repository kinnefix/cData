#ifndef _TYPEINFO_H
#define _TYPEINFO_H
#include "slist.h"

typedef struct {
 void* (*init)(void*);
 void* (*serialize)(void*,void*);
 void* (*deserialize)(void*,void*);
 void (*free)(void*);
 char tag[8];
 int data_size;
 int type_idx;
}TypeInfo;

TypeInfo* typeinfo_init(void* (*init)(void*), void* (*serialize)(void*,void*), void*(*deserialize)(void*,void*), void (*free)(void*), int data_size, char* tag);
#endif