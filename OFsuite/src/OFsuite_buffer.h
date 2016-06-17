/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */

#ifndef OFSUITE_BUFFER_H
#define OFSUITE_BUFFER_H 1

#include <string.h>
#define MAX_BUF_SIZE 1024000

#define ofp_buf char

void init_ofp_buf(ofp_buf *);

#endif