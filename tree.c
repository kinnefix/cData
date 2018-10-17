#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tree.h"
#include "slist.h"

Node* tree_init(int data_size, char* (*serialize_func)(char*, char*), char* (*deserialize_func)(char*, char*)){
 Node* root = (Node*)malloc(sizeof(Node));
 root->data_size = data_size;
 root->data = malloc(data_size);
 root->child_list = slist_init(sizeof(Node*), 2);
 root->serialize_func = serialize_func;
 root->deserialize_func = deserialize_func;
 return root;
}

void tree_add_child(Node* parent, Node** child){
 slist_append_last(parent->child_list, child, 1);
}
int tree_write(Node* node, char* dest){
 char* orig = dest;
 int structure_len=0;
 tree_get_structure(node, NULL, &structure_len);
 char* structure = (char*)malloc(sizeof(char)*structure_len);
 tree_get_structure(node, structure,NULL);
 memcpy(dest, &structure_len, sizeof(int)); dest+=sizeof(int);
 memcpy(dest, &node->data_size, sizeof(int)); dest+=sizeof(int);
 memcpy(dest, structure, sizeof(char)*structure_len); dest+=structure_len;
 dest = tree_write_data(node, dest);
 free(structure);
 return dest-orig;
}
char* tree_get_structure(Node* node, char* ptr, int* len){
 int child_list_len = slist_get_len(node->child_list);
 for(int i=0; i<child_list_len; i++){
  if(ptr!=NULL)*ptr++='{'; if(len!=NULL) (*len)++;
  ptr=tree_get_structure(*(Node**)slist_get(node->child_list,i), ptr, len);
   if(ptr!=NULL)*ptr++='}';  if(len!=NULL)(*len)++;
 }
 return ptr;
}
char* tree_write_data(Node* node, char* ptr){
 ptr = node->serialize_func(ptr, node->data);
 int child_list_len = slist_get_len(node->child_list);
 for(int i=0; i<child_list_len; i++){
  ptr = tree_write_data(*(Node**)slist_get(node->child_list, i), ptr);
 }
 return ptr;
}

char* tree_read_data(char** src, char* structure, Node* node, char* (*serialize_func)(char*, char*), char* (*deserialize_func)(char*, char*)){
 
 *src = node->deserialize_func(node->data, *src);
 char* c = structure;
 for(;c[0]!='\0'; c++){
 printf("%c\n", c[0]);
  switch(c[0]){
   case '{':
   Node* child = tree_init(node->data_size, serialize_func, deserialize_func);
   tree_add_child(node, &child);
   // slist_append_last(node->child_list, &child, 1);
   c = tree_read_data(src, c+1, child, serialize_func, deserialize_func);
   break;
   case '}':
   return c;
   break;
  }
 }
}
Node* tree_read(char* src, char* (*serialize_func)(char*, char*), char* (*deserialize_func)(char*, char*)){
 int structure_len = *((int*)src)++;
 int data_size = *((int*)src)++;
 char* structure = (char*)malloc(sizeof(char)*(structure_len+1));
 memcpy(structure, src, sizeof(char)*structure_len);
 src+=structure_len;
 structure[structure_len]='\0';
 printf("STRUCTURE: %s\n", structure);
 Node* root = tree_init(data_size, serialize_func, deserialize_func);
 tree_read_data(&src, structure, root, serialize_func, deserialize_func);
 return root;
}