/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */

#ifndef OFSUITE_PARSER_H
#define OFSUITE_PARSER_H

#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "OFsuite_handler.h"
#include "OFsuite_ofpmsgs.h"

void parse_command(int argc, char **argv);
void parse_openflow(ofp_buf *, int, int);
void parse_user_command(void);
void init_readline(void);
void OFsuite_getcmdline(char *);
char **CmdComp(const char *text, int start, int end);
char *CmdGenerator(const char *text, int state);
void OFsuite_usage(char **argv);
void user_command_help(void);
char *get_long_from_short(int);
#endif
