#include "flow.h"
#include "../src/openflow.h"
#include "arpa/inet.h"
struct ofp_header oh;
struct flows* init_flows()
{
	struct flows* flows = malloc(sizeof(struct flows));
	flows->length = 0;
	flows->flow_entry_head = NULL;
	return flows;
}

void free_flows(struct flows* flows)
{
	if(flows)
	{
		if(flows->flow_entry_head)
		{
			while(flows->flow_entry_head)
			{
				struct flow_entry* tmp = flows->flow_entry_head;
				flows->flow_entry_head = tmp->next_flow_entry;
				free(tmp->match);
				free(tmp->instructions);
				free(tmp);
			}
		}
		free(flows);
		//flows = NULL;
		//printf("%ld\n",(long)flows);
	}
}


struct flow_entry* create_flow_entry(struct parser_flow_mod pfm)
{
		struct flow_entry* flow_entry = malloc(sizeof(struct flow_entry));
		flow_entry->cookie = pfm.ofm.cookie;
		flow_entry->cookie_mask = pfm.ofm.cookie_mask;
		flow_entry->table_id = pfm.ofm.table_id;
		flow_entry->command = pfm.ofm.command;
		flow_entry->idle_timeout = pfm.ofm.idle_timeout;
		flow_entry->hard_timeout = pfm.ofm.hard_timeout;
		flow_entry->priority = pfm.ofm.priority;
		flow_entry->buffer_id = pfm.ofm.buffer_id;
		flow_entry->out_port = pfm.ofm.out_port;
		flow_entry->out_group = pfm.ofm.out_group;
		flow_entry->flags = pfm.ofm.flags;
		memset(flow_entry->pad, 0, 16);
		flow_entry->match_length = pfm.match_length;
		flow_entry->instruction_length = pfm.instruction_length;
		flow_entry->match = pfm.match;
		flow_entry->instructions = pfm.instructions;
		flow_entry->next_flow_entry = NULL;
		flow_entry->duration_sec = 0;
		flow_entry->duration_nsec = 0;
		flow_entry->packet_count = 0;
		flow_entry->byte_count = 0;
		return flow_entry;
}

uint8_t add_flow_entry(struct flows* flows, struct flow_entry* flow_entry)
{
		if(flows->flow_entry_head == NULL)
		{
			flows->flow_entry_head = flow_entry;
			flows->length = 1;
		}
		else
		{
			int i;
			struct flow_entry* last_flow_entry = flows->flow_entry_head;
			for(i = 0; i < flows->length - 1; i++)
			{
				last_flow_entry = last_flow_entry->next_flow_entry;
			}
			last_flow_entry->next_flow_entry = flow_entry;
			flows->length++;
		}
		return flows->length;
}

uint8_t get_flows_length(struct flows* flows)
{
	return (flows->length);
}

struct flow_entry* get_flow_entry_head(struct flows* flows)
{
	return(flows->flow_entry_head);
}


struct parser_flow_mod parser_flow_mod_msg(char* msg_buf){
	//printf("\ninto parser flow mod\n");
	struct parser_flow_mod pfm;
	memset(&pfm,0,sizeof(pfm));
	//printf("\nset pfm memory to 0\n");
	memcpy(&oh, msg_buf, sizeof(oh));
	//printf("\nversion:%X\n",oh.version);
	//printf("\ntype:%d\n",oh.type);
	//printf("\nlen:%d\n",ntohs(oh.length));
	//printf("\nxid:%d\n",ntohl(oh.xid));
	//uint16_t message_length = ntohs(oh.length);
	struct ofp11_flow_mod ofm;
	//printf("\nofm length: %lu\n", sizeof(ofm));
	memcpy(&ofm, msg_buf+sizeof(oh), sizeof(ofm));
	//printf("\npriority:%d\n",ntohs(ofm.priority));
	struct ofp11_match_header omh;
	memcpy (&omh, msg_buf+sizeof(oh)+sizeof(ofm), sizeof(omh));
	//printf("\nmatch_length:%d\n",ntohs(omh.length));
	uint16_t match_length = ntohs(omh.length);
	if((match_length % 8))
		match_length = 8 * ((match_length / 8) + 1);
	struct ofp11_instruction ofi;
	memcpy(&ofi, msg_buf+sizeof(oh)+sizeof(struct ofp11_flow_mod)+match_length, sizeof(ofi));
	uint16_t instruction_length = ntohs(ofi.len);
	//printf("\ninstruction_length:%d\n",ntohs(ofi.len));
	void* match = (void*)malloc(match_length);
	//printf("\nmatch pointor content:%lu\n", (long)match);
	memcpy(match, msg_buf+sizeof(oh)+sizeof(struct ofp11_flow_mod), match_length);
	void* instructions = (void*)malloc(instruction_length);
	memcpy(instructions, msg_buf+sizeof(oh)+sizeof(struct ofp11_flow_mod)+match_length, instruction_length);
	pfm.ofm = ofm;
	pfm.match = match;
	pfm.instructions= instructions;
	//pfm.match_length = match_length;
	pfm.match_length = ntohs(omh.length);
	pfm.instruction_length = instruction_length;
	return pfm;
}

void static _match_field_print(void* match)
{
	struct ofp11_match_header omh;
	memcpy(&omh, match, sizeof(omh));
	printf("match type:%d\n", ntohs(omh.type));
	printf("match length:%d\n", ntohs(omh.length));
	int i;
	char mem[ntohs(omh.length)];
	memset(mem,0,sizeof(mem));
	memcpy(mem, match, ntohs(omh.length));
	for(i = 0; i<ntohs(omh.length); i++)
	{
		printf("%x ", 0xFF & mem[i]);
	}
	printf("\n");
}

void static _instructions_actions_print(void* instructions)
{
	struct ofp11_instruction ofi;
	memcpy(&ofi, instructions, sizeof(ofi));
	int i;
	char mem[ntohs(ofi.len)];
	memset(mem,0,sizeof(mem));
	memcpy(mem, instructions, ntohs(ofi.len));
	for(i = 0; i<ntohs(ofi.len); i++)
	{
		printf("%x ", 0xFF & mem[i]);
	}
	printf("\n");
}

void flow_entry_print(struct flow_entry* flow_entry)
{
	//printf("%ld\n",(long)flow_entry);
	printf("Flow entry print:\n");
	printf("cookie:%lu\n",flow_entry->cookie);
	printf("cookie_mask:%lu\n",flow_entry->cookie_mask);
	printf("table_id:%d\n",flow_entry->table_id);
	printf("command:%d\n",flow_entry->command);
	printf("idle_timeout:%d\n",flow_entry->idle_timeout);
	printf("hard_timeout:%d\n",flow_entry->hard_timeout);
	printf("priority:%d\n",flow_entry->priority);
	printf("buffer_id:%d\n",flow_entry->buffer_id);
	printf("out_port:%d\n",flow_entry->out_port);
	printf("out_group:%d\n",flow_entry->out_group);
	printf("flags:%d\n",flow_entry->flags);
	printf("match_length:%d\n",flow_entry->match_length);
	printf("instruction_length:%d\n",flow_entry->instruction_length);
	_match_field_print(flow_entry->match);
	_instructions_actions_print(flow_entry->instructions);
	printf("duration_sec:%d\n",flow_entry->duration_sec);
	printf("duration_nsec:%d\n",flow_entry->duration_nsec);
	printf("packet_count:%lu\n",flow_entry->packet_count);
	printf("byte_count:%lu\n",flow_entry->byte_count);

}

char* compose_flow_status_reply(struct flow_entry* flow_entry){

	//TODO: Search the link list and compose proper flow status reply msg
	/*int match_length, instruction_length, msg_length;
	struct ofp11_match_header omh;
	memcpy(&omh, flow_entry->match, sizeof(omh));
	match_length = ntohs(omh.length);
	struct ofp11_instruction ofi;
	memcpy(&ofi, instructions, sizeof(ofi));
	instruction_length = ntohs(ofi.len);*/

	return NULL;
}

int get_output_port(struct parser_flow_mod pfm) {
	int port_no;
	memcpy(&port_no, pfm.instructions + 12, 4);
	port_no = ntohl(port_no);
	return port_no;
}