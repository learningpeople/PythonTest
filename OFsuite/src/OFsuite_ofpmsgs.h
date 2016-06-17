/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */

#ifndef OFSUITE_OFPMSGS_H
#define OFSUITE_OFPMSGS_H

#include <stdio.h>
#include <stdlib.h>
#include "openflow.h"
#include "OFsuite_buffer.h"
#include "openflow-common.h"
#include "types.h"

#define OFP_NO_BUFFER   0xffffffff
#define OFPP_FLOOD 0xfffffffb
#define OFPCML_NO_BUFFER 0xffff
#define DESC_STR_LEN   256
#define SERIAL_NUM_LEN 32

int send_msg(uint16_t, void *, int);
int read_msg(uint16_t, ofp_buf *);
int send_echo_request(uint16_t);
void send_periodic_echo(int);
int send_hello(uint16_t);

struct ofp_arp_match {
    uint16_t type;
    uint16_t length;
    uint8_t oxm_fields[4];
    uint32_t value;
    uint8_t pad[4];
};
OFP_ASSERT(sizeof(struct ofp_arp_match) == 16);

struct ofp_match {
    uint16_t type;
    uint16_t length;
    uint8_t oxm_fields[4];
    uint32_t value;
    uint8_t pad[4];
};
OFP_ASSERT(sizeof(struct ofp_match) == 16);

struct ofp_raw_match {
    uint16_t type;
    uint16_t length;
    uint8_t pad[4];
};
OFP_ASSERT(sizeof(struct ofp_raw_match) == 8);

struct ofp_arp_packet_in {
    struct ofp_header oh;
    uint32_t buffer_id;
    uint16_t total_len;
    uint8_t reason;
    uint8_t table_id;
    uint64_t cookie;
    struct ofp_arp_match match;
};
OFP_ASSERT(sizeof(struct ofp_arp_packet_in) == 40);

struct ofp_packet_in {
    struct ofp_header oh;
    uint32_t buffer_id;
    uint16_t total_len;
    uint8_t reason;
    uint8_t table_id;
    uint64_t cookie;
    struct ofp_match match;
};
OFP_ASSERT(sizeof(struct ofp_packet_in) == 40);

struct ether_header {
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint16_t ether_type;
};

struct arp_header {
    uint16_t hw_type;
    uint16_t pro_type;
    uint8_t hw_size;
    uint8_t pro_size;
    uint16_t opt_code;
    uint8_t sender_mac[6];
    uint8_t sender_ip[4];
    uint8_t target_mac[6];
    uint8_t target_ip[4];
};

struct ofp_lldp {
    uint16_t chassis_subtype;
    uint8_t chassis_id_subtype;
    uint8_t chassis_id[21];
    uint16_t port_subtype;
    uint8_t port_id_subtype;
    uint32_t port_id;
    uint16_t time_subtype;
    uint16_t seconds;
    uint16_t end_lldpdu;
};
void gen_arp_request(struct ofp_arp_packet_in *, struct ether_header *, struct arp_header *, ofp_buf *);

void gen_arp_reply(struct ofp_arp_packet_in *, struct ether_header *, struct arp_header *, ofp_buf *);

struct ipv4_header {
    uint8_t version_length;
    uint8_t dsf;
    uint16_t total_length;
    uint16_t identification;
    uint16_t f_offset;
    uint8_t time_to_live;
    uint8_t protocol;
    uint16_t checksum;
    uint8_t source[4];
    uint8_t destination[4];
};

struct icmp {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint16_t identifier;
    uint16_t sequence_number;
};

void gen_icmp_ping_request(struct ofp_arp_packet_in *, struct ether_header *, ofp_buf *);
void gen_icmp_ping_reply();
uint16_t checksum(uint16_t *buffer, int size);

enum ofp_version {
    OFP10_VERSION = 0x01,
    OFP11_VERSION = 0x02,
    OFP12_VERSION = 0x03,
    OFP13_VERSION = 0x04,
    OFP14_VERSION = 0x05,
    OFP15_VERSION = 0x06
};



enum ofptype {
    
    /* Immutable messages. */
    OFPTYPE_HELLO                = 0,
    OFPTYPE_ERROR                = 1,
    OFPTYPE_ECHO_REQUEST         = 2,
    OFPTYPE_ECHO_REPLY           = 3,
    OFPTYPE_EXPERIMENTER         = 4,
    /* Switch configuration */
    OFPTYPE_FEATURES_REQUEST     = 5, /* Controller/switch */
    OFPTYPE_FEATURES_REPLY       = 6, /* Controller/switch */
    OFPTYPE_GET_CONFIG_REQUEST   = 7, /* Controller/switch */
    OFPTYPE_GET_CONFIG_REPLY     = 8, /* Controller/switch */
    OFPTYPE_SET_CONFIG           = 9, /* Controller/switch */





    /* Asynchronous messages. */
    OFPTYPE_PACKET_IN            = 10,
    OFPTYPE_FLOW_REMOVED         = 11,
    OFPTYPE_PORT_STATUS          = 12,
    
    
    /* Controller command messages. */
    OFPTYPE_PACKET_OUT           = 13, /* Controller/switch */
    OFPTYPE_FLOW_MOD             = 14, /* Controller/switch */
    OFPTYPE_GROUP_MOD            = 15, /* Controller/switch */
    OFPTYPE_PORT_MOD             = 16, /* Controller/switch */
    OFPTYPE_TABLE_MOD            = 17, /* Controller/switch */
    /* Multipart messages. */
    OFPTYPE_MULTIPART_REQUEST    = 18, /* Controller/switch message */
    OFPTYPE_MULTIPART_REPLY      = 19, /* Controller/switch message */


    /* Barrier messages. */
    OFPTYPE_BARRIER_REQUEST      = 20, /* Controller/switch message */
    OFPTYPE_BARRIER_REPLY        = 21, /* Controller/switch message */
    OFPTYPE_QUEUE_GET_CONFIG_REQUEST = 22,
    OFPTYPE_QUEUE_GET_CONFIG_REPLY   = 23,
    /* Controller role change request messages. */
    OFPTYPE_ROLE_REQUEST         = 24, /* Controller/switch message */
    OFPTYPE_ROLE_REPLY           = 25, /* Controller/switch message */
    /* Asynchronous message */
    OFPTYPE_GET_ASYNC_REQUEST    = 26, /* Controller/switch message */
    OFPTYPE_GET_ASYNC_REPLY      = 27, /* Controller/switch message */
    OFPTYPE_SET_ASYNC            = 28, /* Controller/switch message */

    /* Meters and rate limiters configuration messages. */
    OFPTYPE_METER_MOD            = 29, /* Controller/switch message */
    /* Controller role change event messages. */
    OFPTYPE_ROLE_STATUS          = 30, /* Async message */
    /* Asynchronous messages. */
    OFPTYPE_TABLE_STATUS         = 31, /* Async message */
    /* Request forwarding by the switch. */
    OFPTYPE_REQUESTFORWARD       = 32, /* Async message */
    /* Bundle operations (multiple messages as a single operation). */
    OFPTYPE_BUNDLE_CONTROL       = 33,
    OFPTYPE_BUNDLE_ADD_MESSAGE   = 34,
    OFPTYPE_CONTROLLER_STATUS    = 35,
    OFPTYPE_UNKNOWN              = 0Xff,
};

enum ofp_error_type {
    OFPET_HELLO_FAILED          = 0,
    OFPET_BAD_REQUEST           = 1,
    OFPET_BAD_ACTION            = 2,
    OFPET_BAD_INSTRUCTION       = 3,
    OFPET_BAD_MATCH             = 4,
    OFPET_FLOW_MOD_FAILED       = 5,
    OFPET_GROUP_MOD_FAILED      = 6,
    OFPET_PORT_MOD_FAILED       = 7,
    OFPET_TABLE_MOD_FAILED      = 8,
    OFPET_QUEUE_OP_FAILED       = 9,
    OFPET_SWITCH_CONFIG_FAILED  = 10,
    OFPET_ROLE_REQUEST_FAILED   = 11,
    OFPET_METER_MOD_FAILED      = 12,
    OFPET_TABLE_FEATURES_FAILED = 13,
    OFPET_EXPERIMENTER          = 0xffff,
};

enum ofp_bad_request_code {
    OFPBRC_BAD_VERSION = 0,
    OFPBRC_BAD_TYPE = 1,
    OFPBRC_BAD_MULTIPART = 2,
    OFPBRC_BAD_EXPERIMENTER = 3,
    OFPBRC_BAD_EXP_TYPE = 4,
    OFPBRC_EPERM = 5,
    OFPBRC_BAD_LEN = 6,
    OFPBRC_BUFFER_EMPTY = 7,
    OFPBRC_BUFFER_UNKNOWN = 8,
    OFPBRC_BAD_TABLE_ID = 9,
    OFPBRC_IS_SLAVE = 10,
    OFPBRC_BAD_PORT = 11,
    OFPBRC_BAD_PACKET = 12,
    OFPBRC_BAD_MULTIPART_BUFFER_OVERFLOW = 13,
};

enum ofp_multipart_type {
    OFPMP_DESC = 0,
    OFPMP_FLOW = 1,
    OFPMP_AGGREGATE = 2,
    OFPMP_TABLE = 3,
    OFPMP_PORT_STATS = 4,
    OFPMP_QUEUE_STATS = 5,
    OFPMP_GROUP = 6,
    OFPMP_GROUP_DESC = 7,
    OFPMP_GROUP_FEATURES = 8,
    OFPMP_METER = 9,
    OFPMP_METER_CONFIG = 10,
    OFPMP_METER_FEATURES = 11,
    OFPMP_TABLE_FEATURES = 12,
    OFPMP_PORT_DESC = 13,
    OFPMP_TABLE_DESC = 14,
    OFPMP_QUEUE_DESC = 15,
    OFPMP_FLOW_MONITOR = 16,
    OFPMP_EXPERIMENTER = 0xffff,
};

enum ofp_action_type {
    OFPAT_OUTPUT = 0,
    OFPAT_SET_VLAN_VID = 1,
    OFPAT_SET_VLAN_PCP = 2,
    OFPAT_STRIP_VLAN = 3,
    OFPAT_SET_DL_SRC = 4,
    OFPAT_SET_DL_DST = 5,
    OFPAT_SET_NW_SRC = 6,
    OFPAT_SET_NW_DST = 7,
    OFPAT_SET_NW_TOS = 8,
    OFPAT_SET_TP_SRC = 9,
    OFPAT_SET_TP_DST = 10,
    OFPAT_COPY_TTL_OUT = 11,
    OFPAT_COPY_TTL_IN = 12,
    OFPAT_SET_MPLS_LABEL = 13,
    OFPAT_SET_MPLS_TC = 14,
    OFPAT_SET_MPLS_TTL = 15,
    OFPAT_DEC_MPLS_TTL = 16,
    OFPAT_PUSH_VLAN = 17,
    OFPAT_POP_VLAN = 18,
    OFPAT_PUSH_MPLS = 19,
    OFPAT_POP_MPLS = 20,
    OFPAT_SET_QUEUE = 21,
    OFPAT_GROUP = 22,
    OFPAT_SET_NW_TTL = 23,
    OFPAT_DEC_NW_TTL = 24,
    OFPAT_SET_FIELD = 25,
    OFPAT_PUSH_PBB = 26,
    OFPAT_POP_PBB = 27,
    OFPAT_EXPERIMENTER = 0xffff,
};

//struct ofp_error_msg {
//    struct ofp_header oh;
//    uint16_t type;
//   uint16_t code;
//};
//OFP_ASSERT(sizeof(ofp_error_msg) == 12);

struct ofp_port {
    uint32_t port_no;
    uint8_t pad[4];
    uint8_t hw_addr[OFP_ETH_ALEN];
    uint8_t pad2[2];
    char name[OFP_MAX_PORT_NAME_LEN];

    uint32_t config;
    uint32_t state;

    uint32_t curr;
    uint32_t advertised;
    uint32_t supported;
    uint32_t peer;

    uint32_t curr_speed;
    uint32_t max_speed;
};
OFP_ASSERT(sizeof(struct ofp_port) == 64);

struct ofp_port_status {
    struct ofp_header header;
    uint8_t reason;
    uint8_t pad[7];
    struct ofp_port desc;
};
OFP_ASSERT(sizeof(struct ofp_port_status) == 80);

struct ofp_mp_port_stats {
    uint32_t port_no;
    uint8_t pad[4];
    uint64_t rx_packets;
    uint64_t tx_packets;
    uint64_t rx_bytes;
    uint64_t tx_bytes;
    uint64_t rx_dropped;
    uint64_t tx_dropped;
    uint64_t rx_errors;
    uint64_t tx_errors;
    uint64_t rx_frame_err;
    uint64_t rx_over_err;
    uint64_t rx_crc_err;
    uint64_t collisions;
    uint32_t duration_sec;
    uint32_t duration_nsec;
};
OFP_ASSERT(sizeof(struct ofp_mp_port_stats) == 112);

struct ofp_flow_stats {
    uint16_t length;
    uint8_t table_id;
    uint8_t pad;
    uint32_t duration_sec;
    uint32_t duration_nsec;
    uint16_t priority;
    uint16_t idle_timeout;
    uint16_t hard_timeout;
    uint16_t flags;
    uint8_t pad2[4];
    uint64_t cookie;
    uint64_t packet_count;
    uint64_t byte_count;
    struct ofp_raw_match match;
};
OFP_ASSERT(sizeof(struct ofp_flow_stats) == 56);

struct ofp_packet_out {
    struct ofp_header header;
    uint32_t buffer_id;
    uint32_t in_port;
    uint16_t actions_len;
    uint8_t pad[6];
};
OFP_ASSERT(sizeof(struct ofp_packet_out) == 24);

struct ofp_action_output {
    uint16_t type;
    uint16_t len;
    uint32_t port;
    uint16_t max_len;
    uint8_t pad[6];    
};
OFP_ASSERT(sizeof(struct ofp_action_output) == 16);

struct ofp_role_request {
    struct ofp_header header;
    uint32_t role;
    uint8_t pad[4];
    uint64_t generation_id;
};
OFP_ASSERT(sizeof(struct ofp_role_request) == 24);

struct ofp_action_header {
    uint16_t type;
    uint16_t len;
    uint8_t pad[4];
};
OFP_ASSERT(sizeof(struct ofp_action_header) == 8);

struct ofp_multipart_reply {
    uint16_t type;
    uint16_t flags;
};

enum ofp_config_flags {
    OFPC_FRAG_NORMAL = 0,
    OFPC_FRAG_DROP   = 1,
    OFPC_FRAG_REASM  = 2,
    OFPC_FRAG_MASK   = 3,
};

struct ofp_switch_config {
    struct ofp_header header;
    uint16_t flags;
    uint16_t miss_send_len;
};
OFP_ASSERT(sizeof(struct ofp_switch_config) == 12);

struct ofp_port_stats_request {
    uint32_t port_no;
    uint8_t pad[4];
};
OFP_ASSERT(sizeof(struct ofp_port_stats_request) == 8);

struct ofp_desc {
    struct ofp_header header;
    struct ofp_multipart_reply mpr;
    uint8_t pad[4];
    char mfr_desc[DESC_STR_LEN];
    char hw_desc[DESC_STR_LEN];
    char sw_desc[DESC_STR_LEN];
    char serial_num[SERIAL_NUM_LEN];
    char dp_desc[DESC_STR_LEN];
};
OFP_ASSERT(sizeof(struct ofp_desc) == 1072);

int send_port_status(uint16_t sock_fd, struct ofp_port desc, uint8_t reason);

#endif
