/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */

#define _GNU_SOURCE
#include "OFsuite_performance.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "src/OFsuite_socket.h"
#include "src/OFsuite_parser.h"
#include "src/OFsuite_ofpmsgs.h"
#include "src/OFsuite_log.h"
#include "src/OFsuite_timer.h"
#include "src/OFsuite_tokenbucket.h"
#include "src/OFsuite_buffer.h"
#include "src/OFsuite_topology.h"
#include "src/OFsuite_link_connectivity.h"
#include "src/OFsuite_ssl.h"
#include "src/flow.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <python2.7/Python.h>

extern struct token_bucket tb;
extern FILE *OFsuite_logfile;
extern uint16_t n_spine;
extern uint8_t module_log[32];
fakesw fake_sw[MAX_SW];
fakesw *p_fakesw = NULL;
topology OFsuite_topo;
ofp_buf recv_buf[MAX_BUF_SIZE] = {0};
uint8_t topo_mod = 0;
uint8_t send_arp_done = 0;
uint8_t all_arp_send = 0;
uint8_t stop_send_arp = 0;
int total_send = -2;
uint16_t send_dpid[MAX_SW] = {0};
int epfd = 0;
struct timespec outtime;
static struct epoll_event event;
static struct epoll_event *events;
int n_fakesw = 0;
int ON_EXIT = 0;
char addr[MAX_CONTROLLERS][16] = {{0}};
uint16_t port = 0;
struct sockaddr_in controller[MAX_CONTROLLERS];
uint8_t n_controllers = 0;
uint8_t n_auxiliary_ch = 0;
uint8_t thread_done = 0;
uint8_t start_send_arp = 0;
uint8_t is_datapath_switchover = 0;
uint8_t is_datapath_failure = 0;
pthread_mutex_t mutx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_lldp = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_switchover = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_packet_out = PTHREAD_COND_INITIALIZER;
struct send_port sp[MAX_SW] = {{{0}, 0}};
uint64_t arp_rate = 0;
uint16_t switch_host = 0;
uint64_t send_arp = 0;
uint8_t start_conn_test = 0;
uint8_t conn_test_done = 0;
char Project_Dir[256] = {0};
char Log_Dir[256] = {0};
char Python_Dir[256] = {0};
int SSL_MODE = 0;

void init_fksw_flows(fakesw* fksw){
    fksw->flows = init_flows();
}

void init_controller(void) {
    uint8_t i;
    OFsuite_syslog(LV_INFO, "OFsuite_init", "Setting address(es) of controller(s)...");
    OFsuite_syslog(LV_INFO, "OFsuite_init", "Total %d controller(s)", n_controllers);
    for(i = 0; i < n_controllers; i++) {
        memset(&controller[i], 0, sizeof(controller[i]));
        controller[i].sin_family = AF_INET;
        controller[i].sin_addr.s_addr = inet_addr(addr[i]);
        controller[i].sin_port = htons(port);
        OFsuite_syslog(LV_INFO,"OFsuite_init", "controller address: %s", addr[i]);
    }
    OFsuite_syslog(LV_INFO, "OFsuite_init", "controller port: %d", port);
    OFsuite_syslog(LV_INFO, "OFsuite_init", "setting address(es) of controller(s) done!");
}

void init_fakesw_port(fakesw *fksw, int fksw_no) {
    int i;
    struct ofp_port sw;
    sw.port_no = htonl(0xfffffffe);
    for (i = 0; i < OFP_ETH_ALEN; i++) {
        sw.hw_addr[i] = 0xff;
    }

    if (fksw_no < 256){
        sw.hw_addr[OFP_ETH_ALEN-1] = 255 - fksw_no;
    }
    else {
        sw.hw_addr[OFP_ETH_ALEN-1] = 255 - (fksw_no % 256);
        sw.hw_addr[OFP_ETH_ALEN-2] = 255 - (fksw_no / 256);
    }
    sprintf(sw.name, "sw-%d", fksw_no + 1);
    sw.config = htonl(0x00000001);
    sw.state = htonl(0x00000001);
    sw.curr = htonl(0);
    sw.advertised = htonl(0);
    sw.supported = htonl(0);
    sw.peer = htonl(0);
    sw.curr_speed = htonl(0);
    sw.max_speed = htonl(0);

    fksw->pport = (struct p_port *)malloc(sizeof(struct p_port));
    fksw->pport->port = sw;
    fksw->pport->next = NULL;
}

struct p_port *add_fakesw_port(fakesw *fksw, int port_no) {
    int i;
    struct p_port *pport;
    struct ofp_port port;
    port.port_no = htonl(port_no + 1);
    for (i = 0; i < OFP_ETH_ALEN; i++) {
        port.hw_addr[i] = 0x00;
    }
    if (port_no < 256){
        port.hw_addr[OFP_ETH_ALEN-1] = port_no + 1;
        port.hw_addr[OFP_ETH_ALEN-3] = fksw->dpid;
    }
    else {
        port.hw_addr[OFP_ETH_ALEN-1] = (port_no % 256);
        port.hw_addr[OFP_ETH_ALEN-2] = (port_no / 256);
        port.hw_addr[OFP_ETH_ALEN-3] = (fksw->dpid % 256);
        port.hw_addr[OFP_ETH_ALEN-4] = (fksw->dpid / 256);
    }
    memset(port.name, 0, sizeof(port.name));
    sprintf(port.name, "sw-%d:p-%d", fksw->dpid, port_no + 1);
    port.config = htonl(0);
    port.state = htonl(0);
    port.curr = htonl(0x00000840);
    port.advertised = htonl(0);
    port.supported = htonl(0);
    port.peer = htonl(0);
    port.curr_speed = htonl(10000000);
    port.max_speed = htonl(0);
    pport = fksw->pport;
    while(pport->next != NULL) {
        pport = pport->next;
    }
    pport->next = (struct p_port *)malloc(sizeof(struct p_port));
    pport->next->port = port;
    pport->next->next = NULL;
    fksw->n_port++;
    return pport->next;
}

int fakesw_create_connect() {
    int fksw_no;
    uint32_t i,j;
    double start_time, end_time;
    //SSL_CTX *ctx;
    int no_delay = 1;
    //uint64_t tcp_win = 12582912;
    fakesw *fksw;
    p_fakesw = fake_sw;
    fksw = p_fakesw;

    epfd = epoll_create(n_fakesw * n_controllers * (n_auxiliary_ch + 1));
    if (epfd == -1) {
        OFsuite_syslog(LV_ERROR, "OFsuite_fakesw", "Epoll Create error");
    }
    memset(&event, 0, sizeof(event));
    OFsuite_syslog(LV_INFO,"OFsuite_fakesw", "Creating switch(es) and connecting to controller(s)...");
    OFsuite_syslog(LV_INFO,"OFsuite_fakesw", "Total %d switch(es)", n_fakesw);
    start_time = get_msec();
    for (fksw_no = 0; fksw_no < n_fakesw; fksw_no++) {
        
        //ctx = InitCTX();
        //SSL_CTX_use_PrivateKey_file(ctx, "/home/bii/workspace/test_tool/OFsuite_performance/sc-privkey.pem", SSL_FILETYPE_PEM);
        //SSL_CTX_use_certificate_file(ctx, "/usr/local/var/lib/openvswitch/pki/switchca/cacert.pem", SSL_FILETYPE_PEM);
        //if (ctx == NULL) 
        //{
        //    ERR_print_errors_fp(stdout);
        //    exit(1);
        //}
        for (i = 0; i < n_controllers; i++) {
            if ((fksw->sock_fd[i] = socket_create()) < 0) {
                OFsuite_syslog(LV_ERROR, "OFsuite_fakesw","Failed to create switch %d!", fksw_no + 1);
                exit(1);
            }
            if( (setsockopt( fksw->sock_fd[i], SOL_TCP, TCP_NODELAY,
                   (char *)&no_delay, sizeof(no_delay))) < 0) {
                OFsuite_syslog(LV_WARN,"OFsuite_fakesw", "Failed to set option of switch %d's socket", fksw_no+1);
            }
            /*
            if( (setsockopt( fksw->sock_fd[i], SOL_SOCKET, SO_RCVBUF, (char *)&tcp_win, sizeof(tcp_win))) < 0) {
                printf("error\n");
            }
            if( (setsockopt( fksw->sock_fd[i], SOL_SOCKET, SO_SNDBUF, (char *)&tcp_win, sizeof(tcp_win))) < 0) {
                printf("error\n");
            }*/
            
            event.data.fd = fksw->sock_fd[i];
            event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;

            int flag = epoll_ctl(epfd, EPOLL_CTL_ADD, fksw->sock_fd[i], &event);
            if (flag == -1) {
               OFsuite_syslog(LV_ERROR, "OFsuite_fakesw","Failed to add switch %d's socket into epoll!", fksw_no + 1);
                break;
            }
            
            if ((socket_connect(fksw->sock_fd[i], (struct sockaddr *)&controller[i])) < 0) {
                OFsuite_syslog(LV_ERROR,"OFsuite_fakesw","Faile to connect switch %d to its controller!", fksw_no + 1);
                exit(1);
            }
            
            if ((make_socket_non_blocking(fksw->sock_fd[i])) < 0) {
                OFsuite_syslog(LV_WARN,"OFsuite_fakesw","Failed to make switch %d's socket none blocking!", fksw_no + 1);
                exit(1);
            }
            
            for (j = 0; j < n_auxiliary_ch; j++) {
                if ((fksw->auxi[i*n_auxiliary_ch+j].auxi_fd = socket_create()) < 0) {
                    OFsuite_syslog(LV_ERROR, "OFsuite_fakesw","Failed to create switch %d!", fksw_no + 1);
                    exit(1);
                }
                if( (setsockopt( fksw->auxi[i*n_auxiliary_ch+j].auxi_fd, SOL_TCP, TCP_NODELAY,
                       (char *)&no_delay, sizeof(no_delay) )) < 0) {
                    OFsuite_syslog(LV_WARN,"OFsuite_fakesw","Failed to set option of switch %d's socket", fksw_no+1);
                } 
                event.data.fd = fksw->auxi[i*n_auxiliary_ch+j].auxi_fd;
                event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;

                int flag = epoll_ctl(epfd, EPOLL_CTL_ADD, fksw->auxi[i*n_auxiliary_ch+j].auxi_fd, &event);
                if (flag == -1) {
                    OFsuite_syslog(LV_ERROR, "OFsuite_fakesw", "Failed to add switch %d's socket into epoll!", fksw_no + 1);
                    break;
                }
                
                if ((socket_connect(fksw->auxi[i*n_auxiliary_ch+j].auxi_fd, (struct sockaddr *)&controller[i])) < 0) {
                    OFsuite_syslog(LV_ERROR, "OFsuite_fakesw", "Failed to connect switch %d to its controller!", fksw_no + 1);
                    exit(1);
                }
                
                if ((make_socket_non_blocking(fksw->auxi[i*n_auxiliary_ch+j].auxi_fd)) < 0) {
                    OFsuite_syslog(LV_WARN, "OFsuite_fakesw","Failed to make switch %d's socket none blocking!", fksw_no + 1);
                    exit(1);
                }
                fksw->auxi[i*n_auxiliary_ch+j].auxi_id = j + 1;
            }
            /*
            if (SSL_MODE) {
                int error = 0;
                fksw->ssl[i] = SSL_new(ctx);
                SSL_set_fd(fksw->ssl[i], fksw->sock_fd[i]);
                error = SSL_connect(fksw->ssl[i]);
                ShowCerts(fksw->ssl[i]);
                printf("\nssl connect error: %d", error);
                printf("\nssl error: %d", SSL_get_error(fksw->ssl[i], error));
            }
            */
        }
        fksw->dpid = fksw_no + 1;
        init_fakesw_port(fksw, fksw_no);
        fksw->n_port = 1;
        fksw->table = NULL;
        init_fksw_flows(fksw);
        fksw->is_enabled = 1;
        OFsuite_topo.fksw[fksw_no] = fksw;
        fksw++;
    }
    end_time = get_msec();
    OFsuite_syslog(LV_INFO,"OFsuite_fakesw","OpenFlow Channel Establishment time: %f ms", end_time - start_time);

    for(i = 0; i < n_fakesw; i++) {
        for(j = 0; j < n_fakesw; j++) {
            OFsuite_topo.matrix[i][j] = 0;
        }
    }
    create_topology(n_fakesw, &OFsuite_topo, topo_mod);
    for(i = 0; i < n_fakesw; i++) {
        for(j = 0; j < n_fakesw; j++) {
            if(OFsuite_topo.matrix[i][j])
                add_fakesw_port(OFsuite_topo.fksw[i], j);
        }
    }
    init_recv_event_thread();
    
    fksw = p_fakesw;
    for (fksw_no = 0; fksw_no < n_fakesw; fksw_no++) {
        for (i = 0; i < n_controllers; i++) {
            send_hello(fksw->sock_fd[i]);
            usleep(1000);
            for(j = 0; j < n_auxiliary_ch; j++) {
                send_hello(fksw->auxi[i*n_auxiliary_ch+j].auxi_fd);
                usleep(1000);
            }
        }
        fksw++;
    }
    fksw = NULL;
    return 0;
}

void update_fakesw_topology(uint8_t topo) {
    uint32_t i, j, k;
    int error = 0;
    struct p_port *pport;
    if (topo_mod) {
        OFsuite_syslog(LV_WARN, "OFsuite_topology", "Topology already exists, Do nothing!");
    }
    else {
        topo_mod = topo;
        create_topology(n_fakesw, &OFsuite_topo, topo_mod);
        for(i = 0; i < n_fakesw; i++) {
            for(j = 0; j < n_fakesw; j++) {
                if(OFsuite_topo.matrix[i][j]) {
                    pport = add_fakesw_port(OFsuite_topo.fksw[i], j);
                    for (k = 0; k < n_controllers; k++) {
                        error = send_port_status(OFsuite_topo.fksw[i]->sock_fd[k], pport->port, OFPPR_ADD);
                        if(error < 0)
                            OFsuite_syslog(LV_WARN, "OFsuite_topology","Failed to send switch %d's port status!", i+1);
                    }
                }
            }
        }
        if (module_log[LOG_LLDP_TIME] || module_log[LOG_LINK_EVENT]) {
            OFsuite_syslog(LV_INFO, "OFsuite_topology","Waiting for topology discovery...");
            outtime = set_outtime(30);
            pthread_mutex_lock(&mutx);
            if(!pthread_cond_timedwait(&cond_lldp, &mutx, &outtime)) {
                OFsuite_syslog(LV_INFO,"OFsuite_topology","Topology discovery done!");
            }
            else
                OFsuite_syslog(LV_WARN,"OFsuite_topology","Topology discovery time out!");
            pthread_mutex_unlock(&mutx);
        }
    }
}

int add_fakesw(int n_sw) {
    uint16_t fksw_no;
    int i, j;
    struct p_port *pport;
    uint8_t con_no;
    uint8_t auxi_no;
    fakesw *fksw;
    int cur_nsw = n_fakesw;
    uint64_t so = 12582912;
    n_fakesw += n_sw;
    fksw = &fake_sw[cur_nsw];
    OFsuite_topo.sw_num = n_fakesw;
    for(fksw_no = 0; fksw_no < n_sw; fksw_no++) {
        for(con_no = 0; con_no < n_controllers; con_no++) {
            if ((fksw->sock_fd[con_no] = socket_create()) < 0) {
                OFsuite_syslog(LV_ERROR, "OFsuite_fakesw", "Failed to create switch %d!", fksw_no + cur_nsw + 1);
                return ERROR_CREATESW;
                //exit(1);
            }
            if( (setsockopt( fksw->sock_fd[con_no], SOL_TCP, TCP_NODELAY, (char *)&so, sizeof(so) )) < 0) {
                OFsuite_syslog(LV_WARN, "OFsuite_fakesw","Failed to set switch %d's socket option", fksw_no+cur_nsw+1);
            }

            event.data.fd = fksw->sock_fd[con_no];
            event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;

            int flag = epoll_ctl(epfd, EPOLL_CTL_ADD, fksw->sock_fd[con_no], &event);
            if (flag == -1) {
                OFsuite_syslog(LV_ERROR,"OFsuite_fakesw","Failed to add switch %d's socket into epoll!\n", fksw_no + cur_nsw + 1);
                return ERROR_EPOLL;
                //exit(1);
            }

            if ((socket_connect(fksw->sock_fd[con_no], (struct sockaddr *)&controller[con_no])) < 0) {
                OFsuite_syslog(LV_ERROR,"OFsuite_fakesw","Failed to connect switch %d to its controller!\n", fksw_no + cur_nsw + 1);
                cur_nsw += fksw_no;
                return ERROR_CONNECT_CONTROLLER;
                //exit(1);
            }
            
            if ((make_socket_non_blocking(fksw->sock_fd[con_no])) < 0) {
                OFsuite_syslog(LV_WARN,"OFsuite_fakesw","Failed to make switch %d's socket none blocking!\n", fksw_no + cur_nsw + 1);
                return ERROR_MAKE_SOCKET_NO_BLOCKING;
                //exit(1);
            }
            for (j = 0; j < n_auxiliary_ch; j++) {
                if ((fksw->auxi[con_no*n_auxiliary_ch+j].auxi_fd = socket_create()) < 0) {
                    OFsuite_syslog(LV_ERROR,"OFsuite_fakesw","Failed to create switch %d!", fksw_no + cur_nsw + 1);
                    exit(1);
                }
                if( (setsockopt( fksw->auxi[con_no*n_auxiliary_ch+j].auxi_fd, SOL_TCP, TCP_NODELAY, (char *)&so, sizeof(so) )) < 0) {
                    OFsuite_syslog(LV_WARN, "OFsuite_fakesw","Failed to set switch %d's socket option!", fksw_no+cur_nsw+1);
                } 
                event.data.fd = fksw->auxi[con_no*n_auxiliary_ch+j].auxi_fd;
                event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;

                int flag = epoll_ctl(epfd, EPOLL_CTL_ADD, fksw->auxi[con_no*n_auxiliary_ch+j].auxi_fd, &event);
                if (flag == -1) {
                    OFsuite_syslog(LV_ERROR, "OFsuite_fakesw","Failed to add switch %d's socket into epoll!", fksw_no + cur_nsw + 1);
                    break;
                }
                
                if ((socket_connect(fksw->auxi[con_no*n_auxiliary_ch+j].auxi_fd, (struct sockaddr *)&controller[con_no])) < 0) {
                    OFsuite_syslog(LV_ERROR,"OFsuite_fakesw","Failed to connect switch %d to its controller!", fksw_no + cur_nsw + 1);
                    exit(1);
                }
                
                if ((make_socket_non_blocking(fksw->auxi[con_no*n_auxiliary_ch+j].auxi_fd)) < 0) {
                    OFsuite_syslog(LV_WARN,"OFsuite_fakesw","Failed to make switch %d's socket none blocking!", fksw_no + cur_nsw + 1);
                    exit(1);
                }
                fksw->auxi[con_no*n_auxiliary_ch+j].auxi_id = j + 1;
            }
        }
        fksw->dpid = fksw_no + cur_nsw + 1;
        init_fakesw_port(fksw, fksw_no + cur_nsw);
        fksw->n_port = 1;
        fksw->table = NULL;
        init_fksw_flows(fksw);
        fksw->is_enabled = 1;
        OFsuite_topo.fksw[fksw_no + cur_nsw] = fksw;
        fksw++;
    }
    OFsuite_syslog(LV_INFO,"OFsuite_fakesw","Adding %d switches...", n_sw);
    update_topology(cur_nsw, n_sw, &OFsuite_topo, topo_mod);
    if (topo_mod == TOPO_LINEAR) {
        for(i = 0; i <= n_sw; i++) {
            for(j = 0; j <= n_sw; j++) {
                if(OFsuite_topo.matrix[i + cur_nsw - 1][j + cur_nsw - 1])
                    add_fakesw_port(OFsuite_topo.fksw[i + cur_nsw - 1], j + cur_nsw - 1);
            }
        }
        for(fksw_no = cur_nsw; fksw_no < cur_nsw + n_sw; fksw_no++) {
            if (OFsuite_topo.matrix[cur_nsw - 1][fksw_no]) {
                pport = OFsuite_topo.fksw[cur_nsw - 1]->pport;
                while(ntohl(pport->port.port_no) != fksw_no + 1 &&  pport != NULL) {
                    pport = pport->next;
                }
                if (pport != NULL) {
                    for(con_no = 0; con_no < n_controllers; con_no++) {
                        send_port_status(OFsuite_topo.fksw[cur_nsw - 1]->sock_fd[con_no], pport->port, OFPPR_ADD);
                    }
                }
            }
        }
    }
    else if (topo_mod == TOPO_RING) {
        pport = OFsuite_topo.fksw[0]->pport;
        while(ntohl(pport->port.port_no) != cur_nsw &&  pport != NULL) {
            pport = pport->next;
        }
        if (pport != NULL) {
            for(con_no = 0; con_no < n_controllers; con_no++) {
                send_port_status(OFsuite_topo.fksw[0]->sock_fd[con_no], pport->port, OFPPR_DELETE);
            }
            pport->port.port_no = htonl(cur_nsw + n_sw);
            sprintf(pport->port.name, "sw-1:port-%d", cur_nsw + n_sw);
            for(con_no = 0; con_no < n_controllers; con_no++) {
                send_port_status(OFsuite_topo.fksw[0]->sock_fd[con_no], pport->port, OFPPR_ADD);
            }
        }
        pport = OFsuite_topo.fksw[cur_nsw - 1]->pport;
        while(ntohl(pport->port.port_no) != 1 &&  pport != NULL) {
            pport = pport->next;
        }
        if (pport != NULL) {
            for(con_no = 0; con_no < n_controllers; con_no++) {
                send_port_status(OFsuite_topo.fksw[cur_nsw - 1]->sock_fd[con_no], pport->port, OFPPR_DELETE);
            }
            pport->port.port_no = htonl(cur_nsw + 1);
            sprintf(pport->port.name, "sw-%d:port-%d", cur_nsw, cur_nsw + 1);
            for(con_no = 0; con_no < n_controllers; con_no++) {
                send_port_status(OFsuite_topo.fksw[cur_nsw - 1]->sock_fd[con_no], pport->port, OFPPR_ADD);
            }
        }
        for(i = 0; i < n_sw; i++) {
            for(j = 0; j < cur_nsw + n_sw; j++) {
                if(OFsuite_topo.matrix[i + cur_nsw][j])
                    add_fakesw_port(OFsuite_topo.fksw[i + cur_nsw], j);
            }
        }
    }
    else if (topo_mod == TOPO_FULL_MESH) {

        for(i = 0; i < cur_nsw; i++) {
            for (j = 0; j < n_sw; j++) {
                add_fakesw_port(OFsuite_topo.fksw[i], cur_nsw + j);
                pport = OFsuite_topo.fksw[i]->pport;
                while(ntohl(pport->port.port_no) != cur_nsw + j + 1 &&  pport != NULL) {
                    pport = pport->next;
                }
                if (pport != NULL) {
                    for(con_no = 0; con_no < n_controllers; con_no++) {
                        send_port_status(OFsuite_topo.fksw[i]->sock_fd[con_no], pport->port, OFPPR_ADD);
                    }
                }
            }
        }
        for (i = 0; i < n_sw; i++) {
            for (j = 0; j < cur_nsw + n_sw; j++) {
                if (OFsuite_topo.matrix[i + cur_nsw][j])
                    add_fakesw_port(OFsuite_topo.fksw[i + cur_nsw], j);
            }
        }
    }
    else if (topo_mod == TOPO_LEAF_SPINE) {
        for (i = 0; i < n_spine; i++) {
            for (j = 0; j < n_sw; j++) {
                pport = add_fakesw_port(OFsuite_topo.fksw[i], j + cur_nsw);
                for(con_no = 0; con_no < n_controllers; con_no++) {
                    send_port_status(OFsuite_topo.fksw[i]->sock_fd[con_no], pport->port, OFPPR_ADD);
                }
            }
        }
        for (i = 0; i < n_sw; i++) {
            for (j = 0; j < n_spine; j++) {
                add_fakesw_port(OFsuite_topo.fksw[i + cur_nsw], j);
            }
        }
    }
    fksw = &fake_sw[cur_nsw];
    for(fksw_no = 0; fksw_no < n_sw; fksw_no++) {
        for(con_no = 0; con_no < n_controllers; con_no++) {
            send_hello(fksw->sock_fd[con_no]);
            usleep(1000);
            for(auxi_no = 0; auxi_no < n_auxiliary_ch; auxi_no++) {
                send_hello(fksw->auxi[con_no*n_auxiliary_ch+auxi_no].auxi_fd);
                usleep(1000);
            }
        }
        fksw++;
    }
    OFsuite_syslog(LV_INFO, "OFsuite_fakesw", "Adding switches done, now total %d switches", n_fakesw);
    fksw = NULL;
    return 0;
}

void init_recv_event(void *arg) {
    init_ofp_buf(recv_buf);
    int i;
    int fd = 0;
    int length = 0;
    sigset_t sigset;
    events = (struct epoll_event *)malloc(MAX_EVENTS * sizeof(event));
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGALRM);
    //sigaddset(&sigset, SIGPIPE);
    if (pthread_sigmask(SIG_BLOCK, &sigset, NULL) < 0) {
        OFsuite_syslog(LV_ERROR, "OFsuite_recv_event", "Failed to set sigprocmask!");
        exit(-1);
    }
    set_timer_sec(60);
    signal(SIGALRM, send_periodic_echo);
    while (!ON_EXIT) {
        fd = 0;
        length = 0;
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, 5000);
        if (nfds < 0) {
            OFsuite_syslog(LV_WARN, "OFsuite_recv_event", "Error occured in epoll wait!");
        }
        
        //else if (nfds == 0) {
        //    OFsuite_syslog(LV_INFO, "OFsuite_recv_event", "No OpenFlow message received in past 5 seconds!");
        //}

        else if(nfds > 0) {
            for(i = 0; i < nfds; i++) {
                fd = events[i].data.fd;
                length = 0;
                //int tmp_length = 0;
                if (events[i].events == EPOLLIN) {
                    length = read_msg(fd, recv_buf);
                    //printf("\nread buffer length: %d\n", length);
                    if (length < 0) {
                        OFsuite_syslog(LV_WARN,"OFsuite_recv_event", "Read sw %d buffer error", fd);
                        printf("OFsuite_performance> ");
                        continue;
                    }
                    
                    else if (length == 0) {
                        OFsuite_syslog(LV_INFO,"OFsuite_recv_event","Socket %d has been closed!", fd);
                        printf("OFsuite_performance> ");
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &event);
                        socket_destroy(fd);
                    }
                    //if (!error && length <= MAX_BUF_SIZE) {
                    else {
                        //if (length >= MAX_BUF_SIZE)
                        //    printf("\nread message length: %d\n", length);
                        parse_openflow(recv_buf, fd, length);
                        memset(recv_buf, 0, MAX_BUF_SIZE);
                    }
                    /*
                    if (tmp_length == MAX_BUF_SIZE) {
                        length += tmp_length;
                        while(tmp_length == MAX_BUF_SIZE) {
                            tmp_length = read_msg(fd, recv_buf + length);
                            length += tmp_length;
                        }
                    }
                    else
                        length = tmp_length;
                        */
                }
                
                else if (events[i].events == (EPOLLIN | EPOLLRDHUP)) {
                    int i = get_controller_from_sockfd(fd);
                    static int n_disconnected = 0;
                    if(i < n_controllers){
                        OFsuite_syslog(LV_INFO, "OFsuite_connection","Controller %s disconnected!", addr[i]);
                        OFsuite_syslog(LV_INFO, "OFsuite_connection", "%d TCP disconnected!", ++n_disconnected);
                    }
                    else {
                        struct fksw_auxi fk_au;
                        get_switch_from_auxifd(&fk_au, fd);
                        OFsuite_syslog(LV_INFO, "OFsuite_connection", "Auxiliary connection %d disconnected!", fk_au.auxi_id);
                    }
                    printf("OFsuite_performance> ");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &event);
                    socket_destroy(fd);
                    continue;
                }
            }
        }
    }
    free(events);
    close (epfd);
    thread_done = 1;
}

void init_recv_event_thread(void) {
    pthread_t thread;
    pthread_create(&thread, NULL, (void *)&init_recv_event, NULL);
    pthread_detach(thread);
}

void send_arp_packet_in(void *arg) {
    
    cpu_set_t set;
    struct ofp_arp_packet_in pi;
    uint16_t fksw_no;
    uint8_t i;
    struct p_port *pport;
    //uint8_t first_packet_in = 1;
    fakesw *fksw = p_fakesw;
    ofp_buf buf[MAX_BUF_SIZE];

    struct ether_header eth_h;
    struct arp_header arp_h;
    /*
    struct thread_packet_arg {
        uint8_t mode;
    }p_arg;
    p_arg.mode = ((struct thread_packet_arg *)arg)->mode;*/
    CPU_ZERO(&set);
    CPU_SET(7, &set);
    sched_setaffinity(0, sizeof(set), &set);
    pport = add_fakesw_port(fksw, MAX_SW);
    for (i = 0; i < n_controllers; i++) {
        send_port_status(fksw->sock_fd[i], pport->port, OFPPR_ADD);
    }
    pport = add_fakesw_port(fksw, MAX_SW + 1);
    for (i = 0; i < n_controllers; i++) {
        send_port_status(fksw->sock_fd[i], pport->port, OFPPR_ADD);
    }
    init_ofp_buf(buf);
    gen_arp_request(&pi, &eth_h, &arp_h, buf);
    //send_msg(fksw->sock_fd, buf, 84);
    //gen_arp_reply(&pi, &eth_h, &arp_h, buf);
    //send_msg(fksw->sock_fd, buf, 84);
    signal(SIGPIPE, SIG_IGN);
    if (module_log[LOG_PACKET_OUT_RATE] || module_log[LOG_FLOW_MOD_RATE]) {
        start_send_arp = 1;
        //printf("\nTesting, please wait...");
        while (send_arp < 2 * arp_rate) {
            uint8_t i;
            if(fksw_no >= n_fakesw)
                fksw = p_fakesw;
            //pthread_mutex_lock(&mutx);
            while(tb.tokens < 1) {
                //pthread_cond_wait(&cond,&mutx);
                ;
            }
            tb.tokens -= 1;
            //pthread_mutex_unlock(&mutx);
            
            for(fksw_no = 0; fksw_no < n_fakesw; fksw_no++) {
                for (i = 0; i < n_controllers; i++) {
                    if(fksw->sock_fd[i]) {
                        //init_ofp_buf(buf);
                        //gen_arp_request(&pi, &eth_h, &arp_h, fksw->dpid, buf);
                        send_msg(fksw->sock_fd[i], buf, 84);
                        /*if (first_packet_in) {
                            first_packet_in = 0;
                            OFsuite_log(LOG_PACKET_OUT_RATE, send_arp + 1, "first packet in");
                        }
                        else {
                            OFsuite_log(LOG_PACKET_OUT_RATE, send_arp + 1, "send packet in");
                        }*/
                    }
                }
                send_arp++;
                OFsuite_log(LOG_PACKET_OUT_RATE, send_arp, "send packet in");
                /*
                pi.oh.xid += htonl(1);
                memcpy(buf, &pi, sizeof(struct ofp_arp_packet_in));
                arp_h.sender_ip[3]++;
                arp_h.target_ip[3]++;
                memcpy(buf + sizeof(struct ofp_arp_packet_in) + 2 + sizeof(struct ether_header), &arp_h, sizeof(struct arp_header));
                */
                //fksw++;
            }
        }
        all_arp_send = 1;
        outtime = set_outtime(10);
        pthread_mutex_lock(&mutx);
        if(!pthread_cond_timedwait(&cond_packet_out, &mutx, &outtime)) {
            OFsuite_syslog(LV_INFO,"OFsuite_test","Test done!");
            send_arp_done = 1;
        }
        else {
            OFsuite_syslog(LV_INFO,"OFsuite_test","Test time out!");
            send_arp_done = 1;
        }
        pthread_mutex_unlock(&mutx);
    }
    else if (module_log[LOG_RECOVERY_TIME]) {
        send_arp_done = 1;
        //OFsuite_syslog(LV_INFO,"OFsuite_test","Test done!");
        while (!stop_send_arp) {
            uint8_t i;
            if(fksw_no >= n_fakesw)
                fksw = p_fakesw;
            //pthread_mutex_lock(&mutx);
            while(tb.tokens < 1) {
                //pthread_cond_wait(&cond,&mutx);
                ;
            }
            tb.tokens -= 1;
            //pthread_mutex_unlock(&mutx);
            
            for(fksw_no = 0; fksw_no < n_fakesw; fksw_no++) {
                for (i = 0; i < n_controllers; i++) {
                    if(fksw->sock_fd[i]) {
                        //init_ofp_buf(buf);
                        //gen_arp_request(&pi, &eth_h, &arp_h, fksw->dpid, buf);
                        send_msg(fksw->sock_fd[i], buf, 84);
                        /*if (first_packet_in) {
                            first_packet_in = 0;
                            OFsuite_log(LOG_PACKET_OUT_RATE, send_arp + 1, "first packet in");
                        }
                        else {
                            OFsuite_log(LOG_PACKET_OUT_RATE, send_arp + 1, "send packet in");
                        }*/
                    }
                }
                send_arp++;
                //OFsuite_log(LOG_PACKET_OUT_RATE, send_arp, "send packet in");
                //fksw++;
            }
        }
    }
    else if (module_log[LOG_AUXILIARY_CHANNEL] || module_log[LOG_FLOW_MOD_AUXI_RATE]) {
        start_send_arp = 1;
        while(send_arp < 2 * arp_rate * (n_auxiliary_ch+1)) {
            uint8_t i,j;
            while(tb.tokens < 1) {
                ;
            }
            tb.tokens -= 1;
            for(fksw_no = 0; fksw_no < n_fakesw; fksw_no++) {
                for (i = 0; i < n_controllers; i++) {
                    if(fksw->sock_fd[i]) {
                        send_msg(fksw->sock_fd[i], buf, 84);
                        send_arp++;
                        OFsuite_log(LOG_AUXILIARY_CHANNEL, send_arp, "send packet in");
                        OFsuite_log(LOG_FLOW_MOD_AUXI_RATE, send_arp, "send packet in");
                    }
                    for (j = 0; j < n_controllers * n_auxiliary_ch; j++) {
                        send_msg(fksw->auxi[i*n_controllers+j].auxi_fd, buf, 84);
                        send_arp++;
                        OFsuite_log(LOG_AUXILIARY_CHANNEL, send_arp, "send packet in");
                        OFsuite_log(LOG_FLOW_MOD_AUXI_RATE, send_arp, "send packet in");
                    }
                }
            }
        }
        send_arp_done = 1;
        OFsuite_syslog(LV_INFO,"OFsuite_test","All packet_in has been send to the controller!");
        printf("OFsuite_performance> ");
    }
    else {
        send_arp_done = 1;
        OFsuite_syslog(LV_INFO,"OFsuite_test","Test done!");
        printf("OFsuite_performance> ");
    }
}

void set_arp_packet_in(uint64_t rate) {
    pthread_t thread_token_bucket,thread_packet_in;
    struct thread_token_arg {
        uint64_t rate;
    };
    struct thread_token_arg *t_arg = (struct thread_token_arg *)malloc(sizeof(struct thread_token_arg));
    t_arg->rate = rate;
    pthread_create(&thread_packet_in, NULL, (void *)&send_arp_packet_in, NULL);
    pthread_detach(thread_packet_in);
    pthread_create(&thread_token_bucket, NULL, (void *)&init_token_bucket, (void *)t_arg);
    pthread_detach(thread_token_bucket);
}

int set_port_down(void) {
    struct p_port *pport;
    uint8_t i;
    uint8_t error = 0;
    ofp_buf buf[MAX_BUF_SIZE];
    struct ofp_arp_packet_in pi;
    struct ether_header eth_h;
    struct arp_header arp_h;
    fakesw *fksw = OFsuite_topo.fksw[n_fakesw - 1];
    if (topo_mod == TOPO_NONE) {
        OFsuite_syslog(LV_INFO,"OFsuite_test","No topology has been set! Please set up a topology with set-topo command");
        return ERROR_SET_PORT_DOWN;
    }
    else if(topo_mod == TOPO_LINEAR || topo_mod == TOPO_TREE || topo_mod == TOPO_MANUAL) {
        OFsuite_syslog(LV_WARN, "OFsuite_test", "Invalid topology, please run this test with ring, full-mesh or leaf-spine topology!");
        return ERROR_SET_PORT_DOWN;
    }
    else {
        tree_destroy();
        tree_init();
        is_datapath_failure = 1;
        if(!switch_host) {
            pport = add_fakesw_port(OFsuite_topo.fksw[0], MAX_SW);
            for (i = 0; i < n_controllers; i++) {
                send_port_status(OFsuite_topo.fksw[0]->sock_fd[i], pport->port, OFPPR_ADD);
            }
            pport = add_fakesw_port(fksw, MAX_SW + 1);
            for (i = 0; i < n_controllers; i++) {
                send_port_status(fksw->sock_fd[i], pport->port, OFPPR_ADD);
            }
            switch_host = n_fakesw;
        }
        conn_test_done = 0;
        pport = fksw->pport;
        while (ntohl(pport->port.port_no) == 0xfffffffe && pport != NULL) {
            pport = pport->next;
        }
        if (pport != NULL) {
            fksw->n_port--;
            OFsuite_topo.matrix[n_fakesw-1][ntohl(pport->port.port_no)-1] = 0;
            OFsuite_topo.matrix[ntohl(pport->port.port_no)-1][n_fakesw-1] = 0;
            (OFsuite_topo.fksw[ntohl(pport->port.port_no)-1]->n_port)--;
            pport->port.config = htonl(OFPPC_PORT_DOWN);
            for (i = 0; i < n_controllers; i++) {
                if (send_port_status(fksw->sock_fd[i], pport->port, OFPPR_DELETE) < 0)
                    error = ERROR_SET_PORT_DOWN;
            }
            OFsuite_log(LOG_LINK_EVENT, 0, "set link down");
        }
        init_ofp_buf(buf);
        gen_arp_request(&pi, &eth_h, &arp_h, buf);
        for(i = 0; i < n_controllers; i++) {
            send_msg(OFsuite_topo.fksw[0]->sock_fd[i], buf, 84);
        }
        OFsuite_syslog(LV_INFO,"OFsuite_test","Waiting for datapath recovery...");
        outtime = set_outtime(30);
        pthread_mutex_lock(&mutx);
        if(!pthread_cond_timedwait(&cond, &mutx, &outtime)) {
            OFsuite_log(LOG_LINK_EVENT, 1, "datapath failure recovery");
        }
        else {
            OFsuite_syslog(LV_INFO,"OFsuite_test","Test time out!");
            error = ERROR_SET_PORT_DOWN;
        }
        pthread_mutex_unlock(&mutx);
    }
    return error;
}

int send_conn_arp(uint16_t sw_dpid) {
    uint8_t i,j;
    uint8_t error = 0;
    ofp_buf buf[MAX_BUF_SIZE];
    struct ofp_arp_packet_in pi;
    struct ether_header eth_h;
    struct arp_header arp_h;
    struct p_port *pport;

    if(sw_dpid <= n_fakesw) {
        pport = add_fakesw_port(OFsuite_topo.fksw[0], MAX_SW);
        for (i = 0; i < n_controllers; i++) {
            send_port_status(OFsuite_topo.fksw[0]->sock_fd[i], pport->port, OFPPR_ADD);
        }
        pport = add_fakesw_port(OFsuite_topo.fksw[sw_dpid - 1], MAX_SW + 1);
        for (i = 0; i < n_controllers; i++) {
            send_port_status(OFsuite_topo.fksw[sw_dpid - 1]->sock_fd[i], pport->port, OFPPR_ADD);
        }
        init_ofp_buf(buf);
        gen_arp_request(&pi, &eth_h, &arp_h, buf);
        memset(send_dpid, 0, sizeof(send_dpid));
        //send_dpid[0] = 1;
        //total_send = 1;
        for (i = 0; i < n_controllers; i++) {
            send_msg(OFsuite_topo.fksw[0]->sock_fd[i], buf, 84);
            for(j = 0; j < n_auxiliary_ch; j++) {
                send_msg(OFsuite_topo.fksw[0]->auxi[i*n_auxiliary_ch+j].auxi_fd, buf, 84);
            }
        }
        OFsuite_log(LOG_CONNECTIVITY_TEST, 0, "start connectivity test");
        //while (total_send < n_fakesw) {;}
        memset(send_dpid, 0, sizeof(send_dpid));
        //send_dpid[sw_dpid - 1] = 1;
        //total_send = 1;
        /*
        init_ofp_buf(buf);
        gen_arp_reply(&pi, &eth_h, &arp_h, buf);
        for (i = 0; i < n_controllers; i++) {
            send_msg(OFsuite_topo.fksw[sw_dpid - 1]->sock_fd[i], buf, 84);
        }*/
        OFsuite_syslog(LV_INFO,"OFsuite_test", "Waiting for creating end-to-end link...");
        outtime = set_outtime(30);
        pthread_mutex_lock(&mutx);
        if(pthread_cond_timedwait(&cond, &mutx, &outtime)) {
            OFsuite_syslog(LV_INFO,"OFsuite_test","Test time out!");
            error = ERROR_SET_HOST;
        }
        pthread_mutex_unlock(&mutx);
    }
    else {
        OFsuite_syslog(LV_INFO,"OFsuite_test","Invalid switch number, no such switch");
        error = ERROR_SET_HOST;
    }
    return error;
}

int set_new_switch(void) {
    int error;
    if (switch_host) {
        int i;
        //int j;
        ofp_buf buf[MAX_BUF_SIZE];
        struct ofp_arp_packet_in pi;
        struct ether_header eth_h;
        struct arp_header arp_h;
        struct p_port *pport;
        uint8_t con_no;
        fakesw *fksw;
        int cur_nsw = n_fakesw;
        uint64_t so = 12582912;
        n_fakesw += 1;
        fksw =&fake_sw[cur_nsw];
        OFsuite_topo.sw_num = n_fakesw;
        for (con_no = 0; con_no < n_controllers; con_no++) {
            if ((fksw->sock_fd[con_no] = socket_create()) < 0) {
                OFsuite_syslog(LV_ERROR,"OFsuite_fakesw","Failed to create switch %d!", cur_nsw + 1);
                error = ERROR_CREATESW;
            }
            if( (setsockopt( fksw->sock_fd[con_no], SOL_TCP, TCP_NODELAY, (char *)&so, sizeof(so) )) < 0) {
                OFsuite_syslog(LV_ERROR, "OFsuite_fakesw","Failed to set switch %d's socket option", cur_nsw+1);
            }
            event.data.fd = fksw->sock_fd[con_no];
            event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
            int flag = epoll_ctl(epfd, EPOLL_CTL_ADD, fksw->sock_fd[con_no], &event);
            if (flag == -1) {
                OFsuite_syslog(LV_ERROR,"OFsuite_fakesw","Failed to add switch %d's socket into epoll!", cur_nsw + 1);
                error = ERROR_EPOLL;
            }
            if ((socket_connect(fksw->sock_fd[con_no], (struct sockaddr *)&controller[con_no])) < 0) {
                OFsuite_syslog(LV_ERROR,"OFsuite_fakesw","Failed to connect switch %d to its controller!", cur_nsw + 1);
                error = ERROR_CONNECT_CONTROLLER;
            }
            if ((make_socket_non_blocking(fksw->sock_fd[con_no])) < 0) {
                OFsuite_syslog(LV_ERROR,"OFsuite_fakesw","Failed to make switch %d's socket none blocking!", cur_nsw + 1);
                error = ERROR_MAKE_SOCKET_NO_BLOCKING;
            }
            for (i = 0; i < n_auxiliary_ch; i++) {
                if ((fksw->auxi[con_no*n_auxiliary_ch+i].auxi_fd = socket_create()) < 0) {
                    OFsuite_syslog(LV_ERROR,"OFsuite_fakesw","Failed to create switch %d!\n", cur_nsw + 1);
                    exit(1);
                }
                if( (setsockopt( fksw->auxi[con_no*n_auxiliary_ch+i].auxi_fd, SOL_TCP, TCP_NODELAY, (char *)&so, sizeof(so) )) < 0) {
                    OFsuite_syslog(LV_ERROR,"OFsuite_fakesw","Failed to set switch %d's socket option", cur_nsw+1);
                }
                event.data.fd = fksw->auxi[con_no*n_auxiliary_ch+i].auxi_fd;
                event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;

                int flag = epoll_ctl(epfd, EPOLL_CTL_ADD, fksw->auxi[con_no*n_auxiliary_ch+i].auxi_fd, &event);
                if (flag == -1) {
                    OFsuite_syslog(LV_ERROR, "OFsuite_fakesw","Failed to add switch %d's socket into epoll failed!", cur_nsw + 1);
                    break;
                }
                
                if ((socket_connect(fksw->auxi[con_no*n_auxiliary_ch+i].auxi_fd, (struct sockaddr *)&controller[con_no])) < 0) {
                    OFsuite_syslog(LV_ERROR,"OFsuite_fakesw","Failed to connect switch %d connect to controller failed!", cur_nsw + 1);
                    exit(1);
                }
                
                if ((make_socket_non_blocking(fksw->auxi[con_no*n_auxiliary_ch+i].auxi_fd)) < 0) {
                    OFsuite_syslog(LV_ERROR,"OFsuite_fakesw","Failed to make switch %d's socket none blocking failed!", cur_nsw + 1);
                    exit(1);
                }
                fksw->auxi[con_no*n_auxiliary_ch+i].auxi_id = i + 1;
            }
        }
        fksw->dpid = cur_nsw + 1;
        init_fakesw_port(fksw, cur_nsw);
        fksw->n_port = 1;
        fksw->table = NULL;
        init_fksw_flows(fksw);
        OFsuite_topo.fksw[cur_nsw] = fksw;
        OFsuite_syslog(LV_INFO, "OFsuite_fakesw","Setting a new switch...");
        OFsuite_topo.matrix[0][cur_nsw] = 1;
        OFsuite_topo.matrix[cur_nsw][0] = 1;
        OFsuite_topo.matrix[switch_host - 1][cur_nsw] = 1;
        OFsuite_topo.matrix[cur_nsw][switch_host - 1] = 1;
        add_fakesw_port(OFsuite_topo.fksw[cur_nsw], 0);
        add_fakesw_port(OFsuite_topo.fksw[cur_nsw], switch_host - 1);
        pport = add_fakesw_port(OFsuite_topo.fksw[0], cur_nsw);
        for(con_no = 0; con_no < n_controllers; con_no++) {
            send_port_status(OFsuite_topo.fksw[0]->sock_fd[con_no], pport->port, OFPPR_ADD);
        }
        pport = add_fakesw_port(OFsuite_topo.fksw[switch_host - 1], cur_nsw);
        for(con_no = 0; con_no < n_controllers; con_no++) {
            send_port_status(OFsuite_topo.fksw[switch_host-1]->sock_fd[con_no], pport->port, OFPPR_ADD);
        }
        /*
        pport = OFsuite_topo.fksw[0]->pport;
        while(ntohl(pport->port.port_no) != cur_nsw + 1 &&  pport != NULL) {
            pport = pport->next;
        }
        if (pport != NULL) {
            for(con_no = 0; con_no < n_controllers; con_no++) {
                send_port_status(OFsuite_topo.fksw[0]->sock_fd[con_no], pport->port, OFPPR_ADD);
            }
        }
        pport = OFsuite_topo.fksw[switch_host - 1]->pport;
        while(ntohl(pport->port.port_no) != cur_nsw + 1 &&  pport != NULL) {
            pport = pport->next;
        }
        if (pport != NULL) {
            for(con_no = 0; con_no < n_controllers; con_no++) {
                send_port_status(OFsuite_topo.fksw[switch_host - 1]->sock_fd[con_no], pport->port, OFPPR_ADD);
            }
        }*/
        for(con_no = 0; con_no < n_controllers; con_no++) {
            send_hello(fksw->sock_fd[con_no]);
        }
        OFsuite_syslog(LV_INFO,"OFsuite_fakesw","Setting a new switch done!");
        fksw = NULL;
        tree_destroy();
        tree_init();
        is_datapath_switchover = 1;
        conn_test_done = 0;
        init_ofp_buf(buf);
        gen_arp_request(&pi, &eth_h, &arp_h, buf);
        for (i = 0; i < n_controllers; i++) {
            send_msg(OFsuite_topo.fksw[0]->sock_fd[i], buf, 84);
        }
        //init_ofp_buf(buf);
        //gen_arp_reply(&pi, &eth_h, &arp_h, buf);
        //for (i = 0; i < n_controllers; i++) {
        //    send_msg(OFsuite_topo.fksw[switch_host - 1]->sock_fd[i], buf, 84);
        //    for(j = 0; j < n_auxiliary_ch; j++) {
        //        send_msg(OFsuite_topo.fksw[switch_host - 1]->auxi[i*n_auxiliary_ch+j].auxi_fd, buf, 84);
        //    }
        //}
        OFsuite_log(LOG_DATAPATH_SWITCHOVER, 0, "start datapath switchover test");
        OFsuite_syslog(LV_INFO, "OFsuite_test","Waiting for datapath switchover...");
        
        outtime = set_outtime(30);
        pthread_mutex_lock(&mutx);
        if(pthread_cond_timedwait(&cond, &mutx, &outtime)) {
            OFsuite_syslog(LV_INFO,"OFsuite_test","time out!");
            error = ERROR_SET_NEW_SWITCH;
        }
        pthread_mutex_unlock(&mutx);
    }
    else {
        error = ERROR_SET_NEW_SWITCH;
        OFsuite_syslog(LV_INFO, "OFsuite_test","No enough hosts, please setup host with set-host command!");
    }
    return error;
}

fakesw *get_switch_from_sockfd(uint16_t socket_fd) {
    fakesw *fksw = NULL;
    uint8_t is_found = 0;
    uint16_t fksw_no = 0;
    uint8_t ctrler_no = 0;
    while(fksw_no < n_fakesw && !is_found) {
        ctrler_no = 0;
        while(ctrler_no < n_controllers && !is_found) {
            if(socket_fd == OFsuite_topo.fksw[fksw_no]->sock_fd[ctrler_no]) {
                is_found = 1;
                fksw = OFsuite_topo.fksw[fksw_no];
            }
            else
                ctrler_no++;
        }
        if(!is_found)
            fksw_no++;
    }
    return fksw;
}

void get_switch_from_auxifd(struct fksw_auxi *fk_au, uint16_t auxi_fd) {
    fk_au->fksw = NULL;
    fk_au->auxi_id = 0;
    uint8_t is_found = 0;
    uint16_t fksw_no = 0;
    uint8_t ctrler_no = 0;
    uint8_t auxi_no = 0;
    while(fksw_no < n_fakesw && !is_found) {
        ctrler_no = 0;
        while(ctrler_no < n_controllers && !is_found) {
            auxi_no = 0;
            while(auxi_no < n_auxiliary_ch && !is_found) {
                if (auxi_fd == OFsuite_topo.fksw[fksw_no]->auxi[ctrler_no*n_auxiliary_ch+auxi_no].auxi_fd) {
                    is_found = 1;
                    fk_au->fksw = OFsuite_topo.fksw[fksw_no];
                    fk_au->auxi_id = OFsuite_topo.fksw[fksw_no]->auxi[ctrler_no*n_auxiliary_ch+auxi_no].auxi_id;
                }
                else
                    auxi_no++;
            }
            if(!is_found)
                ctrler_no++;
        }
        if(!is_found)
            fksw_no++;
    }
}

int get_controller_from_sockfd(uint16_t socket_fd) {
    uint8_t is_found = 0;
    uint16_t fksw_no = 0;
    uint8_t ctrler_no = 0;
    while(fksw_no < n_fakesw && !is_found) {
        ctrler_no = 0;
        while(ctrler_no < n_controllers && !is_found) {
            if(socket_fd == OFsuite_topo.fksw[fksw_no]->sock_fd[ctrler_no]) {
                is_found = 1;
            }
            else
                ctrler_no++;
        }
        if(!is_found)
            fksw_no++;
    }
    return ctrler_no;
}

void OFsuite_exit(void) {
    int fksw_no, i;
    OFsuite_syslog(LV_INFO,"OFsuite_exit","Quiting...");
    ON_EXIT = 1;
    while (!thread_done);
    for(fksw_no = 0; fksw_no < n_fakesw; fksw_no++) {
        if (p_fakesw->pport != NULL) {
            free(p_fakesw->pport);
            p_fakesw->pport = NULL;
        }
        for (i = 0; i < n_controllers; i++) {
            socket_destroy(p_fakesw->sock_fd[i]);
        }
        for (i = 0; i < n_controllers*n_auxiliary_ch; i++) {
            socket_destroy(p_fakesw->auxi[i].auxi_fd);
        }
        p_fakesw++;
    }
    socket_destroy(epfd);
    OFsuite_syslog(LV_INFO,"OFsuite_exit","Now quit!\n");
    system("sudo killall tcpdump > /dev/null 2>&1 &");
}
