#ifndef __TABLE_H__
#define __TABLE_H__

#include "flow.h"

struct sw_table{
    int table_id;
	struct flows *flows_head;

};

void insert_flows();
void delete_flows();
void ingress_packet_handler();

#endif
