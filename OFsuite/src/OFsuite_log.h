/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <python2.7/Python.h>

#define DEFAULT_LOG_FILE "OFsuite_performance.log"
#define DEFAULT_LOG_LEVEL LV_INFO

typedef uint8_t syslog_level;

void lv2str(syslog_level lv, char *lv_str);
void OFsuite_syslog(syslog_level lv, char *log_module, const char *format, ...)__attribute__((format(printf, 3, 4)));
void OFsuite_log_init(uint8_t m, char *filename);
int OFsuite_log(uint8_t m, double, char *);
void OFsuite_log_close(void);
void parse_log(void);
int parse_max_sw(void);
int parse_lldp_time(void);
int parse_link_event(void);
int parse_packet_out_rate(void);
int parse_flow_mod_rate(void);
int parse_flow_mod_auxi_rate(void);
int parse_conn_test(void);
int parse_intimation_time(void);
int parse_recovery_time(void);
int parse_auxiliary_channel(void);
int parse_proactive_flow(void);
int parse_datapath_switchover(void);
void plot_max_sw(PyObject *);
void plot_lldp_time(PyObject *);
void plot_link_event(PyObject *);
void plot_packet_out_rate(PyObject *);
void plot_flow_mod_rate(PyObject *);
void plot_flow_mod_auxi_rate(PyObject *);
void plot_conn_test(PyObject *);
void plot_intimation_time(PyObject *);
void plot_recovery_time(PyObject *);
void plot_auxiliary_channel(PyObject *);
void plot_proactive_flow(PyObject *);
void plot_datapath_switchover(PyObject *);

struct rate_result {
	double min_latency;
	double max_latency;
	double avg_latency;
	int rate;
};

void calc_latency(uint64_t n_line, struct rate_result *rr, FILE *file1, FILE *file2);
void calc_rate(uint64_t n_line, struct rate_result *rr, FILE *file);
float calc_packet_loss(double start_time, uint64_t *n_line, uint64_t n_packet_in, FILE *file);

enum error_type {
	NO_ERROR,
	ERROR_CREATESW,
	ERROR_EPOLL,
	ERROR_CONNECT_CONTROLLER,
	ERROR_MAKE_SOCKET_NO_BLOCKING,
	ERROR_SET_PORT_DOWN,
	ERROR_SET_HOST,
	ERROR_SET_NEW_SWITCH,
};

enum log_module {
	LOG_MAX_SW,
	LOG_LLDP_TIME,
	LOG_LINK_EVENT,
	LOG_PACKET_OUT_RATE,
	LOG_FLOW_MOD_RATE,
	LOG_FLOW_MOD_AUXI_RATE,
	LOG_CONNECTIVITY_TEST,
	LOG_INTIMATION_TIME,
	LOG_RECOVERY_TIME,
	LOG_AUXILIARY_CHANNEL,
	LOG_PROACTIVE_FLOW,
	LOG_DATAPATH_SWITCHOVER,
};

enum OFsuite_log_level {
	LV_FATAL,
	LV_ERROR,
	LV_WARN,
	LV_INFO,
	LV_DEBUG,
};