#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../tree.h"
#include "../slist.h"
#include "../typeinfo.h"

extern SList* type_info_list;
TypeInfo* tmp_type_info;
typedef struct{
 int val;
 TypeInfo* type_info;
}Tmp;

void* tmp_init(){
 Tmp* tmp = malloc(sizeof(Tmp));
 return tmp;
}
void* tmp_serialize(void* tmp, void* ptr){
 memcpy(ptr, &((Tmp*)tmp)->val, sizeof(int)); ((char*)ptr)+=sizeof(int);
 return ptr;
}
void* tmp_deserialize(void* tmp, void* ptr){
 memcpy(&((Tmp*)tmp)->val, ptr, sizeof(int)); ((char*)ptr)+=sizeof(int);
 return ptr;
}

void tmp_free(void* tmp){
 free(tmp);
}
char* serialize(char* dest, char* src){
 memcpy(dest, src, sizeof(int));
 return dest+sizeof(int);
}
char* deserialize(char* dest, char* src){
 memcpy(dest, src, sizeof(int));
 return src+sizeof(int);
}
void print_tree(Node* root, int d){
 printf("%d : %d \n", d, *((int*)root->data));
 int len= slist_get_len(root->child_list);
 for(int i=0; i<len; i++){
  Node* node = slist_get(root->child_list, i);
  print_tree(node, d+1);
 }
}

void init_type_info(){
 type_info_list = slist_init(NULL, 8);
 node_type_info = typeinfo_init(node_init, node_serialize, node_deserialize, node_free, sizeof(Node));
 tmp_type_info= typeinfo_init(tmp_init, tmp_serialize, tmp_deserialize, tmp_free, sizeof(Tmp));
}
int main(){
 init_type_info();
 Node ** root = (Node**)malloc(sizeof(Node*)*10);
 Node* node = node_init();
 for(int i=0; i<10; i++){
  root[i] = tree_init(tmp_type_info);
  memcpy(root[i]->data, &i, sizeof(int));
 }
 printf("Init\n");
 tree_add_child(root[0], root[1]);
 tree_add_child(root[1], root[2]);
 tree_add_child(root[1], root[3]);
 tree_add_child(root[1], root[4]);
 printf("Added\n");
 print_tree(root[0],0);
 char* buf = (char*)malloc(sizeof(char)*1024);
 node_serialize(root[0], buf);
 node_deserialize(node, buf);
 print_tree(node, 0);
 printf("Terminated\n");
 return 0;
}