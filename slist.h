#ifndef SLIST_H
#define SLIST_H
typedef struct _SList{
 char* data;
 int elem_size;
 int used;
 int alloced;
}SList;

char* slist_get(SList* list, int i);
int slist_reserve(SList* list, int size);
int slist_get_len(SList* list);
SList* slist_init(int elem_size, int alloced);
int slist_append_range(SList* list, void* src, int orig, int len);
int slist_append_last(SList* list, void* src, int len);
int slist_remove_range(SList* list, int orig, int len);
int slist_write(SList* list, const char* filepath);
SList* slist_read(const char* filepath);
#endif