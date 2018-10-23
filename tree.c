#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tree.h"
#include "slist.h"

extern SList* type_info_list;
TypeInfo* node_type_info;

void* node_init(){
 Node* node = (Node*)malloc(sizeof(Node));
 return (void*)node;
}

Node* tree_init(TypeInfo* type_info){
 Node* root = (Node*)malloc(sizeof(Node));
 root->type_info = type_info;
 root->data = malloc(type_info->data_size);
 root->child_list = slist_init(node_type_info, 2);
 return root;
}

void tree_add_child(Node* parent, void* child){
 slist_append(parent->child_list, child, 1);
}

void* node_serialize(Node* node, void* ptr){
 TypeInfo* type_info = node->type_info;
 memcpy(ptr, &type_info->type_idx, sizeof(int)); ((char*)ptr)+=sizeof(int);
 ptr = type_info->serialize(node->data, ptr);
 ptr = slist_serialize(node->child_list, ptr);
 return ptr;
}
void* node_deserialize(void* dest, void* ptr){
 int type_idx;
 Node* node = (Node*)dest;
 memcpy(&type_idx, ptr, sizeof(int)); ((char*)ptr)+=sizeof(int);
 TypeInfo* type_info=slist_get(type_info_list, type_idx);
 node->type_info = type_info;
 node->data = type_info->init();
 ptr =  type_info->deserialize(node->data, ptr);
 node->child_list = slist_init(node_type_info, 2);
 ptr = slist_deserialize(node->child_list, ptr);
 return ptr;
}
void node_free(void* target){
 Node* node = (Node*)target;
 slist_free(node->child_list);
 node->type_info->free(node->data);
 free(node);
}