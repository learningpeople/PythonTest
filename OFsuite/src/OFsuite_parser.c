/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */

#include "OFsuite_parser.h"
#include "OFsuite_topology.h"
#include "OFsuite_link_connectivity.h"
#include "OFsuite_log.h"
#include "../OFsuite_performance.h"
#include <netinet/in.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/stat.h> 

extern uint16_t n_fakesw;
extern uint8_t n_controllers;
extern fakesw *p_fakesw;
extern char addr[MAX_CONTROLLERS][16];
extern uint16_t port;
extern uint64_t arp_rate;
extern uint8_t topo_mod;
extern uint8_t send_arp_done;
extern uint8_t stop_send_arp;
extern uint8_t start_conn_test;
//extern char Project_Dir[256];
//extern char Log_Dir[256];
//extern char Python_Dir[256];
extern int SSL_MODE;
extern uint8_t n_auxiliary_ch;
extern syslog_level LOG_LV;
extern pthread_mutex_t echo_mutx;
extern pthread_cond_t echo_cond;
extern uint16_t switch_host;


int receive_echo_reply = 0;
uint8_t connect_state[MAX_SW] = {0};
char *short_opts = ":hs:c:p:t:lx:g:a:dr:o:wue:n:";
char cmd[BUFSIZ] = {0};
struct option long_opts[] = {
    {"help", no_argument, NULL, 'h'},
    {"switches", required_argument, NULL, 's'},
    {"controller", required_argument, NULL, 'c'},
    {"port", required_argument, NULL, 'p'},
    {"topo", required_argument, NULL, 't'},
    {"ssl", no_argument, NULL, 'l'},
    {"auxi", required_argument, NULL, 'x'},
    {"log", required_argument, NULL, 'g'},
    {"add-sw", required_argument, NULL, 'a'},
    {"set-port-down", no_argument, NULL, 'd'},
    {"set-arp-rate", required_argument, NULL, 'r'},
    {"set-topo", required_argument, NULL, 'o'},
    {"set-new-sw", no_argument, NULL, 'w'},
    {"show-result", no_argument, NULL, 'u'},
    {"set-host", required_argument, NULL, 'e'},
    {"stop-send-arp", no_argument, NULL, 'n'},
    {NULL, 0, NULL, 0}
};

typedef struct command {
    char *name;
}COMMAND;

COMMAND commands[] = {
    {"add-sw"},
    {"set-port-down"},
    {"set-arp-rate"},
    {"set-topo"},
    {"set-new-sw"},
    {"show-result"},
    {"set-host"},
    {"stop-send-arp"},
    {"help"},
    {"quit"},
    {NULL},
};
/*
char *get_long_from_short(int c) {
    int n_options = sizeof(long_opts)/sizeof(struct option);
    char *ret = NULL;
    int i;
    for(i = 0; i < n_options; i++) {
        if(long_opts[i].val == c) {
            ret = long_opts[i].name;
            break;
        }
    }
    return ret;
}
*/
uint8_t (*handle_openflow[36]) (ofp_buf *msg_buf, uint16_t ofconn) = {
    handle_hello,
    handle_default,
    handle_echo_request,
    handle_default,
    handle_default,
    handle_features_request,
    handle_default,
    handle_get_config_request,
    handle_default,
    handle_set_config,
    handle_default,
    handle_default,
    handle_default,
    handle_packet_out,
    handle_flow_mod,
    handle_default,
    handle_default,
    handle_default,
    handle_multipart_request,
    handle_default,
    handle_barrier_request,
    handle_default,
    handle_default,
    handle_default,
    handle_role_request,
    handle_default,
    handle_default,
    handle_default,
    handle_default,
    handle_default,
    handle_default,
    handle_default,
    handle_default,
    handle_default,
    handle_default,
    handle_default,
};

void parse_command(int argc, char **argv) {

    int opt;
    char *substr;
    uint8_t i = 0;
    //char *cur_dir = Project_Dir;
    //char *p_dir = NULL;
    char all_addr[MAX_CONTROLLERS * 15];
    opterr = 0;
    if(argc < 4) {
        OFsuite_usage(argv);
        exit(1);
    }
    /*
    getcwd(cur_dir, 256);
    substr = strstr(cur_dir, "OFsuite_performance");
    while(substr != NULL) {
        p_dir = substr;
        substr = strstr(substr + 20, "OFsuite_performance");
    }
    if (p_dir) {
        strcpy(p_dir, "OFsuite_performance");
        strcpy(Log_Dir, Project_Dir);
        strcat(Log_Dir, "/OFsuite_log/");
        strcpy(Python_Dir, Project_Dir);
        strcat(Python_Dir, "/OFsuite_python");
    }
    else {
        strcpy(Log_Dir, Project_Dir);
        strcat(Log_Dir, "/OFsuite_log/");
        mkdir("OFsuite_log", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        strcpy(Python_Dir, Project_Dir);
        strcat(Python_Dir, "/OFsuite_python");
        mkdir("OFsuite_python", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }*/
    while((opt = getopt_long(argc, argv, short_opts, long_opts, NULL)) != -1) {
		switch(opt) {
			case 'h':
				OFsuite_usage(argv);
				break;
			case 's':
				n_fakesw = atoi(optarg);
                if(!n_fakesw){
                    OFsuite_syslog(LV_ERROR, "OFsuite_init", "No switch set!");
                    exit(1);
                }
				break;
			case 'c':
				strcpy(all_addr, optarg);
				substr = strtok(all_addr, ",");
				while(substr != NULL) {
					strcpy(addr[i], substr);
					substr = strtok(NULL, ",");
					i++;
					n_controllers = i;
				}
                for(i=0;i<n_controllers;i++){
                    if(strstr(addr[i], "--")){
                        n_controllers = 0;
                        break;
                    }
                }
                if(!n_controllers){
                    OFsuite_syslog(LV_ERROR, "OFsuite_init", "No controller address set!");
                    exit(1);
                }
				break;
			case 'p':
				port = atoi(optarg);
                if(!port){
                    OFsuite_syslog(LV_ERROR, "OFsuite_init", "Invalid port number!");
                    exit(1);
                }
				break;
			case 't':
				if (!strcmp(optarg, "linear")) {
					topo_mod = TOPO_LINEAR;
				}
				else if (!strcmp(optarg, "ring")) {
					topo_mod = TOPO_RING;
				}
				else if (!strcmp(optarg, "full-mesh")) {
					topo_mod = TOPO_FULL_MESH;
				}
				else if (!strcmp(optarg, "leaf-spine")) {
					topo_mod = TOPO_LEAF_SPINE;
				}
				else {
					topo_mod = TOPO_NONE;
				}
				break;
			case 'l':
				//if (atoi(optarg))
				SSL_MODE = 1;
				break;
			case 'x':
				n_auxiliary_ch = atoi(optarg);
				if(!n_auxiliary_ch) {
					OFsuite_syslog(LV_ERROR, "OFsuite_test", "No auxiliary connection set!");
					exit(1);
				}
				break;
			case 'g':
				if (!strcmp(optarg, "fatal"))
					LOG_LV = LV_FATAL;
				else if(!strcmp(optarg, "error"))
					LOG_LV = LV_ERROR;
				else if(!strcmp(optarg, "warn"))
					LOG_LV = LV_WARN;
				else if(!strcmp(optarg, "info"))
					LOG_LV = LV_INFO;
				else if(!strcmp(optarg, "debug"))
					LOG_LV = LV_DEBUG;
				break;
            case ':': {
                char *e_opt = (char *)malloc(64);
                strcpy(e_opt, argv[optind-1]);
                OFsuite_syslog(LV_ERROR, "OFsuite_parser", "Option \"%s\" needs an argument!", e_opt);
                free(e_opt);
                exit(1);
                break;
            }
            case '?': {
                char *e_opt = (char *)malloc(64);
                strcpy(e_opt, argv[optind-1]);
                OFsuite_syslog(LV_ERROR, "OFsuite_parser", "Unknown option: %s", e_opt);
                free(e_opt);
                exit(1);
                break;
            }
			default:
                OFsuite_syslog(LV_ERROR, "OFsuite_parser", "Unknown option");
				break;
		}
    }
    printf(\
            "\033[34m\t   _____    ___________________     ________________________________           _______ \n"
            "\033[34m\t  / ___ \\  / ______/  ______/ /    / /___  ___/___  _____/ ________/          / _____ \\\n"
            "\033[34m\t / /   \\ \\/ /_____ |  |    / /    / /   / /      / /    / /_____    _____    / /____/ /\n"
            "\033[34m\t/ /    / / /_____/  \\  \\  / /    / /   / /      / /    / /_____/   /_____/  / /______/ \n"
            "\033[34m\t\\ \\___/ / /     _____|  |/  \\___/ /___/ /___   / /    / /_______           / /         \n"
            "\033[34m\t \\_____/_/     /________/\\_______/_________/  /_/    /_________/          /_/          \n"
            "\033[0m\n"
            );
}

void parse_user_command(void) {
    uint16_t n_sw;
    int opt;
    uint8_t topo = 0;
    char **argv = (char **)malloc(2 * sizeof(char *));
    char *substr;
    char substr1[32] = {0};
    char substr2[32] = {0};
    argv[0] = (char *)malloc(32 * sizeof(char));
    argv[1] = (char *)malloc(32 * sizeof(char));
    strcpy(argv[0], "OFsuite_performance");
    init_readline();
    while(1) {
        setbuf(stdin, NULL);
        //printf("OFsuite_performance> ");
        OFsuite_getcmdline(cmd);
        //fgets(cmd, BUFSIZ, stdin);
        //fflush(stdin);
        //cmd[strlen(cmd)-1] = '\0';
        if (!strcmp(cmd, "quit") || !strcmp(cmd, "quit "))
            break;
        else if (strlen(cmd) > 1) {
            strcpy(argv[1], cmd);
            if (argv[1][0] != '-') {
                if (argv[1][1] != '-')
                    sprintf(argv[1], "--%s", cmd);
            }
            substr = strtok(argv[1], " ");
            strcpy(substr1, substr);
            while(substr != NULL) {
                strcpy(substr2, substr);
                substr = strtok(NULL, " ");
            }
            if (strcmp(substr1, substr2))
                sprintf(argv[1],"%s=%s", substr1, substr2);
            optind = 0;
            while((opt = getopt_long(2, argv, short_opts, long_opts, NULL)) != -1) {
                switch(opt) {
                    case 'a':
                        n_sw = atoi(optarg);
                        add_fakesw(n_sw);
                        break;
                    case 'h':
                        user_command_help();
                        break;
                    case 'd':
                        tree_init();
                        start_conn_test = 1;
                        if (set_port_down()) {
                            OFsuite_syslog(LV_WARN,"OFsuite_test","Datapath failure cannot recovery!");
                        }
                        else {
                            while (!tree_search(switch_host)) {;}
                            OFsuite_syslog(LV_INFO,"OFsuite_test","Test done!");
                            //tree_print();
                        }
                        break;
                    case 'r':
                        arp_rate = atoi(optarg);
                        if (arp_rate >= 1){
                            OFsuite_syslog(LV_INFO,"OFsuite_test","Start sending packet_in...");
                            set_arp_packet_in(arp_rate);
                            while (!send_arp_done) {;}
                        }
                        else {
                            OFsuite_syslog(LV_WARN, "OFsuite_test","Invalid arp rate!");
                        }
                        break;
                    case 'o':
                        if (!strcmp(optarg, "linear")) {
                            topo = TOPO_LINEAR;
                        }
                        else if (!strcmp(optarg, "ring")) {
                            topo = TOPO_RING;
                        }
                        else if (!strcmp(optarg, "full-mesh")) {
                            topo = TOPO_FULL_MESH;
                        }
                        else if (!strcmp(optarg, "leaf-spine")) {
                            topo = TOPO_LEAF_SPINE;
                        }
                        else if (!strcmp(optarg, "tree")) {
                            topo = TOPO_TREE;
                        }
                        else {
                            topo = TOPO_NONE;
                        }
                        update_fakesw_topology(topo);
                        break;
                    case 'u':
                        parse_log();
                        break;
                    case 'e':
                        tree_init();
                        switch_host = atoi(optarg);
                        start_conn_test = 1;
                        if (send_conn_arp(switch_host)) {
                            OFsuite_syslog(LV_WARN,"OFsuite_test","Cannot setup end-to-end link!");
                        }
                        else {
                            while (!tree_search(atoi(optarg))) {;}
                            OFsuite_syslog(LV_INFO,"OFsuite_test","End-to-end flow setup done!");
                            //tree_print();
                        }
                        break;
                    case 'n':
                        stop_send_arp = 1;
                        break;
                    case 'w':
                        start_conn_test = 1;
                        if(set_new_switch()) {
                            OFsuite_syslog(LV_WARN, "OFsuite_test","Failed to test datapath switchover");
                        }
                        else {
                            while (!tree_search(switch_host)) {;}
                            //tree_print();
                            if(get_dpid(n_fakesw)) {
                                OFsuite_syslog(LV_INFO, "OFsuite_test", "Datapath switchover test done!");
                            }
                            else {
                                //tree_print();
                                OFsuite_syslog(LV_WARN, "OFsuite_test", "End-to-end path not change");
                            }
                        }
                        break;
                    case ':':{
                        OFsuite_syslog(LV_ERROR, "OFsuite_CLI", "Command \"%s\" needs an argument!", argv[1]+2);
                        break;
                    }
                    case '?':{
                        OFsuite_syslog(LV_ERROR, "OFsuite_CLI", "Unknown command: %s", argv[1]+2);
                        break;
                    }
                    default:
                        OFsuite_syslog(LV_ERROR, "OFsuite_CLI", "Unknown command: %s", argv[1]+2);
                        break;
                }
            }
        }
    }
    free(argv[0]);
    free(argv[1]);
    free(argv);
}

void parse_openflow(ofp_buf *msg_buf, int ofconn, int length) {
    struct ofp_header oh;
    uint8_t type;
    int remain_length = 0;
    ofp_buf *p_buf;
    remain_length = length;
    p_buf = msg_buf;
    while (remain_length > 0) {
        memcpy(&oh, p_buf, sizeof(oh));
        type = oh.type;
        OFsuite_syslog(LV_DEBUG, "OFsuite_parser", "Receive OpenFlow message type: %d", type);
        if (type == OFPTYPE_ECHO_REPLY) {
            uint16_t first_sockfd = p_fakesw->sock_fd[0];
            if(!connect_state[ofconn - first_sockfd]) {
                connect_state[ofconn - first_sockfd] = 1; 
                receive_echo_reply++;
                if(receive_echo_reply == n_fakesw) {
                    pthread_mutex_lock(&echo_mutx);
                    pthread_cond_signal(&echo_cond);
                    pthread_mutex_unlock(&echo_mutx);
                    receive_echo_reply = 0;
                    memset(connect_state, 0, sizeof(connect_state));
                }
            }
        }
        if (type < 36) {
            //printf("\nreceive length: %d, OF message type %d\n", length, type);
            handle_openflow[type](p_buf, ofconn);
            memset(p_buf, 0, ntohs(oh.length));
            remain_length -= ntohs(oh.length);
            if (remain_length > 0)
                p_buf += ntohs(oh.length);
        }
        else {
            //printf("\nreceive message length: %d, OF message type %d\n", length, type);
            //printf("\nremain length: %d\n", remain_length);
            remain_length = 0;
            //memset(p_buf, 0, ntohs(oh.length));
            //length -= ntohs(oh.length);
            //if (length > 0)
            //    p_buf += ntohs(oh.length);
        }
    }
}

void init_readline(void) {
    rl_attempted_completion_function = CmdComp;
}

void OFsuite_getcmdline(char *cmd) {
    const char *getline = readline("OFsuite_performance> ");
    //char **rl_comp = rl_completion_matches(getline, CmdGenerator);
    strcpy(cmd, getline);
    if(strlen(getline))
        add_history(getline);
    fflush(stdin);
    //printf("\n%s\n", cmd);
}

char **CmdComp(const char *text, int start, int end){
    char **pMatches = NULL;
    if(start == 0){
        pMatches = rl_completion_matches(text, CmdGenerator);
    }
    return pMatches;
}

char *CmdGenerator(const char *text, int state){
    static int index, len;
    char *cmd_name;
    if(!state){
        index = 0;
        len = strlen(text);
    }
    while((cmd_name = commands[index].name)) {
        index++;
        if(strncmp(cmd_name, text, len) == 0)
            return (strdup(cmd_name));
    }
    return (char *)NULL;
}

void OFsuite_usage(char **argv) {
    
    printf("\nNAME\n\tOFsuite_performance-Openflow SDN Controller Performance Test Tool\n");
    printf("USAGE\n\t %s [options]\n", argv[0]);
    printf("DESCRIPTION\n\tThis test tool aims to perform benchmarking "
        "test for SDN controllers which speak Openflow protocol. To be "
        "specific, this test tool simulates a topology consists of "
        "thousands of Openflow switches and all kinds of Openflow/SDN "
        "network events, measure how much time does a device under "
        "test(Openflow SDN controller) take in order to finish its "
        "specific task.\n");
    printf("OPTIONS\n\t--controller/-c: specify the address of the controller under test\n"
        "\t--port/-p: specify the tcp port of OpenFlow channel (6633 or 6653)\n"
        "\t--switches/-s: number of switches to be created (max 20000)\n"
        "\t--topo/-t: network topology (linear, ring, full-mesh or leaf-spine)\n"
        "\t--auxi/-x: number of auxiliary channel per switch\n"
        "\t--ssl/-l: use ssl for OpenFlow channel\n"
        "\t--log/-g: set log level(fatal/error/warn/info/debug)\n"
        "\t--help/-h: help information\n");

}

void user_command_help(void) {
    printf("\nNAME\n\tOFsuite_performance-CLI\n");
    printf("USAGE\n\t [COMMAND] [argument]\n");
    printf("COMMAND\n\tadd-sw [n]: add n switches \n"
        "\tset-port-down: set one port of the switch down\n"
        "\tset-arp-rate [rate]: set the rate of arp packet in sent to controller\n"
        "\tset-topo [linear/ring/grid/leaf-spine]: set topology\n"
        "\tset-host [sw_no]: set a host connect to [sw_no]\n"
        "\tset-new-sw: setup a new switch for datapath switchover test\n"
        "\tstop-send-arp: stop send arp packet in\n"
        "\tshow-result: show the test result of the current test case\n"
        "\tquit: quit OFsuite_performance tool\n"
        "\thelp: help information\n");
}
