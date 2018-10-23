#ifndef _TREE_H
#define _TREE_H
#include "slist.h"
typedef struct Node{
 void* data;
 SList* child_list; //List<Node*>
 TypeInfo* type_info;
}Node;

extern TypeInfo* node_type_info;

void* node_init();
Node* tree_init(TypeInfo* type_info);
void tree_add_child(Node* parent, void* data);
void* node_serialize(void* node, void* ptr);
void* node_deserialize(void* node, void* ptr);
void node_free(void* node);
#endif