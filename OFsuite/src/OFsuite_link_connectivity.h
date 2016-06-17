#ifndef __OFSUITE_LINK_CONNECTIVITY_H__
#define __OFSUITE_LINK_CONNECTIVITY_H__

#define MAX_CHILD_NUM 50
#define MAX_SW_NUM 2000

struct link_node{
	int sw_dpid;
	int child_num;
	struct link_node* child[MAX_CHILD_NUM];
};

struct tree{
	int node_num;
	struct link_node* root;
};

int get_root_dpid();
void assign_root(struct tree* tree);
void tree_init(void);
void tree_destroy(void);
struct link_node* link_node_init(int dpid);
int node_insert(int, int);
int tree_search(int);
void free_tree(struct tree* tree);
void recu_search(struct link_node *ln, int dpid);
void tree_print(void);
void recu_print(struct link_node *ln);
int get_dpid(int dpid);
#endif
