#include "slist.h"
typedef struct Node{
 void* data;
 int data_size;
 SList* child_list; //List<Node*>
 char* (*serialize_func)(char* , char* );
 char* (*deserialize_func)(char* , char* );
}Node;

Node* tree_init(int data_size, char* (*serialize_func)(char*, char*),char* (*deserialize_func)(char*, char*));
void tree_add_child(Node* parent, char* data);
int tree_write(Node* node, char* dest);
char* tree_get_structure(Node* node, char* ptr, int* len);
char* tree_write_data(Node* node, char* ptr);
char* tree_read_data(char** src, char* structure, Node* node);
Node* tree_read(char* src);