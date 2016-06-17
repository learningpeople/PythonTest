#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "OFsuite_link_connectivity.h"

struct tree node_tree;
static int search_result = 0;
int visited_sw[2000] = {0};
struct link_node* connectivity_array[MAX_SW_NUM] = {NULL};
int get_root_dpid() {
	return 1;
}
//Call this only after tree_init()
void assign_root(struct tree* tree){
	int root_dpid = get_root_dpid();
	struct link_node* tmp = link_node_init(root_dpid);
	tree->root = tmp;
	connectivity_array[root_dpid - 1] = tmp;

}

void tree_init(void){
	node_tree.node_num = 0;
	node_tree.root = NULL;
	assign_root(&node_tree);
}

void tree_destroy(void) {
	int node_num = node_tree.node_num;
	int i,j;
	for(i = 0; i < node_num; i++) {
		if (connectivity_array[i] != NULL) {
			//printf("\ndestroy %d nodes!\n", i+1);
			for(j = 0; j < connectivity_array[i]->child_num; j++) {
				connectivity_array[i]->child[j] = NULL;
			}
			connectivity_array[i]->sw_dpid = 0;
			connectivity_array[i]->child_num = 0;
			connectivity_array[i] = NULL;
		}
	}
	search_result = 0;
}

struct link_node* link_node_init(int dpid){
	struct link_node* ln = (struct link_node*)malloc(sizeof(struct link_node));
	ln->sw_dpid = dpid;
	ln->child_num = 0;
	memset(ln->child, 0, sizeof(ln->child));
	node_tree.node_num++;
	//printf("\nnow total %d nodes\n", node_tree.node_num);
	return ln;
}

int node_insert(int parent_dpid, int child_dpid){
	int i;
	int child_num;
	//printf("parent: %d, child: %d\n", parent_dpid, child_dpid);
	if (parent_dpid < MAX_SW_NUM && child_dpid < MAX_SW_NUM) {
		if(!connectivity_array[parent_dpid - 1])
			connectivity_array[parent_dpid - 1] = link_node_init(parent_dpid);
		if(!connectivity_array[child_dpid - 1])
			connectivity_array[child_dpid - 1] = link_node_init(child_dpid);
		child_num = connectivity_array[parent_dpid - 1]->child_num;
		if(child_num == 0){
			connectivity_array[parent_dpid - 1]->child[child_num] = connectivity_array[child_dpid - 1];
			connectivity_array[parent_dpid - 1]->child_num++;
		}
		else {
			for (i = 0; i < child_num; i++) {
				if(connectivity_array[parent_dpid - 1]->child[i] == connectivity_array[child_dpid - 1])
					break;				
			}
			if(i >= child_num) {
				connectivity_array[parent_dpid - 1]->child[child_num] = connectivity_array[child_dpid - 1];
				connectivity_array[parent_dpid - 1]->child_num++;
			}
		}
	}
	return 0;
}


int tree_search(int dst_dpid){
	struct link_node *ln = node_tree.root;
	memset(visited_sw, 0, sizeof(visited_sw));
	recu_search(ln, dst_dpid);
	return search_result;
}

void recu_search(struct link_node *ln, int dst_dpid) {
	int i = 0;
	
	if (ln != NULL && !search_result){
		if (!visited_sw[ln->sw_dpid]) {
			if (ln->sw_dpid == dst_dpid) {
				search_result = 1;
			}
			visited_sw[ln->sw_dpid] = 1;
			for (i = 0; i < ln->child_num; i++) {
				recu_search(ln->child[i], dst_dpid);
			}
		}
	}
}

void tree_print(void) {
	struct link_node *root = node_tree.root;
	memset(visited_sw, 0, sizeof(visited_sw));
	recu_print(root);
}

void recu_print(struct link_node *ln) {
	int i = 0;
	
	if (ln != NULL){
		if (!visited_sw[ln->sw_dpid]) {
			visited_sw[ln->sw_dpid] = 1;
			printf("\ncurrent switch: %d\n", ln->sw_dpid);
			for (i = 0; i < ln->child_num; i++) {
				recu_print(ln->child[i]);
			}
		}
	}
}

int get_dpid(int dpid) {
	int result = 0;
	struct link_node *ln = node_tree.root;
	search_result = 0;
	memset(visited_sw, 0, sizeof(visited_sw));
	recu_search(ln, dpid);
	result = search_result;
	return result;
}