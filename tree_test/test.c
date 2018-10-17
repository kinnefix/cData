#include "../tree.h"
#include "../slist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  Node* node = *(Node**)slist_get(root->child_list, i);
  print_tree(node, d+1);
 }
}

int main(){
 Node ** root = (Node**)malloc(sizeof(Node*)*10);
 for(int i=0; i<10; i++){
  root[i] = tree_init(sizeof(int), serialize, deserialize);
  memcpy(root[i]->data, &i, sizeof(int));
 }
 
 tree_add_child(root[0], root+1);
 tree_add_child(root[1], root+2);
 tree_add_child(root[1], root+3);
 tree_add_child(root[1], root+4);
 print_tree(root[0],0);
 
 char* buf = (char*)malloc(sizeof(char)*1024);
 tree_write(root[0], buf);
 Node* node = tree_read(buf, serialize, deserialize);
 print_tree(node, 0);
 printf("Terminated\n");
 return 0;
}