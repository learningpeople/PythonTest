/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "OFsuite_log.h"
#include "OFsuite_timer.h"
#include "../OFsuite_performance.h"
#include <python2.7/Python.h>

extern char Log_Dir[256];
extern char Python_Dir[256];
extern uint64_t arp_rate;
extern uint8_t topo_mod;
extern uint16_t n_fakesw;
extern uint8_t n_controllers;
extern uint8_t n_auxiliary_ch;
extern fakesw *p_fakesw;
extern char Project_Dir[256];
extern char Log_Dir[256];
extern char Python_Dir[256];

syslog_level LOG_LV = DEFAULT_LOG_LEVEL;

FILE *OFsuite_logfile;
FILE *OFsuite_syslogfile;
uint8_t module_log[32] = {0};
uint8_t log_type = 255;
char timeBuf[256] = {0};
uint64_t log_seq = 0;
char log_content[256] = {0};

int (*show_parse_result[32]) (void) = {
	parse_max_sw,
	parse_lldp_time,
	parse_link_event,
	parse_packet_out_rate,
	parse_flow_mod_rate,
	parse_flow_mod_auxi_rate,
	parse_conn_test,
	parse_intimation_time,
	parse_recovery_time,
	parse_auxiliary_channel,
	parse_proactive_flow,
	parse_datapath_switchover,
};

void (*py_plot[32]) (PyObject *) = {
	plot_max_sw,
	plot_lldp_time,
	plot_link_event,
	plot_packet_out_rate,
	plot_flow_mod_rate,
	plot_flow_mod_auxi_rate,
	plot_conn_test,
	plot_intimation_time,
	plot_recovery_time,
	plot_auxiliary_channel,
	plot_proactive_flow,
	plot_datapath_switchover,
};

void OFsuite_syslog(syslog_level lv, char *log_module, const char *format, ...){
	if (lv <= LOG_LV) {
		char lv_str[8] = {0};
		va_list arg_ptr;
		va_start(arg_ptr, format);
	    if (strstr(format, "%f")){
	    	double arg = va_arg(arg_ptr, double);
	    	sprintf(log_content, format, arg);
	    }
	    else {
			int arg = va_arg(arg_ptr, int);
			sprintf(log_content, format, arg);
	    }
		va_end(arg_ptr);
		lv2str(lv, lv_str);
		time_t ltime = time(NULL);
		strftime(timeBuf, sizeof(timeBuf), "%F %T", localtime(&ltime));
		setbuf(stdout, NULL);
		printf("\r[%s] |%05ld|%s|%s|%s\n", timeBuf, ++log_seq, log_module,lv_str,log_content);
		fprintf(OFsuite_syslogfile, "[%s] |%05ld|%s|%s|%s\n", timeBuf, log_seq, log_module,lv_str,log_content);
	}
}

void lv2str(syslog_level lv, char *lv_str) {
	switch(lv) {
	case LV_FATAL:
		sprintf(lv_str, "%s", "FATAL");
		break;
	case LV_ERROR:
		sprintf(lv_str, "%s", "ERROR");
		break;
	case LV_WARN:
		sprintf(lv_str, "%s", "WARN");
		break;
	case LV_INFO:
		sprintf(lv_str, "%s", "INFO");
		break;
	case LV_DEBUG:
		sprintf(lv_str, "%s", "DEBUG");
		break;
	default:
		sprintf(lv_str, "%s", "INFO");
		break;
	}
}

void OFsuite_log_init(uint8_t m, char *filename) {
	char logfile[256];
	char sys_logfile[256];
	char pcap_file[256];
	char sys_call[256];
	char *substr;
	char *cur_dir = Project_Dir;
    char *p_dir = NULL;

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
        mkdir("OFsuite_log", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        strcpy(Python_Dir, Project_Dir);
        strcat(Python_Dir, "/OFsuite_python");
        mkdir("OFsuite_python", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
    else {
        strcpy(Log_Dir, Project_Dir);
        strcat(Log_Dir, "/OFsuite_log/");
        mkdir("OFsuite_log", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        strcpy(Python_Dir, Project_Dir);
        strcat(Python_Dir, "/OFsuite_python");
        mkdir("OFsuite_python", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }

	strcpy(sys_logfile, Log_Dir);
	strcat(sys_logfile, "OFsuite_system.log");
	OFsuite_syslogfile = fopen(sys_logfile, "a+");
	if(OFsuite_syslogfile == NULL) {
		printf("Cannot open system log file, now quit!");
		exit(1);
	}
	strcpy(logfile, Log_Dir);
	strcat(logfile, filename);
	mkdir(logfile, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	strcat(logfile, "/");
	strcat(logfile, filename);
	strcpy(pcap_file, logfile);
	strcat(logfile, ".log");
	time_t ltime = time(NULL);
	strftime(timeBuf, sizeof(timeBuf), "%Y%m%d_%H%M%S", localtime(&ltime));
	strcat(pcap_file, "_");
	strcat(pcap_file, timeBuf);
	strcat(pcap_file, ".pcap");
	OFsuite_logfile = fopen(logfile, "w+");
	if (OFsuite_logfile == NULL) {
		OFsuite_syslog(LV_ERROR, "OFsuite_test_log", "Failed to open test log file");
		exit(1);
	}
	module_log[m] = 1;
	log_type = m;
	sprintf(sys_call, "sudo tcpdump -i lo -w %s -q > /dev/null 2>&1 &", pcap_file);
	system(sys_call);
	printf("\nStarting OFsuite_performance...\n");
	sleep(1);
	//fprintf(OFsuite_logfile, "\t\t\t %s \t\t\t","OFsuite_performance Log");
	//fflush(OFsuite_logfile);
}

int OFsuite_log(uint8_t m, double r, char *log_msg) {
    if (module_log[m]) {
	    if (OFsuite_logfile != NULL) {
		   double cur_time = get_msec();
		   fprintf(OFsuite_logfile, "%f %f %s\n", cur_time, r, log_msg);
		   fflush(OFsuite_logfile);
	    }
	}
	return 0;
}

void OFsuite_log_close(void) {
	fclose(OFsuite_logfile);
	fclose(OFsuite_syslogfile);
    OFsuite_logfile = NULL;
    OFsuite_syslogfile = NULL;
}

void parse_log(void) {
	char py_dir[256] = {0};
	static int is_imported = 0;
	static PyObject *pModule;
	if (show_parse_result[log_type]() != 0) {
		Py_Initialize();
		if(!is_imported) {
			is_imported = 1;
			sprintf(py_dir, "sys.path.append(\"%s\")", Python_Dir);
			PyRun_SimpleString("import sys");
			PyRun_SimpleString(py_dir);
			pModule = PyImport_ImportModule((char *)"graph_OFsuite");
		}
		if (pModule) {
			py_plot[log_type](pModule);
		}
		//Py_Finalize();
	}
}


int parse_max_sw(void) {
	int value = 0;
	char results[1024];
	char *substr;
	int result = 0;
	//fpos_t pos;
	//fgetpos(OFsuite_logfile, &pos);
	fseek(OFsuite_logfile, 0L, SEEK_SET);
	//fgets(results, 1024, OFsuite_logfile);
	while(!feof(OFsuite_logfile)) {
		fgets(results, 1024, OFsuite_logfile);
	}
	substr = strtok(results, " ");
	if(substr != NULL){
		substr = strtok(NULL, " ");
		if(substr != NULL){
			value = (int)atoi(substr);
		}
	}
	if(value) {
		result = 1;
		printf("\ncurrent switches number: %d\n", value);
	}
	else {
		printf("\nError: no valid test result, please rerun this test!\n");
	}
	return result;
}
int parse_lldp_time(void) {
	double first_value = 0;
	double cur_value = 0;
	char results[1024];
	char *substr;
	int result = 0;
	static int is_calcd = 0;
	fseek(OFsuite_logfile, 0L, SEEK_SET);
	//fgets(results, 1024, OFsuite_logfile);
	fgets(results, 1024, OFsuite_logfile);
	substr = strstr(results, "start lldp log");
	if(substr != NULL){
		substr = strtok(results, " ");
		first_value = atof(substr);
	}

	while(!feof(OFsuite_logfile)) {
		fgets(results, 1024, OFsuite_logfile);
		substr = strstr(results, "end lldp log");
		if(substr != NULL) {
			substr = strtok(results, " ");
			cur_value = atof(substr);
		}
	}
	if (first_value && cur_value) {
		result = 1;
		printf("\ntopology discovery time: %f ms\n", cur_value - first_value);
	}
	else {
		printf("\nError: no valid test result, please rerun this test!\n");
	}
	if (!is_calcd && result) {
		switch(topo_mod) {
			case 1:
			OFsuite_log(LOG_LLDP_TIME, cur_value - first_value, "linear");
			is_calcd = 1;
			break;
			case 2:
			OFsuite_log(LOG_LLDP_TIME, cur_value - first_value, "ring");
			is_calcd = 1;
			break;
			case 3:
			OFsuite_log(LOG_LLDP_TIME, cur_value - first_value, "full-mesh");
			is_calcd = 1;
			break;
			case 4:
			OFsuite_log(LOG_LLDP_TIME, cur_value - first_value, "leaf-spine");
			is_calcd = 1;
			break;
			case 5:
			OFsuite_log(LOG_LLDP_TIME, cur_value - first_value, "tree");
			is_calcd = 1;
			break;
			default:
			break;
		}
	}
	return result;
}
int parse_link_event(void) {
	double first_time = 0;
	double end_time = 0;
	char results[1024];
	int result = 0;
	char *substr;
	static int is_calcd = 0;
	fseek(OFsuite_logfile, 0L, SEEK_SET);
	//fgets(results, 1024, OFsuite_logfile);
	fgets(results, 1024, OFsuite_logfile);
	substr = strstr(results, "set link down");
	if(substr != NULL){
		substr = strtok(results, " ");
		first_time = atof(substr);
	}
	while(!feof(OFsuite_logfile)) {
		fgets(results, 1024, OFsuite_logfile);
		end_time = atof(strtok(results, " "));
	}
	if (first_time && end_time) {
		result = 1;
		OFsuite_syslog(LV_INFO,"OFsuite_test","datapath failure recovery time: %f ms", end_time - first_time);
	}
	else {
		printf("\nError: no valid test result, please rerun this test!\n");
	}
	if (!is_calcd && result) {
		OFsuite_log(LOG_LINK_EVENT, end_time - first_time, "datapath failure recovery time");
	}
	return result;
}
int parse_packet_out_rate(void) {
	uint64_t n_packet_in = 0;
	uint64_t n_packet_out = 0;
	struct rate_result rr_in;
	struct rate_result rr_out;
	double first_time = 0;
	FILE *temp_file1;
	FILE *temp_file2;
	char results[1024];
	char *substr;
	int result = 0;
	static int is_calcd = 0;
	temp_file1 = fopen("packet_in_time.tmp", "w+");
	temp_file2 = fopen("packet_out_time.tmp", "w+");
	if (temp_file1 == NULL || temp_file2 == NULL) {
		printf("error\n");
	}
	else {
		fseek(OFsuite_logfile, 0L, SEEK_SET);
		//fgets(results, 1024, OFsuite_logfile);
		while (!feof(OFsuite_logfile)) {
			fgets(results, 1024, OFsuite_logfile);
			substr = strstr(results, "send packet in");
			if (substr != NULL) {
				n_packet_in++;
				substr = strtok(results, " ");
				fprintf(temp_file1, "%s\n", substr);
			}
			substr = strstr(results, "receive packet out");
			if (substr != NULL) {
				n_packet_out++;
				substr = strtok(results, " ");
				fprintf(temp_file2, "%s\n", substr);
			}
		}
		fseek(temp_file1, 0L, SEEK_SET);
		fseek(temp_file2, 0L, SEEK_SET);
		fgets(results, 32, temp_file1);
		first_time = atof(results);
		//fclose(temp_file1);
		//fclose(temp_file2);
		//temp_file1 = fopen("packet_in_time.tmp", "r");
		//temp_file2 = fopen("packet_out_time.tmp", "r");
		if (n_packet_out < n_packet_in) {
			if (n_packet_out)
				calc_latency(n_packet_out, &rr_out, temp_file1, temp_file2);
		}
		else {
			if (n_packet_in)
				calc_latency(n_packet_in, &rr_out, temp_file1, temp_file2);
		}
		if (n_packet_in && n_packet_out) {
			float packet_loss = 0;
			calc_rate(n_packet_in, &rr_in, temp_file1);
			calc_rate(n_packet_out, &rr_out, temp_file2);
			packet_loss = calc_packet_loss(first_time, &n_packet_out, n_packet_in, temp_file2);
			result = 1;
			printf("\npacket in rate: %d packets/s\n", rr_in.rate);
			printf("\npacket out latency:"
				"\n\tmin: %f ms"
				"\n\tmax: %f ms"
				"\n\tavg: %f ms", rr_out.min_latency, rr_out.max_latency, rr_out.avg_latency);
			printf("\npacket out rate: %d packets/s\n", rr_out.rate);
			printf("\nTotal send %ld packet in, total receive %ld packet out, %%%.2f packet loss\n", n_packet_in, n_packet_out, packet_loss);
			fclose(temp_file1);
			fclose(temp_file2);
			remove("packet_in_time.tmp");
			remove("packet_out_time.tmp");
			if (!is_calcd && result) {
				is_calcd = 1;
				OFsuite_log(LOG_PACKET_OUT_RATE, rr_in.rate, "packet in rate");
				OFsuite_log(LOG_PACKET_OUT_RATE, rr_out.rate, "packet out rate");
				OFsuite_log(LOG_PACKET_OUT_RATE, rr_out.min_latency, "min latency");
				OFsuite_log(LOG_PACKET_OUT_RATE, rr_out.max_latency, "max latency");
				OFsuite_log(LOG_PACKET_OUT_RATE, rr_out.avg_latency, "avg latency");
				OFsuite_log(LOG_PACKET_OUT_RATE, packet_loss, "packet loss");	
			}
		}
		else {
			printf("\nError: no valid test result, please rerun this test!\n");
		}
	}
	return result;
}
int parse_flow_mod_rate(void) {
	uint64_t n_packet_in = 0;
	uint64_t n_flow_mod = 0;
	struct rate_result rr_in;
	struct rate_result rr_out;
	double first_time = 0;
	FILE *temp_file1;
	FILE *temp_file2;
	char results[1024];
	char *substr;
	int result = 0;
	static int is_calcd = 0;
	temp_file1 = fopen("packet_in_time.tmp", "w+");
	temp_file2 = fopen("flow_mod_time.tmp", "w+");
	if (temp_file1 == NULL || temp_file2 == NULL) {
		printf("error\n");
	}
	else {
		fseek(OFsuite_logfile, 0L, SEEK_SET);
		//fgets(results, 1024, OFsuite_logfile);
		while (!feof(OFsuite_logfile)) {
			fgets(results, 1024, OFsuite_logfile);
			substr = strstr(results, "send arp reply");
			if (substr != NULL) {
				n_packet_in++;
				substr = strtok(results, " ");
				fprintf(temp_file1, "%s\n", substr);
			}
			substr = strstr(results, "receive flow mod");
			if (substr != NULL) {
				n_flow_mod++;
				substr = strtok(results, " ");
				fprintf(temp_file2, "%s\n", substr);
			}
		}
		fseek(temp_file1, 0L, SEEK_SET);
		fseek(temp_file2, 0L, SEEK_SET);
		fgets(results, 32, temp_file1);
		first_time = atof(results);
		//fclose(temp_file1);
		//fclose(temp_file2);
		//temp_file1 = fopen("packet_in_time.tmp", "r");
		//temp_file2 = fopen("flow_mod_time.tmp", "r");
		if (n_flow_mod < n_packet_in) {
			if (n_flow_mod)
				calc_latency(n_flow_mod, &rr_out, temp_file1, temp_file2);
		}
		else {
			if (n_packet_in)
				calc_latency(n_packet_in, &rr_out, temp_file1, temp_file2);
		}
		if (n_packet_in && n_flow_mod) {
			float packet_loss = 0;
			calc_rate(n_packet_in, &rr_in, temp_file1);
			calc_rate(n_flow_mod, &rr_out, temp_file2);
			packet_loss = calc_packet_loss(first_time, &n_flow_mod, n_packet_in, temp_file2);
			result = 1;
			printf("\narp request packet in rate: %ld packets/s\n", arp_rate);
			printf("\narp reply packet in rate: %d packets/s\n", rr_in.rate);
			printf("\nflow_mod latency:"
				"\n\tmin: %f ms"
				"\n\tmax: %f ms"
				"\n\tavg: %f ms", rr_out.min_latency, rr_out.max_latency, rr_out.avg_latency);
			printf("\nflow_mod rate: %d packets/s\n", rr_out.rate);
			printf("\nTotal send %ld arp reply packet in, total receive %ld flow mod, %%%.2f packet loss\n", n_packet_in, n_flow_mod, packet_loss);
			fclose(temp_file1);
			fclose(temp_file2);
			remove("packet_in_time.tmp");
			remove("flow_mod_time.tmp");
			if (!is_calcd && result) {
				is_calcd = 1;
				OFsuite_log(LOG_FLOW_MOD_RATE, arp_rate, "arp request packet in rate");
				OFsuite_log(LOG_FLOW_MOD_RATE, rr_in.rate, "arp reply packet in rate");
				OFsuite_log(LOG_FLOW_MOD_RATE, rr_out.rate, "flow setup rate");
				OFsuite_log(LOG_FLOW_MOD_RATE, rr_out.min_latency, "min latency");
				OFsuite_log(LOG_FLOW_MOD_RATE, rr_out.max_latency, "max latency");
				OFsuite_log(LOG_FLOW_MOD_RATE, rr_out.avg_latency, "avg latency");
				OFsuite_log(LOG_FLOW_MOD_RATE, packet_loss, "packet loss");
			}
		}
		else {
			printf("\nError: no valid test result, please rerun this test!\n");
		}
	}
	return result;
}

int parse_flow_mod_auxi_rate(void) {
	uint64_t n_packet_in = 0;
	uint64_t n_flow_mod = 0;
	struct rate_result rr_in;
	struct rate_result rr_out;
	FILE *temp_file1;
	FILE *temp_file2;
	char results[1024];
	char *substr;
	int result = 0;
	static int is_calcd = 0;
	temp_file1 = fopen("packet_in_time.tmp", "w+");
	temp_file2 = fopen("flow_mod_time.tmp", "w+");
	if (temp_file1 == NULL || temp_file2 == NULL) {
		printf("error\n");
	}
	else {
		fseek(OFsuite_logfile, 0L, SEEK_SET);
		//fgets(results, 1024, OFsuite_logfile);
		while (!feof(OFsuite_logfile)) {
			fgets(results, 1024, OFsuite_logfile);
			substr = strstr(results, "send packet in");
			if (substr != NULL) {
				n_packet_in++;
				substr = strtok(results, " ");
				fprintf(temp_file1, "%s\n", substr);
			}
			substr = strstr(results, "receive flow mod");
			if (substr != NULL) {
				n_flow_mod++;
				substr = strtok(results, " ");
				fprintf(temp_file2, "%s\n", substr);
			}
		}
		fseek(temp_file1, 0L, SEEK_SET);
		fseek(temp_file2, 0L, SEEK_SET);
		//fclose(temp_file1);
		//fclose(temp_file2);
		//temp_file1 = fopen("packet_in_time.tmp", "r");
		//temp_file2 = fopen("flow_mod_time.tmp", "r");
		if (n_packet_in && n_flow_mod) {
			calc_rate(n_packet_in, &rr_in, temp_file1);
			calc_rate(n_flow_mod, &rr_out, temp_file2);
			result = 1;
			printf("\npacket in rate: %d packets/s\n", rr_in.rate);
			printf("\nflow_mod rate: %d packets/s\n", rr_out.rate);
			fclose(temp_file1);
			fclose(temp_file2);
			remove("packet_in_time.tmp");
			remove("flow_mod_time.tmp");
			if (!is_calcd && result) {
				is_calcd = 1;
				OFsuite_log(LOG_FLOW_MOD_RATE, rr_in.rate, "packet in rate");
				OFsuite_log(LOG_FLOW_MOD_RATE, rr_out.rate, "flow setup rate");
			}
		}
		else {
			printf("\nError: no valid test result, please rerun this test!\n");
		}
	}
	return result;
}

void calc_latency(uint64_t n_line, struct rate_result *rr, FILE *file1, FILE *file2) {
	uint64_t line = 0;
	double in_time;
	double out_time;
	double total_latency;
	char log_time[32];
	fseek(file1, 0L, SEEK_SET);
	fseek(file2, 0L, SEEK_SET);
	fgets(log_time, 32, file1);
	in_time = atof(log_time);
	fgets(log_time, 32, file2);
	out_time = atof(log_time);
	rr->min_latency = out_time - in_time;
	rr->max_latency = out_time - in_time;
	rr->rate = 0;
	total_latency = out_time - in_time;
	while(line < n_line - 1) {
		fgets(log_time, 32, file1);
		in_time = atof(log_time);
		fgets(log_time, 32, file2);
		out_time = atof(log_time);
		total_latency += out_time - in_time;
		if ((out_time - in_time) < rr->min_latency) {
			rr->min_latency = out_time - in_time;
		}
		if((out_time - in_time) > rr->max_latency) {
			rr->max_latency = out_time - in_time;
		}
		line++;
	}
	//printf("\nline: %ld\n", line);
	rr->avg_latency = total_latency/line;
}

void calc_rate(uint64_t n_line, struct rate_result *rr, FILE *file) {
	double last_time = 0;
	double cur_time = 0;
	double total_time = 0;
	char log_time[32];
	uint64_t line = 0;
	fseek(file, 0L, SEEK_SET);
	fgets(log_time, 32, file);
	last_time = atof(log_time);
	while(line < n_line - 1) {
		fgets(log_time, 32, file);
		cur_time = atof(log_time);
		total_time += cur_time - last_time;
		last_time = cur_time;
		line++;
	}
	//printf("\nline: %ld\n", line);
	rr->rate = (int)(1000/(total_time/(line-1)));
	if (rr->rate == 0) {
		rr->rate = 1;
	}
}

float calc_packet_loss(double start_time, uint64_t *n_line, uint64_t n_in, FILE *file) {
	float packet_loss = 0;
	double cur_time = 0;
	uint64_t line = 0;
	char log_time[32];
	fseek(file, 0L, SEEK_SET);
	while(line < *n_line) {
		fgets(log_time, 32, file);
		cur_time = atof(log_time);
		if (cur_time <= (start_time + 12000.0)) {
			line++;
		}
		else
			break;
	}
	*n_line = line;
	//printf("\nline: %ld\n", line);
	packet_loss = (1.0 - (float)line/(float)n_in)*100;
	return packet_loss;
}

int parse_conn_test(void) {
	double first_value = 0;
	double cur_value = 0;
	char results[1024];
	char *substr;
	int result = 0;
	static int is_calcd = 0;
	fseek(OFsuite_logfile, 0L, SEEK_SET);
	//fgets(results, 1024, OFsuite_logfile);
	fgets(results, 1024, OFsuite_logfile);
	first_value = atof(strtok(results, " "));
	while(!feof(OFsuite_logfile)) {
		fgets(results, 1024, OFsuite_logfile);
		substr = strstr(results, "end connectivity log");
		if(substr != NULL) {
			substr = strtok(results, " ");
			cur_value = atof(substr);
		}
	}
	if (cur_value && first_value) {
		result = 1;
		printf("\nend-to-end link create time: %f ms\n", cur_value - first_value);
		if (!is_calcd && result) {
			is_calcd = 1;
			OFsuite_log(LOG_CONNECTIVITY_TEST, n_fakesw, "total switches");
			switch(topo_mod) {
				case 1:
				OFsuite_log(LOG_CONNECTIVITY_TEST, cur_value - first_value, "linear");
				break;
				case 2:
				OFsuite_log(LOG_CONNECTIVITY_TEST, cur_value - first_value, "ring");
				break;
				case 3:
				OFsuite_log(LOG_CONNECTIVITY_TEST, cur_value - first_value, "full-mesh");
				break;
				case 4:
				OFsuite_log(LOG_CONNECTIVITY_TEST, cur_value - first_value, "leaf-spine");
				break;
				case 5:
				OFsuite_log(LOG_CONNECTIVITY_TEST, cur_value - first_value, "tree");
				break;
				default:
				break;
			}
		}
		else ;
	}
	else {
		OFsuite_syslog(LV_WARN, "OFsuite_test","No valid test result, please rerun this test!");
	}
	return result;
}

int parse_intimation_time(void) {
	double start_time = 0;
	double end_time = 0;
	double cur_time = 0;
	char results[1024];
	char *substr;
	int result = 0;
	static int is_calcd = 0;
	fseek(OFsuite_logfile, 0L, SEEK_SET);
	fgets(results, 1024, OFsuite_logfile);
	while(!feof(OFsuite_logfile)) {
		if((substr = strstr(results, "receive first role request")) != NULL) {
			substr = strtok(results, " ");
			start_time = atof(substr);
			continue;
		}
		fgets(results, 1024, OFsuite_logfile);
		substr = strtok(results, " ");
		end_time = atof(substr);
	}
	if (end_time > start_time) {
		cur_time = get_msec();
		if((cur_time - start_time) >= 50000 ) {
			result = 1;
			OFsuite_syslog(LV_INFO, "OFsuite_test", "Role intimation time: %f ms", end_time - start_time);
			if(!is_calcd) {
				is_calcd = 1;
				OFsuite_log(LOG_INTIMATION_TIME, end_time - start_time, "role intimation time");
			}
		}
		else
			OFsuite_syslog(LV_WARN, "OFsuite_test", "No enough wait time, please wait for at least 1 miniute");
	}
	else {
		OFsuite_syslog(LV_WARN,"OFsuite_test","No valid test result, please rerun this test!");
	}
	return result;
}

int parse_recovery_time(void) {
	double start_time = 0;
	double end_time = 0;
	double last_time = 0;
	double cur_time = 0;
	char results[1024];
	char *substr;
	uint8_t is_changed = 0;
	uint16_t controller_id = 0;
	//uint8_t is_master_up = 0;
	//int n_packet_out = 0;
	double recovery_time = 0;
	int result = 0;
	static int is_calcd = 0;
	fseek(OFsuite_logfile, 0L, SEEK_SET);
	//fgets(results, 1024, OFsuite_logfile);
	fgets(results, 1024, OFsuite_logfile);
	substr = strstr(results, "receive packet out");
	if(substr != NULL) {
		substr = strtok(results, " ");
		last_time = atof(substr);
		substr = strtok(NULL, " ");
		controller_id = atoi(substr);
	}
	//last_time = start_time;
	//cur_time = start_time;
	while(!feof(OFsuite_logfile)) {
		fgets(results, 1024, OFsuite_logfile);
		substr = strstr(results, "receive packet out");
		if(substr != NULL) {
			substr = strtok(results, " ");
			cur_time = atof(substr);
			start_time = last_time;
			last_time = cur_time;
			substr = strtok(NULL, " ");
			if(controller_id != atoi(substr) && !is_changed) {
				is_changed = 1;
				end_time = cur_time;
				recovery_time = end_time - start_time;
			}
		}
	}
	if (recovery_time) {
		result = 1;
		printf("\nmaster-slave recovery time: %f ms\n", recovery_time);
		if (!is_calcd && result) {
			is_calcd = 1;
			OFsuite_log(LOG_RECOVERY_TIME, n_controllers, "number of controllers");
			OFsuite_log(LOG_RECOVERY_TIME, recovery_time, "master-slave recovery time");
		}
	}
	else {
		printf("\nError: no valid test result, please rerun this test!\n");
	}
	return result;
}

int parse_auxiliary_channel(void) {
	double first_packetin = 0;
	double first_flowmod = 0;
	double packetin_time = 0;
	double flowmod_time = 0;
	uint64_t n_packetin = 0;
	uint8_t auxi_no = 0;
	uint16_t fd = 0;
	uint64_t n_flowmod[n_auxiliary_ch+1];
	char results[1024];
	char *substr;
	int result = 0;
	static int is_calcd = 0;
	memset(&n_flowmod, 0, sizeof(n_flowmod));
	fseek(OFsuite_logfile, 0L, SEEK_SET);
	while(!feof(OFsuite_logfile)) {
		fgets(results, 1024, OFsuite_logfile);
		substr = strstr(results, "send packet in");
		if(substr != NULL) {
			substr = strtok(results, " ");
			packetin_time = atof(substr);
			if (!first_packetin) {
				first_packetin = packetin_time;
			}
			if (packetin_time <= first_packetin + 2000) {
				n_packetin++;
			}
		}
		substr = strstr(results, "receive flow mod");
		if(substr != NULL) {
			substr = strtok(results, " ");
			flowmod_time = atof(substr);
			if (!first_flowmod) {
				first_flowmod = flowmod_time;
			}
			substr = strtok(NULL, " ");
			fd = atoi(substr);
			if(fd == p_fakesw->sock_fd[0]) {
				if (flowmod_time <= first_flowmod + 2000) {
					n_flowmod[0]++;
					continue;
				}
			}
			for(auxi_no = 0; auxi_no < n_auxiliary_ch; auxi_no++) {
				if(fd == p_fakesw->auxi[auxi_no].auxi_fd) {
					if (flowmod_time <= first_flowmod + 2000) {
						n_flowmod[p_fakesw->auxi[auxi_no].auxi_id]++;
						continue;
					}
				}
			}
		}
	}
	if (n_packetin) {
		result = 1;
		if(!n_auxiliary_ch) {
			printf("\nNo auxiliary connections\n");
		}
		else
			printf("\nTotal %d connections, 1 main connection, %d auxiliary connections\n", n_auxiliary_ch+1, n_auxiliary_ch);
		printf("\npacket in rate: %d packets per connection per second\n", (int)n_packetin/(2*(n_auxiliary_ch+1)));
		printf("\nflow mod rate of main connection: %d packets per second\n",(int)n_flowmod[0]/2);
		for(auxi_no = 0; auxi_no < n_auxiliary_ch; auxi_no++) {
			printf("\nflow mod rate of auxiliary connection %d: %d packets per second\n", auxi_no+1, (int)n_flowmod[auxi_no+1]/2);
		}
		if (!is_calcd && result) {
			is_calcd = 1;
			OFsuite_log(LOG_AUXILIARY_CHANNEL, n_auxiliary_ch, "number of auxiliary channel");
			OFsuite_log(LOG_AUXILIARY_CHANNEL, (int)n_packetin/(2*(n_auxiliary_ch+1)), "packet in rate");
			OFsuite_log(LOG_AUXILIARY_CHANNEL, (int)n_flowmod[0]/2, "flow mod rate of main connection");
			for (auxi_no = 0; auxi_no < n_auxiliary_ch; auxi_no++) {
				OFsuite_log(LOG_AUXILIARY_CHANNEL, (int)n_flowmod[auxi_no+1]/2, "flow mod rate of auxiliary connecion");
			}
		}
	}
	else {
		printf("\nError: no valid test result, please rerun this test!\n");
	}
	return result;
}

int parse_proactive_flow(void) {
	uint64_t line = 0;
	struct rate_result rr;
	int result = 0;
	static int is_calcd = 0;
	char log_time[32];
	fseek(OFsuite_logfile, 0L, SEEK_SET);
	while(!feof(OFsuite_logfile)) {
		fgets(log_time, 32, OFsuite_logfile);
		line++;
	}
	//printf("\nline: %ld\n", line);
	if (line >= 2) {
		calc_rate(line, &rr, OFsuite_logfile);
		printf("\nflow_mod rate: %d packets/s\n", rr.rate);
		result = 1;
		if(!is_calcd && result) {
			OFsuite_log(LOG_PROACTIVE_FLOW, rr.rate, "proactive flow mod rate");
		}
	}
	else {
		printf("\nError: no valid test result, please rerun this test!\n");
	}
	return result;
}

int parse_datapath_switchover(void) {
	double start_time = 0;
	double end_time = 0;
	int result = 0;
	static int is_calcd = 0;
	char log_time[1024];
	fseek(OFsuite_logfile, 0L, SEEK_SET);
	fgets(log_time, 1024, OFsuite_logfile);
	start_time = atof(strtok(log_time, " "));
	while(!feof(OFsuite_logfile)) {
		fgets(log_time, 1024, OFsuite_logfile);
		end_time = atof(strtok(log_time, " "));
	}
	if (start_time && end_time) {
		result = 1;
		OFsuite_syslog(LV_INFO,"OFsuite_test","datapath switchover time: %f ms", end_time - start_time);
	}
	else {
		OFsuite_syslog(LV_WARN, "OFsuite_test", "No valid test result, please rerun this test!");
	}
	if (!is_calcd && result) {
		OFsuite_log(LOG_DATAPATH_SWITCHOVER, end_time - start_time, "datapath switchover time");
	}
	return result;
}

void plot_max_sw(PyObject *pModule) {
}

void plot_lldp_time(PyObject *pModule) {
	PyObject *pDict, *pFunc, *pValue;
	char py_logfile[256];
	pDict = PyModule_GetDict(pModule);
	pFunc = PyDict_GetItemString(pDict, (char*)"graph_topologydisc");
	strcpy(py_logfile, Log_Dir);
	strcat(py_logfile, "OFsuite_topologydisc.log");
	pValue=Py_BuildValue("(z)", (char *)py_logfile);
	PyObject_CallObject(pFunc,pValue);
}
void plot_link_event(PyObject *pModule) {
	PyObject *pDict, *pFunc, *pValue;
	char py_logfile[256];
	pDict = PyModule_GetDict(pModule);
	pFunc = PyDict_GetItemString(pDict, (char*)"graph_topologyredisc");
	strcpy(py_logfile, Log_Dir);
	strcat(py_logfile, "OFsuite_topologyredisc.log");
	pValue=Py_BuildValue("(z)", (char *)py_logfile);
	PyObject_CallObject(pFunc,pValue);
}
void plot_packet_out_rate(PyObject *pModule) {
	PyObject *pDict, *pFunc, *pValue;
	char py_logfile[256];
	pDict = PyModule_GetDict(pModule);
	pFunc = PyDict_GetItemString(pDict, (char*)"graph_packetoutrate");
	strcpy(py_logfile, Log_Dir);
	strcat(py_logfile, "OFsuite_packetoutrate.log");
	pValue=Py_BuildValue("(z)", (char *)py_logfile);
	PyObject_CallObject(pFunc,pValue);
}
void plot_flow_mod_rate(PyObject *pModule) {
	PyObject *pDict, *pFunc, *pValue;
	char py_logfile[256];
	pDict = PyModule_GetDict(pModule);
	pFunc = PyDict_GetItemString(pDict, (char*)"graph_flowmodrate");
	strcpy(py_logfile, Log_Dir);
	strcat(py_logfile, "OFsuite_flowmodrate.log");
	pValue=Py_BuildValue("(z)", (char *)py_logfile);
	PyObject_CallObject(pFunc,pValue);
}
void plot_flow_mod_auxi_rate(PyObject *pModule) {

}
void plot_conn_test(PyObject *pModule) {
	PyObject *pDict, *pFunc, *pValue;
	char py_logfile[256];
	pDict = PyModule_GetDict(pModule);
	pFunc = PyDict_GetItemString(pDict, (char*)"graph_linkconnectivity");
	strcpy(py_logfile, Log_Dir);
	strcat(py_logfile, "OFsuite_linkconnectivity.log");
	pValue=Py_BuildValue("(z)", (char *)py_logfile);
	PyObject_CallObject(pFunc,pValue);
}
void plot_intimation_time(PyObject *pModule) {
}
void plot_recovery_time(PyObject *pModule) {
	PyObject *pDict, *pFunc, *pValue;
	char py_logfile[256];
	pDict = PyModule_GetDict(pModule);
	pFunc = PyDict_GetItemString(pDict, (char*)"graph_recovery_time");
	strcpy(py_logfile, Log_Dir);
	strcat(py_logfile, "OFsuite_recoverytime.log");
	pValue=Py_BuildValue("(z)", (char *)py_logfile);
	PyObject_CallObject(pFunc,pValue);
}
void plot_auxiliary_channel(PyObject *pModule) {
	PyObject *pDict, *pFunc, *pValue;
	char py_logfile[256];
	pDict = PyModule_GetDict(pModule);
	pFunc = PyDict_GetItemString(pDict, (char*)"graph_auxi_channel");
	strcpy(py_logfile, Log_Dir);
	strcat(py_logfile, "OFsuite_auxiliarychannel.log");
	pValue=Py_BuildValue("(z)", (char *)py_logfile);
	PyObject_CallObject(pFunc,pValue);
}
void plot_proactive_flow(PyObject *pModule) {
	
}
void plot_datapath_switchover(PyObject *pModule) {
	
}