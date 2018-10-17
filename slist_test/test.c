#include "../slist.h"
#include <stdio.h>

void print_slist(SList* list){
 for(int i=0; i<list->used; i++){
  printf("%d ", *(int*)slist_get(list, i));
 }
 printf("\n");
}
void debug_slist(SList* list){
 printf("ELEM_SIZE: %d, USED: %d, ALLOCED: %d\n", list->elem_size, list->used, list->alloced);
}
int main(){
 int arr0[] = {97,98,99,100};
 int arr1[] = {1,2,3,4,5};
 SList* list = slist_init(sizeof(int), 2);
 slist_append_last(list, arr0, sizeof(arr0)/sizeof(int));
 print_slist(list);
 slist_append_last(list, arr1, sizeof(arr1)/sizeof(int));
 print_slist(list);
 slist_remove_range(list, 0, 3);
 print_slist(list);
 slist_remove_range(list, 1, 2);
 print_slist(list);
 return 0;
}
