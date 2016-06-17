/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */

#ifndef OFSUITE_HANDLER_H
#define OFSUITE_HANDLER_H

#include <stdlib.h>
#include <stdint.h>
#include "openflow.h"
#include "types.h"
#include "OFsuite_buffer.h"
#include "../OFsuite_performance.h"

struct send_port {
    int s_port[MAX_PORTS];
    int total_send_port;
};


inline uint64_t htonll(uint64_t);
inline uint64_t ntohll(uint64_t);

uint8_t handle_hello(ofp_buf *, uint16_t);
uint8_t handle_echo_request(ofp_buf *, uint16_t);
uint8_t handle_features_request(ofp_buf *, uint16_t);
uint8_t handle_set_config(ofp_buf *, uint16_t);
uint8_t handle_flow_mod(ofp_buf *, uint16_t);
uint8_t handle_multipart_request(ofp_buf *, uint16_t);
uint8_t handle_mp_port_desc(ofp_buf *, uint16_t);
uint8_t handle_mp_port_stats(ofp_buf *msg_buf, uint16_t ofconn);
uint8_t handle_mp_desc(ofp_buf *msg_buf, uint16_t ofconn);
uint8_t handle_mp_flow(ofp_buf *msg_buf, uint16_t ofconn);
uint8_t handle_mp_table(ofp_buf *msg_buf, uint16_t ofconn);
uint8_t handle_mp_default(ofp_buf *msg_buf, uint16_t ofconn);
uint8_t handle_action_output(fakesw *fksw, uint16_t ofconn, struct ofp_action_output *act, ofp_buf *data_buf, int data_len, uint32_t in_port);
uint8_t handle_packet_out(ofp_buf *, uint16_t);
uint8_t handle_barrier_request(ofp_buf *msg_buf, uint16_t ofconn);
uint8_t handle_role_request(ofp_buf *msg_buf, uint16_t ofconn);
uint8_t handle_get_config_request(ofp_buf *msg_buf, uint16_t ofconn);
uint8_t handle_default(ofp_buf *msg_buf, uint16_t ofconn);
#endif