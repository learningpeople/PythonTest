/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */

#include "OFsuite_buffer.h"

void init_ofp_buf(ofp_buf buf[]) {
	memset(buf, 0, sizeof(MAX_BUF_SIZE));
}