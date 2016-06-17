/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */

#include <string.h>
#include <netinet/in.h>
#include <errno.h>
#include "OFsuite_parser.h"
#include "OFsuite_ofpmsgs.h"
#include "OFsuite_log.h"
#include "OFsuite_timer.h"

extern int n_fakesw;
extern uint8_t n_controllers;
extern int SSL_MODE;
extern fakesw *p_fakesw;
extern int receive_echo_reply;

pthread_mutex_t echo_mutx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t echo_cond = PTHREAD_COND_INITIALIZER;

int send_msg(uint16_t sock_fd, void *msg_buf, int msg_len) {
    int length;
    length = send(sock_fd, msg_buf, msg_len, 0);
    return length;
}

int send_hello(uint16_t sock_fd) {
    struct ofp_header ofph;
    ofph.version = OFP13_VERSION;
    ofph.type = OFPTYPE_HELLO;
    ofph.length = htons(8);
    ofph.xid = htonl(0);
    return send_msg(sock_fd, &ofph, 8);
}

int send_echo_request(uint16_t sock_fd) {
    struct ofp_header ofph;
    ofph.version = OFP13_VERSION;
    ofph.type = OFPTYPE_ECHO_REQUEST;
    ofph.length = htons(8);
    ofph.xid = htonl(0);
    return send_msg(sock_fd, &ofph, 8);
}

void send_periodic_echo(int signo) {
    int fksw_no, contr_no;
    struct timespec echo_outtime;
    fakesw *fksw = p_fakesw;
    for (fksw_no = 0; fksw_no < n_fakesw; fksw_no++) {
        for (contr_no = 0; contr_no < n_controllers; contr_no++) {
            send_echo_request(fksw->sock_fd[contr_no]);
        }
        fksw++;
    }
    echo_outtime = set_outtime(10);
    pthread_mutex_lock(&echo_mutx);
    if(!pthread_cond_timedwait(&echo_cond, &echo_mutx, &echo_outtime)) {
        OFsuite_syslog(LV_INFO,"OFsuite_connect","All connections between switch(es) and controller(s) are alive!");
        printf("OFsuite_performance> ");
    }
    else {
        OFsuite_syslog(LV_INFO, "OFsuite_connect", "Waiting for echo reply time out!");
        OFsuite_syslog(LV_INFO, "OFsuite_connect", "Receive %d echo reply!", receive_echo_reply);
        printf("OFsuite_performance> ");
        receive_echo_reply = 0;
    }
    pthread_mutex_unlock(&echo_mutx);
}

int read_msg(uint16_t sock_fd, ofp_buf *buf) {
    int length = 0;
    length = recv(sock_fd, buf, MAX_BUF_SIZE, 0);
    if (length < 0) {
        printf("\nread error no: %d\n", errno);
    }
    else if (length == MAX_BUF_SIZE) {
        printf("\nread full buffer!\n");
    }
    return length;
}

int send_port_status(uint16_t sock_fd, struct ofp_port desc, uint8_t reason) {
    struct ofp_port_status ofp_ps;
    ofp_ps.header.version = OFP13_VERSION;
    ofp_ps.header.type = OFPTYPE_PORT_STATUS;
    ofp_ps.header.length = htons(80);
    ofp_ps.header.xid = htonl(0);
    ofp_ps.reason = reason;
    ofp_ps.desc = desc;
    return send_msg(sock_fd, &ofp_ps, sizeof(ofp_ps));
}

void gen_arp_request(struct ofp_arp_packet_in *pi, struct ether_header *eth_h, struct arp_header *arp_h, ofp_buf *buf) {
    int i;
    init_ofp_buf(buf);
    pi->oh.version = OFP13_VERSION;
    pi->oh.type = OFPTYPE_PACKET_IN;
    pi->oh.length = htons(84);
    pi->oh.xid = htonl(0);
    pi->buffer_id = htonl(OFP_NO_BUFFER);
    pi->total_len = htons(42);
    //pi->reason = OFPR_NO_MATCH;
    pi->reason = OFPR_ACTION;
    pi->table_id = 0;
    pi->cookie = htonll(0);
    pi->match.type = htons(1);
    pi->match.length = htons(12);
    pi->match.oxm_fields[0] = 0x80;
    pi->match.oxm_fields[1] = 0x00;
    pi->match.oxm_fields[2] = 0x00;
    pi->match.oxm_fields[3] = 0x04;
    pi->match.value = htonl(MAX_SW + 1);
    srand(1);
    for (i = 0; i < 6; i++) {
        eth_h->src_mac[i] = rand() % 255;
        eth_h->dst_mac[i] = 0xff;
    }
    eth_h->src_mac[0] |= 0x02;
    eth_h->src_mac[0] &= 0xfe;
    

    eth_h->ether_type = htons(0x0806);
    arp_h->hw_type = htons(1);
    arp_h->pro_type = htons(0x0800);
    arp_h->hw_size = 6;
    arp_h->pro_size = 4;
    arp_h->opt_code = htons(1);
    for (i = 0; i < 6; i++) {
        arp_h->sender_mac[i] = eth_h->src_mac[i];
        //arp_h->target_mac[i] = eth_h->dst_mac[i];
        arp_h->target_mac[i] = 0x00;
    }
    arp_h->sender_ip[0] = 10;
    arp_h->sender_ip[1] = 1;
    arp_h->sender_ip[2] = 1;
    arp_h->sender_ip[3] = 1;

    arp_h->target_ip[0] = 10;
    arp_h->target_ip[1] = 1;
    arp_h->target_ip[2] = 1;
    arp_h->target_ip[3] = 2;

    memcpy(buf, pi, sizeof(struct ofp_arp_packet_in));
    memcpy(buf + sizeof(struct ofp_arp_packet_in) + 2, eth_h, sizeof(struct ether_header));
    memcpy(buf + sizeof(struct ofp_arp_packet_in) + 2 + sizeof(struct ether_header), arp_h, sizeof(struct arp_header));
}

void gen_arp_reply(struct ofp_arp_packet_in *pi, struct ether_header *eth_h, struct arp_header *arp_h, ofp_buf *buf) {
    int i;
    init_ofp_buf(buf);
    pi->oh.version = OFP13_VERSION;
    pi->oh.type = OFPTYPE_PACKET_IN;
    pi->oh.length = htons(84);
    pi->oh.xid = htonl(0);
    pi->buffer_id = htonl(OFP_NO_BUFFER);
    pi->total_len = htons(42);
    //pi->reason = OFPR_NO_MATCH;
    pi->reason = OFPR_ACTION;
    pi->table_id = 0;
    pi->cookie = htonll(0);
    pi->match.type = htons(1);
    pi->match.length = htons(12);
    pi->match.oxm_fields[0] = 0x80;
    pi->match.oxm_fields[1] = 0x00;
    pi->match.oxm_fields[2] = 0x00;
    pi->match.oxm_fields[3] = 0x04;
    pi->match.value = htonl(MAX_SW + 2);
    for (i = 0; i < 6; i++) {
        eth_h->dst_mac[i] = eth_h->src_mac[i];
    }

    srand(2);
    for (i = 0; i < 6; i++) {
        eth_h->src_mac[i] = rand() % 255;
    }
    eth_h->src_mac[0] |= 0x02;
    eth_h->src_mac[0] &= 0xfe;
    
    eth_h->ether_type = htons(0x0806);
    arp_h->hw_type = htons(1);
    arp_h->pro_type = htons(0x0800);
    arp_h->hw_size = 6;
    arp_h->pro_size = 4;
    arp_h->opt_code = htons(2);
    for (i = 0; i < 6; i++) {
        arp_h->sender_mac[i] = eth_h->src_mac[i];
        arp_h->target_mac[i] = eth_h->dst_mac[i];
    }
    arp_h->sender_ip[0] = 10;
    arp_h->sender_ip[1] = 1;
    arp_h->sender_ip[2] = 1;
    arp_h->sender_ip[3] = 2;

    arp_h->target_ip[0] = 10;
    arp_h->target_ip[1] = 1;
    arp_h->target_ip[2] = 1;
    arp_h->target_ip[3] = 1;

    memcpy(buf, pi, sizeof(struct ofp_arp_packet_in));
    memcpy(buf + sizeof(struct ofp_arp_packet_in) + 2, eth_h, sizeof(struct ether_header));
    memcpy(buf + sizeof(struct ofp_arp_packet_in) + 2 + sizeof(struct ether_header), arp_h, sizeof(struct arp_header));
}

void gen_icmp_ping_request(struct ofp_arp_packet_in *pi, struct ether_header *eth_h, ofp_buf *buf) {
    uint8_t i,addr;
    struct ipv4_header ip_h;
    struct icmp icmp;
    init_ofp_buf(buf);
    pi->oh.version = OFP13_VERSION;
    pi->oh.type = OFPTYPE_PACKET_IN;
    pi->oh.length = htons(92);
    pi->oh.xid = htonl(0);
    pi->buffer_id = htonl(OFP_NO_BUFFER);
    pi->total_len = htons(50);
    //pi->reason = OFPR_NO_MATCH;
    pi->reason = OFPR_ACTION;
    pi->table_id = 0;
    pi->cookie = htonll(0);
    pi->match.type = htons(1);
    pi->match.length = htons(12);
    pi->match.oxm_fields[0] = 0x80;
    pi->match.oxm_fields[1] = 0x00;
    pi->match.oxm_fields[2] = 0x00;
    pi->match.oxm_fields[3] = 0x04;
    pi->match.value = htonl(MAX_SW + 1);
    for(i = 0; i < 6 ; i++) {
        addr = eth_h->dst_mac[i];
        eth_h->dst_mac[i] = eth_h->src_mac[i];
        eth_h->src_mac[i] = addr;
    }
    eth_h->ether_type = htons(0x0800);
    ip_h.version_length = 0x45;
    ip_h.dsf = 0;
    ip_h.total_length = htons(36);
    ip_h.identification = htonl(0);
    ip_h.f_offset = htonl(0x4000);
    ip_h.time_to_live = 255;
    ip_h.protocol = 1;
    ip_h.checksum = htons(0);
    ip_h.source[0] = 10;
    ip_h.source[1] = 1;
    ip_h.source[2] = 1;
    ip_h.source[3] = 1;
    ip_h.destination[0] = 10;
    ip_h.destination[1] = 1;
    ip_h.destination[2] = 1;
    ip_h.destination[3] = 2;
    icmp.type = 8;
    icmp.code = 0;
    icmp.checksum = htons(0xbb84);
    icmp.identifier = 0;
    icmp.sequence_number = htons(1);
    memcpy(buf, pi, sizeof(struct ofp_arp_packet_in));
    memcpy(buf + sizeof(struct ofp_arp_packet_in) + 2, eth_h, sizeof(struct ether_header));
    memcpy(buf + sizeof(struct ofp_arp_packet_in) + 2 + sizeof(struct ether_header), &ip_h, sizeof(ip_h));
    memcpy(buf + sizeof(struct ofp_arp_packet_in) + 2 + sizeof(struct ether_header) + sizeof(ip_h), &icmp, sizeof(icmp));
}
void gen_icmp_ping_reply(struct ofp_arp_packet_in *pi, struct ether_header *eth_h, ofp_buf *buf) {
    uint8_t i,addr;
    struct ipv4_header ip_h;
    struct icmp icmp;
    init_ofp_buf(buf);
    pi->oh.version = OFP13_VERSION;
    pi->oh.type = OFPTYPE_PACKET_IN;
    pi->oh.length = htons(92);
    pi->oh.xid = htonl(0);
    pi->buffer_id = htonl(OFP_NO_BUFFER);
    pi->total_len = htons(50);
    //pi->reason = OFPR_NO_MATCH;
    pi->reason = OFPR_ACTION;
    pi->table_id = 0;
    pi->cookie = htonll(0);
    pi->match.type = htons(1);
    pi->match.length = htons(12);
    pi->match.oxm_fields[0] = 0x80;
    pi->match.oxm_fields[1] = 0x00;
    pi->match.oxm_fields[2] = 0x00;
    pi->match.oxm_fields[3] = 0x04;
    pi->match.value = htonl(MAX_SW + 2);
    for(i = 0; i < 6 ; i++) {
        addr = eth_h->dst_mac[i];
        eth_h->dst_mac[i] = eth_h->src_mac[i];
        eth_h->src_mac[i] = addr;
    }
    eth_h->ether_type = htons(0x0800);
    ip_h.version_length = 0x45;
    ip_h.dsf = 0;
    ip_h.total_length = htons(36);
    ip_h.identification = htons(0);
    ip_h.f_offset = htons(0x4000);
    ip_h.time_to_live = 255;
    ip_h.protocol = 1;
    ip_h.checksum = htons(0);
    ip_h.source[0] = 10;
    ip_h.source[1] = 1;
    ip_h.source[2] = 1;
    ip_h.source[3] = 2;
    ip_h.destination[0] = 10;
    ip_h.destination[1] = 1;
    ip_h.destination[2] = 1;
    ip_h.destination[3] = 1;
    icmp.type = 0;
    icmp.code = 0;
    icmp.checksum = htons(0xc384);
    icmp.identifier = htons(0);
    icmp.sequence_number = htons(0x0001);
    
    memcpy(buf, pi, sizeof(struct ofp_arp_packet_in));
    memcpy(buf + sizeof(struct ofp_arp_packet_in) + 2, eth_h, sizeof(struct ether_header));
    memcpy(buf + sizeof(struct ofp_arp_packet_in) + 2 + sizeof(struct ether_header), &ip_h, sizeof(ip_h));
    memcpy(buf + sizeof(struct ofp_arp_packet_in) + 2 + sizeof(struct ether_header) + sizeof(ip_h), &icmp, sizeof(icmp));
}

uint16_t checksum(uint16_t *buffer, int size) {
    unsigned long cksum = 0;
    while(size>1)
    {
        cksum += *buffer++;
        size -= sizeof(uint16_t);
    }
    if(size)
    {
        cksum += *(uint16_t*)buffer;
    }
    cksum = (cksum>>16) + (cksum&0xffff); 
    cksum += (cksum>>16); 
    return (uint16_t)(~cksum);
}