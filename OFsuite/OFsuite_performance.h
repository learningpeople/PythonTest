/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */

#ifndef OFSUITE_PERFORMANCE_H
#define OFSUITE_PERFORMANCE_H 1

#include <stdio.h>
#include <sys/epoll.h>
#include <stdint.h>
#include <fcntl.h>
#include <pthread.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <signal.h>
#include "src/OFsuite_ofpmsgs.h"
#include "src/flow.h"

#define MAX_EVENTS 20000
#define MAX_SW 20000
#define MAX_PORTS 2000
#define MAX_CONTROLLERS 8
#define MAX_AUXILIARY 8
#define MAX_ADDR_LEN 480

struct fksw_table {
    uint8_t t_id;
};

struct p_port {
	struct ofp_port port;
	struct p_port *next;
};

struct auxi_chan {
    uint16_t auxi_fd;
    uint8_t auxi_id;
};

typedef struct OFsuite_fakesw {
    uint16_t sock_fd[MAX_CONTROLLERS];
    struct auxi_chan auxi[MAX_AUXILIARY * MAX_CONTROLLERS];
    SSL *ssl[MAX_CONTROLLERS];
    uint16_t dpid;
    uint16_t n_port;
    struct p_port *pport;
    uint8_t n_table;
    struct fksw_table *table;
    struct flows* flows;
    uint8_t is_enabled;
}fakesw;

struct fksw_auxi {
    fakesw *fksw;
    uint8_t auxi_id;
};

void init_fksw_flows(fakesw* fksw);
void init_controller(void);
void init_fakesw_port(fakesw *fksw, int fksw_no);
struct p_port  *add_fakesw_port(fakesw *fksw, int fksw_no);
int fakesw_create_connect(void);
int add_fakesw(int);
void init_recv_event(void *arg);
void init_recv_event_thread(void);
void update_fakesw_topology(uint8_t topo_mode);
void set_arp_packet_in(uint64_t rate);
int set_port_down(void);
int send_conn_arp(uint16_t sw_id);
int set_new_switch(void);
fakesw *get_switch_from_sockfd(uint16_t socket_fd);
void get_switch_from_auxifd(struct fksw_auxi *fk_au, uint16_t auxi_fd);
int get_controller_from_sockfd(uint16_t sock_fd);
void OFsuite_exit(void);
#endif