/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */


#include <string.h>
#include <unistd.h>
#include "OFsuite_handler.h"
#include "OFsuite_ofpmsgs.h"
#include "openflow.h"
#include "OFsuite_socket.h"
#include "OFsuite_buffer.h"
#include "OFsuite_topology.h"
#include "OFsuite_log.h"
#include "../OFsuite_performance.h"
#include "openflow-1.1.h"
#include "flow.h"
#include "OFsuite_ofpmsgs.h"
#include "OFsuite_link_connectivity.h"

uint8_t ALL_DONE = 0;
ofp_buf send_buf[MAX_BUF_SIZE];

extern fakesw *p_fakesw;
extern uint8_t n_controllers;
extern int n_fakesw;
extern uint8_t n_auxiliary_ch;
extern topology OFsuite_topo;
static uint8_t contr_no;
static uint16_t fksw_no;
static uint8_t flag = 0;
static uint64_t receive_packet_out = 0;
static uint64_t send_arp_reply = 0;
extern int total_send;
extern struct send_port sp[MAX_SW];
extern uint8_t module_log[32];
extern uint8_t start_conn_test;
static uint8_t link_ch = 0;
static uint8_t controller = 0;
extern uint16_t send_dpid[MAX_SW];
extern uint8_t conn_test_done;
extern pthread_mutex_t mutx;
extern pthread_cond_t cond;
extern pthread_cond_t cond_lldp;
extern pthread_cond_t cond_switchover;
extern pthread_cond_t cond_packet_out;
extern uint64_t send_arp;
extern uint8_t start_send_arp;
extern uint8_t is_datapath_switchover;
extern uint8_t is_datapath_failure;
extern uint16_t switch_host;
extern uint8_t all_arp_send;
struct ofp_header oh;
struct ether_header eth_header;
uint16_t sc_flags = 0;


inline uint64_t htonll(uint64_t v) {
    union { uint32_t lv[2]; uint64_t llv; } u;
    u.lv[0] = htonl(v >> 32);
    u.lv[1] = htonl(v & 0xFFFFFFFFULL);
    return u.llv;
}

inline uint64_t ntohll(uint64_t v) {
    union { uint32_t lv[2]; uint64_t llv; } u;
    u.llv = v;
    return ((uint64_t)ntohl(u.lv[0]) << 32) | (uint64_t)ntohl(u.lv[1]);
}

uint8_t handle_hello(ofp_buf *msg_buf, uint16_t ofconn) {
	ALL_DONE = 0;
	init_ofp_buf(send_buf);
	memcpy(&oh, msg_buf, sizeof(oh));
	oh.version = OFP13_VERSION;
	oh.type = OFPTYPE_HELLO;
	memcpy(send_buf, msg_buf, ntohs(oh.length));
	memcpy(send_buf, &oh, sizeof(oh));
	send_msg(ofconn, send_buf, ntohs(oh.length));
	ALL_DONE = 1;
	return 0;
}

uint8_t handle_echo_request(ofp_buf *msg_buf, uint16_t ofconn) {
	ALL_DONE = 0;
	init_ofp_buf(send_buf);
	memcpy(&oh, msg_buf, sizeof(oh));
	oh.type = OFPTYPE_ECHO_REPLY;
	memcpy(send_buf, msg_buf, ntohs(oh.length));
	memcpy(send_buf, &oh, sizeof(oh));
	send_msg(ofconn, send_buf, ntohs(oh.length));
	ALL_DONE = 1;
	return 0;
}

uint8_t handle_features_request(ofp_buf *msg_buf, uint16_t ofconn) {

	struct ofp_switch_features sw_features;
	static int i = 0;
	fakesw *fksw = get_switch_from_sockfd(ofconn);
	/*
	for(fksw_no = 0; fksw_no < n_fakesw; fksw_no++) {
		for (contr_no = 0; contr_no < n_controllers; contr_no++) {
            if (ofconn == fksw->sock_fd[contr_no]) {
                flag = 1;
                break;
            }
        }
        if (flag) {
        	break;
        }
        fksw++;
	}*/
    ALL_DONE = 0;
	if (fksw != NULL) {
		flag = 0;
		init_ofp_buf(send_buf);
		memcpy(&oh, msg_buf, sizeof(oh));
		oh.type = OFPTYPE_FEATURES_REPLY;
		oh.length = htons(sizeof(oh) + sizeof(sw_features));
		sw_features.datapath_id = htonll(fksw->dpid);
	    sw_features.n_buffers = htonl(0);
	    sw_features.n_tables = 1;
	    sw_features.auxiliary_id = 0;
	    sw_features.capabilities = htonl(0x0000004f);

	    memcpy(send_buf, &oh, sizeof(oh));
	    memcpy(send_buf+sizeof(oh), &sw_features, sizeof(sw_features));
	    send_msg(ofconn, send_buf, sizeof(oh) + sizeof(sw_features));
		OFsuite_log(LOG_MAX_SW, ++i, "switches connect to controller!");
		usleep(5000); 
	}
	else {
		//uint8_t auxi_no;
		struct fksw_auxi fk_au;
		get_switch_from_auxifd(&fk_au, ofconn);
		/*
		for(fksw_no = 0; fksw_no < n_fakesw; fksw_no++) {
			for (contr_no = 0; contr_no < n_controllers; contr_no++) {
				for (auxi_no = 0; auxi_no < n_auxiliary_ch; auxi_no++) {
	            	if (ofconn == fksw->auxi[contr_no*n_auxiliary_ch+auxi_no].auxi_fd) {
	                	flag = 1;
	                	break;
	            	}
	            }
	            if (flag)
	            	break;
	        }
	        if (flag) {
	        	break;
	        }
	        fksw++;
		}*/
		if (fk_au.fksw) {
			flag = 0;
			init_ofp_buf(send_buf);
			memcpy(&oh, msg_buf, sizeof(oh));
			oh.type = OFPTYPE_FEATURES_REPLY;
			oh.length = htons(sizeof(oh) + sizeof(sw_features));
			sw_features.datapath_id = htonll(fk_au.fksw->dpid);
		    sw_features.n_buffers = htonl(0);
		    sw_features.n_tables = 1;
		    sw_features.auxiliary_id = fk_au.auxi_id;
		    sw_features.capabilities = htonl(0x0000004f);

		    memcpy(send_buf, &oh, sizeof(oh));
		    memcpy(send_buf+sizeof(oh), &sw_features, sizeof(sw_features));
		    send_msg(ofconn, send_buf, sizeof(oh) + sizeof(sw_features));
		    usleep(5000);
		}
		else {
			OFsuite_syslog(LV_ERROR, "OFsuite_features", "Target switch not found!");
		}
	}
	ALL_DONE = 1;
	return 0;
}

uint8_t handle_set_config(ofp_buf *msg_buf, uint16_t ofconn) {
	struct ofp_switch_config ofp_sc;
	ALL_DONE = 0;
	memcpy(&ofp_sc, msg_buf, sizeof(ofp_sc));
	sc_flags = ofp_sc.flags;
	ALL_DONE = 1;
	return 0;
}

uint8_t handle_flow_mod(ofp_buf *msg_buf, uint16_t ofconn) {
	static uint64_t receive_flow_mod = 0;
	static int half_done = 0;
	static int full_done = 0;
	int ofport_no = 0;
	struct parser_flow_mod pfm = parser_flow_mod_msg(msg_buf);
	struct flow_entry* pfe = create_flow_entry(pfm);
	//fakesw *fksw = get_switch_from_sockfd(ofconn);
	//if(fksw != NULL) {
	//	add_flow_entry(fksw->flows, pfe);
	//}
	ALL_DONE = 0;
	if(module_log[LOG_FLOW_MOD_RATE] || module_log[LOG_FLOW_MOD_AUXI_RATE]||module_log[LOG_AUXILIARY_CHANNEL]) {
		if (ntohs(pfe->priority) == 1) {
			ofport_no = get_output_port(pfm);
			controller = 1;
			if (ofport_no == (MAX_SW + 1) && pfm.match_length > 4) {
				receive_flow_mod++;
				OFsuite_log(LOG_FLOW_MOD_RATE, receive_flow_mod, "receive flow mod");
				OFsuite_log(LOG_AUXILIARY_CHANNEL, ofconn, "receive flow mod");
				OFsuite_log(LOG_FLOW_MOD_AUXI_RATE, receive_flow_mod, "receive flow mod");
			}
			if (all_arp_send && receive_flow_mod >= send_arp_reply) {
				pthread_mutex_lock(&mutx);
				pthread_cond_signal(&cond_packet_out);
				pthread_mutex_unlock(&mutx);
			}
		}
		else if (ntohs(pfe->priority) == 10) {
			ofport_no = get_output_port(pfm);
			if (start_send_arp) {
				if (ofport_no == (MAX_SW + 2) && pfm.match_length > 4) {
					if (receive_flow_mod < send_arp_reply) {
						receive_flow_mod++;
						OFsuite_log(LOG_FLOW_MOD_RATE, receive_flow_mod, "receive flow mod");
						OFsuite_log(LOG_AUXILIARY_CHANNEL, ofconn, "receive flow mod");
						OFsuite_log(LOG_FLOW_MOD_AUXI_RATE, receive_flow_mod, "receive flow mod");
					}
					if (all_arp_send && receive_flow_mod >= send_arp_reply) {
						pthread_mutex_lock(&mutx);
						pthread_cond_signal(&cond_packet_out);
						pthread_mutex_unlock(&mutx);
					}
				}
			}
		}
	}
	else if(module_log[LOG_CONNECTIVITY_TEST] || module_log[LOG_DATAPATH_SWITCHOVER] || module_log[LOG_LINK_EVENT]) {
		uint16_t fksw_no;
		uint8_t con_no;
		uint8_t auxi_no;
		static uint8_t is_tree_destroied = 0;
		//static uint8_t switchover_end = 0;
		uint8_t flag = 0;
		for (fksw_no = 0; fksw_no < n_fakesw; fksw_no++) {
			for (con_no = 0; con_no < n_controllers; con_no++) {
				if (OFsuite_topo.fksw[fksw_no]->sock_fd[con_no] == ofconn) {
					flag = 1;
					break;
				}
				for (auxi_no = 0; auxi_no < n_auxiliary_ch; auxi_no++) {
	            	if (ofconn == OFsuite_topo.fksw[fksw_no]->auxi[con_no*n_auxiliary_ch+auxi_no].auxi_fd) {
	                	flag = 1;
	                	break;
            		}
				}
				if(flag)
					break;
			}
			if(flag)
				break;
		}
		if (flag && start_conn_test) {
			int ofport_no = get_output_port(pfm);
			if(is_datapath_switchover && !is_tree_destroied){
				is_tree_destroied = 1;
				half_done = 0;
				full_done = 0;
			}
			if(is_datapath_failure && !is_tree_destroied){
				is_tree_destroied = 1;
				half_done = 0;
				full_done = 0;
			}
			if (ofport_no == MAX_SW + 1) {
				if(!half_done) {
					half_done = MAX_SW+1;
					OFsuite_log(LOG_CONNECTIVITY_TEST, 1, "processing connectivity test");
				}
				else {
					if (!full_done && half_done != MAX_SW+1) {
						full_done = MAX_SW+1;
						OFsuite_log(LOG_CONNECTIVITY_TEST, 2, "end connectivity log");
						if(is_datapath_switchover){
							OFsuite_log(LOG_DATAPATH_SWITCHOVER, 1, "end datapath switchover test");
						}
					}
				}
			}
			if (ofport_no == MAX_SW + 2) {
				if(!half_done) {
					half_done = MAX_SW+2;
					OFsuite_log(LOG_CONNECTIVITY_TEST, 1, "processing connectivity test");
				}
				else {
					if (!full_done && half_done != MAX_SW+2) {
						full_done = MAX_SW+2;
						OFsuite_log(LOG_CONNECTIVITY_TEST, 2, "end connectivity log");
						if(is_datapath_switchover){
							OFsuite_log(LOG_DATAPATH_SWITCHOVER, 1, "end datapath switchover test");
						}
					}
				}
			}
			if (half_done && full_done && !conn_test_done) {
				pthread_mutex_lock(&mutx);
				conn_test_done = 1;
				pthread_cond_signal(&cond);
				pthread_mutex_unlock(&mutx);
			}
			if(ofport_no > 0) {
				node_insert(OFsuite_topo.fksw[fksw_no]->dpid, ofport_no);
				printf("parent: %d, child: %d\n", OFsuite_topo.fksw[fksw_no]->dpid, ofport_no);
			}
		}
		/*if (flag && is_datapath_switchover) {
			int ofport_no = get_output_port(pfm);
			if (!is_tree_destroied) {
				tree_destroy();
				is_tree_destroied = 1;
				OFsuite_log(LOG_DATAPATH_SWITCHOVER, 0, "datapath switchover start");
			}
			node_insert(OFsuite_topo.fksw[fksw_no]->dpid, ofport_no);
			if (tree_search(switch_host) && !switchover_end) {
				OFsuite_log(LOG_DATAPATH_SWITCHOVER, 1, "datapath switchover end");
				tree_print();
				pthread_mutex_lock(&mutx);
				switchover_end = 1;
				pthread_cond_signal(&cond_switchover);
				pthread_mutex_unlock(&mutx);
			}
		}*/
	}
	else if (module_log[LOG_PROACTIVE_FLOW]) {
		uint32_t out_port = 0;
		//printf("\nflow mod match length: %d, instructions length: %d\n", pfm.match_length, pfm.instruction_length);
		if (pfm.match_length > 4 && pfm.instruction_length > 0) {
			memcpy(&out_port, pfm.instructions+12, 4);
			out_port = ntohl(out_port);
			//printf("\nout port: %x\n", out_port);
			if (out_port != 0xfffffffd) {
				OFsuite_log(LOG_PROACTIVE_FLOW, ++receive_flow_mod, "receive proactive flow mod");
			}
		}
	}
	ALL_DONE = 1;
	return 0;
}

uint8_t handle_multipart_request(ofp_buf *msg_buf, uint16_t ofconn) {
	uint16_t multipart_type;
	ALL_DONE = 0;
	memcpy(&oh, msg_buf, sizeof(oh));
	memcpy(&multipart_type, msg_buf + sizeof(oh), sizeof(uint16_t));
	multipart_type = ntohs(multipart_type);

	switch(multipart_type) {
		case OFPMP_PORT_DESC:
			handle_mp_port_desc(msg_buf, ofconn);
			break;
		case OFPMP_DESC:
			handle_mp_desc(msg_buf, ofconn);
			break;
		case OFPMP_PORT_STATS:
			handle_mp_port_stats(msg_buf, ofconn);
			break;
		case OFPMP_FLOW:
			handle_mp_flow(msg_buf, ofconn);
			break;
		case OFPMP_TABLE:
			handle_mp_table(msg_buf, ofconn);
			break;
		default:
			handle_mp_default(msg_buf, ofconn);
			break;
	}
	ALL_DONE = 1;
	return 0;

}

uint8_t handle_mp_port_desc(ofp_buf *msg_buf, uint16_t ofconn) {
	int i = 0;
	struct p_port *p;
    uint8_t flag;
    uint8_t auxi_no;
	struct ofp_multipart_reply mp;
	fakesw *fksw = p_fakesw;
	for(fksw_no = 0; fksw_no < n_fakesw; fksw_no++) {
		for (contr_no = 0; contr_no < n_controllers; contr_no++) {
            if (ofconn == fksw->sock_fd[contr_no]) {
                flag = 1;
                break;
            }
            for (auxi_no = 0; auxi_no < n_auxiliary_ch; auxi_no++) {
	        	if (ofconn == OFsuite_topo.fksw[fksw_no]->auxi[contr_no*n_auxiliary_ch+auxi_no].auxi_fd) {
	            	flag = 1;
	            	break;
	    		}
			}
			if(flag)
				break;
        }
        if (flag) {
            break;
        }
        else
            fksw++;
	}
	if (flag) {
		flag = 0;
		p = fksw->pport;
		while (p != NULL) {
			i++;
			p = p->next;
		}
		init_ofp_buf(send_buf);
		memcpy(&oh, msg_buf, sizeof(oh));
		oh.type = OFPTYPE_MULTIPART_REPLY;
		oh.length = htons(sizeof(oh) + 8 + i * sizeof(struct ofp_port));
		memcpy(send_buf, &oh, sizeof(oh));
		mp.type = htons(OFPMP_PORT_DESC);
		mp.flags = htons(0);
		i = 0;
		p = fksw->pport;
		memcpy(send_buf+sizeof(oh), &mp, 8);
		while (p != NULL) {
			memcpy(send_buf+sizeof(oh)+8+i*sizeof(struct ofp_port), &(p->port), 64);
			i++;
			p = p->next;
		}
		send_msg(ofconn, send_buf, sizeof(oh) + 8 + i * sizeof(struct ofp_port));
	}
	return 0;
}

uint8_t handle_mp_port_stats(ofp_buf *msg_buf, uint16_t ofconn) {
	int i = 0;
    uint8_t flag;
    uint8_t auxi_no;
    struct p_port *p;
	struct ofp_multipart_reply mp;
	struct ofp_mp_port_stats port_stats;
	struct ofp_port_stats_request port_stats_req;
	fakesw *fksw = p_fakesw;
	for(fksw_no = 0; fksw_no < n_fakesw; fksw_no++) {
		for (contr_no = 0; contr_no < n_controllers; contr_no++) {
            if (ofconn == fksw->sock_fd[contr_no]) {
                flag = 1;
                break;
            }
            for (auxi_no = 0; auxi_no < n_auxiliary_ch; auxi_no++) {
	        	if (ofconn == OFsuite_topo.fksw[fksw_no]->auxi[contr_no*n_auxiliary_ch+auxi_no].auxi_fd) {
	            	flag = 1;
	            	break;
	    		}
			}
			if(flag)
				break;
        }
        if (flag) {
            break;
        }
        else
            fksw++;
	}
	if (flag) {
		flag = 0;
		i = fksw->n_port;
		init_ofp_buf(send_buf);
		memcpy(&oh, msg_buf, sizeof(oh));
		memcpy(&port_stats_req, msg_buf+16, sizeof(port_stats_req));
		oh.type = OFPTYPE_MULTIPART_REPLY;
		if(port_stats_req.port_no == htonl(0xffffffff)) {
			oh.length = htons(sizeof(oh) + 8 + i * sizeof(struct ofp_mp_port_stats));
		}
		else {
			oh.length = htons(sizeof(oh)+8+sizeof(struct ofp_mp_port_stats));
		}
		memcpy(send_buf, &oh, sizeof(oh));
		mp.type = htons(OFPMP_PORT_STATS);
		mp.flags = htons(0);
		memcpy(send_buf+sizeof(oh), &mp, 4);
		memset(send_buf+sizeof(oh)+4, 0, 4);
		p = fksw->pport;
		memset(&port_stats, 0, sizeof(port_stats));
		if(port_stats_req.port_no == htonl(0xffffffff)) {
			for(i = 0; i < fksw->n_port; i++) {
				port_stats.port_no = p->port.port_no;
				memcpy(send_buf+sizeof(oh)+8+i*sizeof(port_stats), &port_stats, sizeof(port_stats));
				p = p->next;
			}
			send_msg(ofconn, send_buf, ntohs(oh.length));
		}
		else{
			while(htonl(p->port.port_no) != port_stats_req.port_no && p != NULL) {
				p = p->next;
			}
			if(p != NULL) {
				port_stats.port_no = p->port.port_no;
				memcpy(send_buf+sizeof(oh)+8+sizeof(port_stats), &port_stats, sizeof(port_stats));
				send_msg(ofconn, send_buf, ntohs(oh.length));
			}
		}
	}
	return 0;
}

uint8_t handle_mp_desc(ofp_buf *msg_buf, uint16_t ofconn) {
	struct ofp_desc ofpd;
	char *mfr_desc = "BII";
	char *hw_desc = "OFsuite_performance";
	char *sw_desc = "0.9.9";
	char *serial_no = "None";
	char *dp_desc = "None";
	memset(&ofpd, 0, sizeof(ofpd));
	memcpy(&ofpd, msg_buf, 16);
	ofpd.header.type = OFPTYPE_MULTIPART_REPLY;
	ofpd.header.length = htons(1072);
	memcpy(&ofpd.mfr_desc, mfr_desc, 4);
	memcpy(&ofpd.hw_desc, hw_desc, 20);
	memcpy(&ofpd.sw_desc, sw_desc, 6);
	memcpy(&ofpd.serial_num, serial_no, 5);
	memcpy(&ofpd.dp_desc, dp_desc, 5);
	send_msg(ofconn, &ofpd, sizeof(ofpd));
	return 0;
}

uint8_t handle_mp_flow(ofp_buf *msg_buf, uint16_t ofconn) {
	init_ofp_buf(send_buf);
	memcpy(&oh, msg_buf, sizeof(oh));
	oh.type = OFPTYPE_MULTIPART_REPLY;
	oh.length = htons(16);
	memcpy(send_buf, &oh, sizeof(oh));
	memcpy(send_buf+ sizeof(oh), msg_buf + sizeof(oh), 8);
	send_msg(ofconn, send_buf, 16);
	return 0;
}

uint8_t handle_mp_table(ofp_buf *msg_buf, uint16_t ofconn) {
	init_ofp_buf(send_buf);
	memcpy(&oh, msg_buf, sizeof(oh));
	oh.type = OFPTYPE_MULTIPART_REPLY;
	oh.length = htons(16);
	memcpy(send_buf, &oh, sizeof(oh));
	memcpy(send_buf+ sizeof(oh), msg_buf + sizeof(oh), 8);
	send_msg(ofconn, send_buf, 16);
	return 0;
}

uint8_t handle_mp_default(ofp_buf *msg_buf, uint16_t ofconn) {
	struct ofp_error_msg em;
	init_ofp_buf(send_buf);
	memcpy(&oh, msg_buf, sizeof(oh));
	oh.type = OFPTYPE_ERROR;
	oh.length = htons(24);
	em.type = htons(OFPET_BAD_REQUEST);
	em.code = htons(OFPBRC_BAD_MULTIPART);
	memcpy(send_buf, &oh, 8);
	memcpy(send_buf + sizeof(oh), &em, sizeof(em));
	memcpy(send_buf+12, msg_buf, 8);
	send_msg(ofconn, send_buf, 24);
	return 0;
}

uint8_t handle_action_output(fakesw *fksw, uint16_t ofconn, struct ofp_action_output *act, ofp_buf *data_buf, int data_len, uint32_t in_port) {
	struct ether_header eth_header;
	struct ofp_arp_packet_in pi;
    struct arp_header arp_h;
    //static uint8_t stop_log_lldp = 0;
	memcpy(&eth_header, data_buf, sizeof(eth_header));
	if (ntohs(eth_header.ether_type) == 0x88cc) {
		if(ntohl(act->port) != OFPP_CONTROLLER) {
			struct ofp_packet_in pi;
			int dst_sw = ntohl(act->port);
			if (dst_sw <= n_fakesw) {
				if (OFsuite_topo.matrix[fksw->dpid - 1][dst_sw - 1]) {
					pi.oh.version = OFP13_VERSION;
				    pi.oh.type = OFPTYPE_PACKET_IN;
				    pi.oh.length = htons(sizeof(pi) + data_len + 2);
				    pi.oh.xid = htonl(0);
				    pi.buffer_id = htonl(OFP_NO_BUFFER);
				    pi.total_len = htons(data_len);
				    pi.reason = OFPR_ACTION;
				    pi.table_id = 0;
				    pi.cookie = htonll(0);
				    pi.match.type = htons(1);
				    pi.match.length = htons(12);
				    pi.match.oxm_fields[0] = 0x80;
				    pi.match.oxm_fields[1] = 0x00;
				    pi.match.oxm_fields[2] = 0x00;
				    pi.match.oxm_fields[3] = 0x04;
				    pi.match.value = htonl(fksw->dpid);
				    init_ofp_buf(send_buf);
					memcpy(send_buf, &pi, sizeof(pi));
					memset(send_buf + sizeof(pi), 0, 2);
					memcpy(send_buf + sizeof(pi) + 2, data_buf, data_len);
					for(contr_no = 0; contr_no < n_controllers; contr_no++) {
						send_msg(OFsuite_topo.fksw[dst_sw - 1]->sock_fd[contr_no], send_buf, sizeof(pi) + 2 + data_len);
					}
					if (module_log[LOG_LLDP_TIME] || module_log[LOG_LINK_EVENT]) {
						if(total_send == -2) {
							total_send = 0;
							memset(sp, 0, sizeof(sp));
							OFsuite_log(LOG_LLDP_TIME, 0, "start lldp log");
						}
						else if(total_send == -1) {
							link_ch = 1;
							total_send = 0;
							memset(sp, 0, sizeof(sp));
						}
						if (!sp[dst_sw - 1].s_port[fksw->dpid - 1]) {
							sp[dst_sw - 1].s_port[fksw->dpid - 1] = 1;
							sp[dst_sw - 1].total_send_port++;
							if (sp[dst_sw - 1].total_send_port >= (OFsuite_topo.fksw[dst_sw - 1]->n_port - 1)) {
								total_send++;
								//printf("\n%d", dst_sw);
								if(total_send == n_fakesw) {
									if (link_ch) {
										pthread_mutex_lock(&mutx);
										//OFsuite_log(LOG_LINK_EVENT, n_fakesw, "end link event log");
										total_send++;
										pthread_cond_signal(&cond);
										pthread_mutex_unlock(&mutx);
									}
									else {
										pthread_mutex_lock(&mutx);
										OFsuite_log(LOG_LLDP_TIME, n_fakesw, "end lldp log");
										total_send++;
										pthread_cond_signal(&cond_lldp);
										pthread_mutex_unlock(&mutx);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
	else if (ntohs(eth_header.ether_type) == 0x0806) {
		memcpy(&arp_h, data_buf + sizeof(eth_header), sizeof(arp_h));
		if (ntohl(act->port) == OFPP_FLOOD) {
			if(module_log[LOG_PACKET_OUT_RATE]) {
				if(receive_packet_out < send_arp) {
					OFsuite_log(LOG_PACKET_OUT_RATE, ++receive_packet_out, "receive packet out");
					if (all_arp_send && receive_packet_out >= send_arp) {
						pthread_mutex_lock(&mutx);
						pthread_cond_signal(&cond_packet_out);
						pthread_mutex_unlock(&mutx);
					}
				}
			}
			else if(module_log[LOG_FLOW_MOD_RATE]) {
				if(ntohs(arp_h.opt_code) == 1) {
					init_ofp_buf(send_buf);
					gen_arp_reply(&pi, &eth_header, &arp_h, send_buf);
					send_msg(ofconn, send_buf, 84);
					if (send_arp_reply < send_arp && start_send_arp) {
						OFsuite_log(LOG_FLOW_MOD_RATE, ++send_arp_reply, "send arp reply");
					}
				}
				else if (ntohs(arp_h.opt_code) == 2 && ntohl(act->port) == (MAX_SW + 1) && controller != 1) {
					init_ofp_buf(send_buf);
					gen_icmp_ping_request(&pi, &eth_header, send_buf);
					send_msg(ofconn, send_buf, 92);
				}
			}
			
			else if(module_log[LOG_CONNECTIVITY_TEST] || module_log[LOG_DATAPATH_SWITCHOVER] || module_log[LOG_LINK_EVENT]) {
				int i;
				pi.oh.version = OFP13_VERSION;
			    pi.oh.type = OFPTYPE_PACKET_IN;
			    pi.oh.length = htons(sizeof(pi) + data_len + 2);
			    pi.oh.xid = htonl(0);
			    pi.buffer_id = htonl(OFP_NO_BUFFER);
			    pi.total_len = htons(data_len);
			    pi.reason = OFPR_NO_MATCH;
			    pi.table_id = 0;
			    pi.cookie = htonll(0);
			    pi.match.type = htons(1);
			    pi.match.length = htons(12);
			    pi.match.oxm_fields[0] = 0x80;
			    pi.match.oxm_fields[1] = 0x00;
			    pi.match.oxm_fields[2] = 0x00;
			    pi.match.oxm_fields[3] = 0x04;
			    pi.match.value = htonl(fksw->dpid);
			    init_ofp_buf(send_buf);
				memcpy(send_buf, &pi, sizeof(pi));
				memset(send_buf + sizeof(pi), 0, 2);
				memcpy(send_buf + sizeof(pi) + 2, data_buf, data_len);
				for (i = 0; i < n_fakesw; i++) {
					if(OFsuite_topo.matrix[fksw->dpid - 1][i]) {
						//if ((i+1) != in_port && !send_dpid[i]) {
						if ((i+1) != in_port && !conn_test_done) {
							//send_dpid[i] = 1;
							//total_send++;
							//if (total_send <= n_fakesw) {
							for(contr_no = 0; contr_no < n_controllers; contr_no++) {
								send_msg(OFsuite_topo.fksw[i]->sock_fd[contr_no], send_buf, sizeof(pi) + 2 + data_len);
							}
							//}
						}
					}
				}
				if (fksw->dpid == switch_host && ntohs(arp_h.opt_code) == 1) {
					if (!conn_test_done) {
						pi.match.value = htonl(MAX_SW+2);
						init_ofp_buf(send_buf);
						gen_arp_reply(&pi, &eth_header, &arp_h, send_buf);
						send_msg(ofconn, send_buf, 84);
					}
				}
				if (fksw->dpid == 1 && ntohs(arp_h.opt_code) == 2) {
					if (!conn_test_done) {
						pi.match.value = htonl(MAX_SW+1);
						init_ofp_buf(send_buf);
						gen_icmp_ping_request(&pi, &eth_header, send_buf);
						send_msg(ofconn, send_buf, 92);
					}
				}
			}
			else if(module_log[LOG_RECOVERY_TIME]) {
				if(receive_packet_out < send_arp) {
					receive_packet_out++;
					OFsuite_log(LOG_RECOVERY_TIME, ofconn, "receive packet out");
				}
			}
			else if(module_log[LOG_AUXILIARY_CHANNEL] || module_log[LOG_FLOW_MOD_AUXI_RATE]) {
				if(ntohs(arp_h.opt_code) == 1) {
					init_ofp_buf(send_buf);
					gen_arp_reply(&pi, &eth_header, &arp_h, send_buf);
					send_msg(ofconn, send_buf, 84);
				}
				else if (ntohs(arp_h.opt_code) == 2 && ntohl(act->port) == (MAX_SW + 1) && controller != 1) {
					init_ofp_buf(send_buf);
					gen_icmp_ping_request(&pi, &eth_header, send_buf);
					send_msg(ofconn, send_buf, 92);
					send_arp_reply++;
				}
			}
			else {
				return 0;
			}
		}
		else {

			if(module_log[LOG_PACKET_OUT_RATE]) {
				if (receive_packet_out < send_arp) {
					OFsuite_log(LOG_PACKET_OUT_RATE, ++receive_packet_out, "receive packet out");
					if (all_arp_send && receive_packet_out >= send_arp) {
						pthread_mutex_lock(&mutx);
						pthread_cond_signal(&cond_packet_out);
						pthread_mutex_unlock(&mutx);
					}
				}
			}

			else if(module_log[LOG_FLOW_MOD_RATE]) {
				if(ntohs(arp_h.opt_code) == 1) {
					init_ofp_buf(send_buf);
					gen_arp_reply(&pi, &eth_header, &arp_h, send_buf);
					send_msg(ofconn, send_buf, 84);
					if (send_arp_reply < send_arp && start_send_arp) {
						OFsuite_log(LOG_FLOW_MOD_RATE, ++send_arp_reply, "send arp reply");
					}
				}
				
				else if (ntohs(arp_h.opt_code) == 2 && ntohl(act->port) == (MAX_SW + 1) && controller != 1) {
					init_ofp_buf(send_buf);
					gen_icmp_ping_request(&pi, &eth_header, send_buf);
					send_msg(ofconn, send_buf, 92);
				}
			}
			
			else if(module_log[LOG_CONNECTIVITY_TEST] || module_log[LOG_DATAPATH_SWITCHOVER] || module_log[LOG_LINK_EVENT]) {
				int i;
				pi.oh.version = OFP13_VERSION;
			    pi.oh.type = OFPTYPE_PACKET_IN;
			    pi.oh.length = htons(sizeof(pi) + data_len + 2);
			    pi.oh.xid = htonl(0);
			    pi.buffer_id = htonl(OFP_NO_BUFFER);
			    pi.total_len = htons(data_len);
			    pi.reason = OFPR_NO_MATCH;
			    pi.table_id = 0;
			    pi.cookie = htonll(0);
			    pi.match.type = htons(1);
			    pi.match.length = htons(12);
			    pi.match.oxm_fields[0] = 0x80;
			    pi.match.oxm_fields[1] = 0x00;
			    pi.match.oxm_fields[2] = 0x00;
			    pi.match.oxm_fields[3] = 0x04;
			    pi.match.value = htonl(fksw->dpid);
			    init_ofp_buf(send_buf);
				memcpy(send_buf, &pi, sizeof(pi));
				memset(send_buf + sizeof(pi), 0, 2);
				memcpy(send_buf + sizeof(pi) + 2, data_buf, data_len);
				i = ntohl(act->port);
				if (i < (MAX_SW + 1) && i <= n_fakesw && !conn_test_done) {
					//send_dpid[i] = 1;
					//total_send++;
					//if (total_send <= n_fakesw) {
					for(contr_no = 0; contr_no < n_controllers; contr_no++) {
						send_msg(OFsuite_topo.fksw[i - 1]->sock_fd[contr_no], send_buf, sizeof(pi) + 2 + data_len);
					}
					//}
				}
				
				else if (i == (MAX_SW + 1)) {
					if (!conn_test_done) {
						pi.match.value = htonl(MAX_SW+1);
						init_ofp_buf(send_buf);
						gen_icmp_ping_request(&pi, &eth_header, send_buf);
						send_msg(ofconn, send_buf, 92);
					}
				}
				else if(i == MAX_SW + 2) {
					if (!conn_test_done) {
						pi.match.value = htonl(MAX_SW+2);
						init_ofp_buf(send_buf);
						gen_arp_reply(&pi, &eth_header, &arp_h, send_buf);
						send_msg(ofconn, send_buf, 84);
					}
				}
				if(is_datapath_switchover && fksw->dpid == 1 && ntohs(arp_h.opt_code) == 2) {
					if (!conn_test_done) {
						pi.match.value = htonl(MAX_SW+1);
						init_ofp_buf(send_buf);
						gen_icmp_ping_request(&pi, &eth_header, send_buf);
						send_msg(ofconn, send_buf, 92);
					}
				}
			}
			else if(module_log[LOG_RECOVERY_TIME]) {
				if(receive_packet_out < send_arp) {
					receive_packet_out++;
					OFsuite_log(LOG_RECOVERY_TIME, ofconn, "receive packet out");
				}
			}
			else if (module_log[LOG_AUXILIARY_CHANNEL] || module_log[LOG_FLOW_MOD_AUXI_RATE]) {
				if(ntohs(arp_h.opt_code) == 1) {
					init_ofp_buf(send_buf);
					gen_arp_reply(&pi, &eth_header, &arp_h, send_buf);
					send_msg(ofconn, send_buf, 84);
				}
				else if (ntohs(arp_h.opt_code) == 2 && ntohl(act->port) == (MAX_SW + 1) && controller != 1) {
					init_ofp_buf(send_buf);
					gen_icmp_ping_request(&pi, &eth_header, send_buf);
					send_msg(ofconn, send_buf, 92);
					send_arp_reply++;
				}
			}
			else {
				return 0;
			}
		}
	}
	
	else if (ntohs(eth_header.ether_type) == 0x0800) {
		if (module_log[LOG_CONNECTIVITY_TEST] || module_log[LOG_DATAPATH_SWITCHOVER] || module_log[LOG_LINK_EVENT]) {
			int i;
			struct icmp icmp;
			pi.oh.version = OFP13_VERSION;
		    pi.oh.type = OFPTYPE_PACKET_IN;
		    pi.oh.length = htons(sizeof(pi) + data_len + 2);
		    pi.oh.xid = htonl(0);
		    pi.buffer_id = htonl(OFP_NO_BUFFER);
		    pi.total_len = htons(data_len);
		    pi.reason = OFPR_ACTION;
		    pi.table_id = 0;
		    pi.cookie = htonll(0);
		    pi.match.type = htons(1);
		    pi.match.length = htons(12);
		    pi.match.oxm_fields[0] = 0x80;
		    pi.match.oxm_fields[1] = 0x00;
		    pi.match.oxm_fields[2] = 0x00;
		    pi.match.oxm_fields[3] = 0x04;
		    pi.match.value = htonl(fksw->dpid);
		    init_ofp_buf(send_buf);
			memcpy(send_buf, &pi, sizeof(pi));
			memset(send_buf + sizeof(pi), 0, 2);
			memcpy(send_buf + sizeof(pi) + 2, data_buf, data_len);
			memcpy(&icmp, send_buf + sizeof(pi) + 2 + sizeof(eth_header) + sizeof(struct ipv4_header), sizeof(icmp));
			if (ntohl(act->port) != OFPP_FLOOD) {
				i = ntohl(act->port);
				//printf("\nout port: %d\n", i);
				if (i < MAX_SW + 1) {
					if (!conn_test_done) {
						for(contr_no = 0; contr_no < n_controllers; contr_no++) {
							send_msg(OFsuite_topo.fksw[i - 1]->sock_fd[contr_no], send_buf, sizeof(pi) + 2 + data_len);
							//OFsuite_log(LOG_FLOW_MOD_RATE, ++send_arp_reply, "send arp reply");
						}
					}
				}

				else if (i == (MAX_SW + 1)) {
					if (!conn_test_done) {
						pi.match.value = htonl(MAX_SW+1);
						init_ofp_buf(send_buf);
						gen_icmp_ping_request(&pi, &eth_header, send_buf);
						send_msg(ofconn, send_buf, 92);
					}
				}
			
				else if (i == MAX_SW + 2) {
					if (!conn_test_done) {
						pi.match.value = htonl(MAX_SW+2);
						init_ofp_buf(send_buf);
						gen_icmp_ping_reply(&pi, &eth_header, send_buf);
						send_msg(ofconn, send_buf, sizeof(pi) + 2 + data_len);
					}
				}
			}
			else {
				for (i = 0; i < n_fakesw; i++) {
					if(OFsuite_topo.matrix[fksw->dpid - 1][i]) {
						if ((i+1) != in_port && !conn_test_done) {
							for(contr_no = 0; contr_no < n_controllers; contr_no++) {
								send_msg(OFsuite_topo.fksw[i]->sock_fd[contr_no], send_buf, sizeof(pi) + 2 + data_len);
							}
						}
					}
				}
				if (fksw->dpid == switch_host && icmp.type == 8) {
					if (!conn_test_done) {
						pi.match.value = htonl(MAX_SW+2);
						init_ofp_buf(send_buf);
						gen_icmp_ping_reply(&pi, &eth_header, send_buf);
						send_msg(ofconn, send_buf, sizeof(pi) + 2 + data_len);
					}
				}
				if (fksw->dpid == 1 && icmp.type == 0) {
					if (!conn_test_done) {
						pi.match.value = htonl(MAX_SW+1);
						init_ofp_buf(send_buf);
						gen_icmp_ping_request(&pi, &eth_header, send_buf);
						send_msg(ofconn, send_buf, 92);
					}
				}
			}
		}
	}
	/*
	else {
		struct ofp_packet_in pi;
		int dst_sw = ntohl(act->port);
		if (dst_sw <= n_fakesw) {
			if (OFsuite_topo.matrix[fksw->dpid - 1][dst_sw - 1]) {
				pi.oh.version = OFP13_VERSION;
			    pi.oh.type = OFPTYPE_PACKET_IN;
			    pi.oh.length = htons(sizeof(pi) + data_len + 2);
			    pi.oh.xid = htonl(0);
			    pi.buffer_id = htonl(OFP_NO_BUFFER);
			    pi.total_len = htons(data_len);
			    pi.reason = OFPR_ACTION;
			    pi.table_id = 0;
			    pi.cookie = htonll(0);
			    pi.match.type = htons(1);
			    pi.match.length = htons(12);
			    pi.match.oxm_fields[0] = 0x80;
			    pi.match.oxm_fields[1] = 0x00;
			    pi.match.oxm_fields[2] = 0x00;
			    pi.match.oxm_fields[3] = 0x04;
			    pi.match.value = htonl(fksw->dpid);
			    init_ofp_buf(send_buf);
				memcpy(send_buf, &pi, sizeof(pi));
				memset(send_buf + sizeof(pi), 0, 2);
				memcpy(send_buf + sizeof(pi) + 2, data_buf, data_len);
				for(contr_no = 0; contr_no < n_controllers; contr_no++) {
					send_msg(OFsuite_topo.fksw[dst_sw - 1]->sock_fd[contr_no], send_buf, sizeof(pi) + 2 + data_len);
				}
			}
		}
	}*/
	return 0;
}

uint8_t handle_packet_out(ofp_buf *msg_buf, uint16_t ofconn) {
	struct ofp_action_header act_header;
	struct ofp_packet_out packet_out;
	struct ofp_action_output action_output;
	ofp_buf data_buf[MAX_BUF_SIZE];
	int data_len;
	ALL_DONE = 0;
	fakesw *fksw = p_fakesw;
	for(fksw_no = 0; fksw_no < n_fakesw; fksw_no++) {
		for (contr_no = 0; contr_no < n_controllers; contr_no++) {
            if (ofconn == fksw->sock_fd[contr_no])
                flag = 1;
        }
        if (flag) {
            break;
        }
        else
            fksw++;
	}
	if (!flag) {
		uint8_t auxi_no;
		fksw = p_fakesw;
		for(fksw_no = 0; fksw_no < n_fakesw; fksw_no++) {
			for (contr_no = 0; contr_no < n_controllers; contr_no++) {
				if (OFsuite_topo.fksw[fksw_no]->sock_fd[contr_no] == ofconn) {
					flag = 1;
					break;
				}
				for (auxi_no = 0; auxi_no < n_auxiliary_ch; auxi_no++) {
	            	if (ofconn == fksw->auxi[contr_no*n_auxiliary_ch+auxi_no].auxi_fd) {
	                	flag = 1;
	                	break;
	            	}
	            }
	            if (flag)
	            	break;
	        }
	        if (flag) {
	        	break;
	        }
	        fksw++;
		}
	}
	if (flag) {
		flag = 0;
		//if (fksw->dpid == 256) {
		//	printf("\ndpid: %d\n", fksw->dpid);
		//}
		memcpy(&packet_out, msg_buf, sizeof(packet_out));
		memcpy(&act_header, msg_buf + sizeof(packet_out), sizeof(act_header));
		data_len = ntohs(packet_out.header.length) - sizeof(packet_out) - ntohs(act_header.len);
		switch(ntohs(act_header.type)) {
			case OFPAT_OUTPUT:
				memcpy(&action_output, msg_buf + sizeof(packet_out), ntohs(packet_out.actions_len));
				if(data_len) {
					init_ofp_buf(data_buf);
					memcpy(data_buf, msg_buf + sizeof(packet_out) + sizeof(action_output), data_len);
				}
				handle_action_output(fksw, ofconn, &action_output, data_buf, data_len, ntohl(packet_out.in_port));
				break;
			default:
				break;
		}
	}
	ALL_DONE = 1;
	return 0;
}

uint8_t handle_barrier_request(ofp_buf *msg_buf, uint16_t ofconn) {
	while(!ALL_DONE); {
		ALL_DONE = 0;
		init_ofp_buf(send_buf);
		memcpy(&oh, msg_buf, sizeof(oh));
		oh.type = OFPTYPE_BARRIER_REPLY;
		memcpy(send_buf, &oh, sizeof(oh));
		send_msg(ofconn, send_buf, sizeof(oh));
		ALL_DONE = 1;
	}
	return 0;
}

uint8_t handle_role_request(ofp_buf *msg_buf, uint16_t ofconn) {
	struct ofp_role_request ofp_role;
	static int first_role_request = 0;
	ALL_DONE = 0;
	memcpy(&ofp_role, msg_buf, sizeof(ofp_role));
	ofp_role.header.type = OFPTYPE_ROLE_REPLY;
	init_ofp_buf(send_buf);
	if (ofp_role.role == htonl(0)) {
		ofp_role.role = htonl(0x00000001);
		ofp_role.generation_id = htonll(0xffffffffffffffff);
	}
	if(!first_role_request) {
		first_role_request = 1;
		OFsuite_log(LOG_INTIMATION_TIME, 0, "receive first role request");
	}
	else
		OFsuite_log(LOG_INTIMATION_TIME, first_role_request++, "receive role request");
	memcpy(send_buf, &ofp_role, sizeof(ofp_role));
	send_msg(ofconn, send_buf, sizeof(ofp_role));
	ALL_DONE = 1;
	return 0;
}

uint8_t handle_get_config_request(ofp_buf *msg_buf, uint16_t ofconn) {
	struct ofp_switch_config ofp_sc;
	ALL_DONE = 0;
	memcpy(&ofp_sc, msg_buf, sizeof(ofp_sc.header));
	ofp_sc.header.type = OFPTYPE_GET_CONFIG_REPLY;
	ofp_sc.header.length = htons(12);
	ofp_sc.flags = sc_flags;
	ofp_sc.miss_send_len = OFPCML_NO_BUFFER;
	send_msg(ofconn, &ofp_sc, sizeof(ofp_sc));
	ALL_DONE = 1;
	return 0;
}

uint8_t handle_default(ofp_buf *msg_buf, uint16_t ofconn) {
	if(!ALL_DONE) {
		ALL_DONE = 1;
	}
	return 0;
}