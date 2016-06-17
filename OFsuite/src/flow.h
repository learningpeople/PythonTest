#ifndef __FLOW_H__
#define __FLOW_H__

#include "types.h"
#include "OFsuite_ofpmsgs.h"
#include "openflow-1.1.h"

struct flow_entry{
	    uint64_t cookie; 
	    uint64_t cookie_mask; 
	    uint8_t table_id; 
	    uint8_t command; 
	    uint16_t idle_timeout; 
	    uint16_t hard_timeout; 
	    uint16_t priority; 
	    uint32_t buffer_id; 
	    uint32_t out_port; 
	    uint32_t out_group; 
	    uint16_t flags; 
	    uint8_t pad[2];
	    uint16_t match_length;
	    uint16_t instruction_length;
	    void* match; 
	    void* instructions;
	    uint32_t duration_sec;
	    uint32_t duration_nsec;
	    uint64_t packet_count;
	    uint64_t byte_count;
	    struct flow_entry* next_flow_entry;
};


struct flows{
	    int length;
	    struct flow_entry * flow_entry_head;
};

struct parser_flow_mod
{
	struct ofp11_flow_mod ofm;
	uint16_t match_length;
	uint16_t instruction_length;
	void* match;
	void* instructions;
};


struct flows* init_flows();
void free_flows(struct flows* flows);
struct flow_entry* create_flow_entry(struct parser_flow_mod pfm);
uint8_t add_flow_entry(struct flows* flows, struct flow_entry*);
uint8_t delete_flow_entry(struct flows* flows, struct ofp11_flow_mod* flow_mod);
uint8_t get_flows_length(struct flows* flows);
struct flow_entry* get_flow_entry_head(struct flows* flows);
void flow_entry_print(struct flow_entry* flow_entry);
struct parser_flow_mod parser_flow_mod_msg(char* buf);
char* compose_flow_status_reply(struct flow_entry* flow_entry);
int get_output_port(struct parser_flow_mod pfm);

#endif
