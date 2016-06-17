/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */

#ifndef OFSUITE_TOPOLOGY_H
#define OFSUITE_TOPOLOGY_H 1

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../OFsuite_performance.h"

typedef struct OFsuite_topology {
    uint16_t sw_num;
    fakesw *fksw[MAX_SW];
    uint8_t matrix[MAX_SW][MAX_SW];
}topology;

enum topology_type {
	TOPO_NONE   = 0,
	TOPO_LINEAR = 1,
	TOPO_RING   = 2,
	TOPO_FULL_MESH = 3,
	TOPO_LEAF_SPINE = 4,
	TOPO_TREE = 5,
	TOPO_MANUAL = 6,
};

void create_topology(int n_fakesw, topology *topo, uint8_t topo_mod);
void update_topology(int cur_n_fakesw, int add_n_fakesw, topology *OFsuite_topo, uint8_t topo_mode);
#endif