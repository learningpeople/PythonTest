/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */

#ifndef OFSUITE_SOCKET_H
#define OFSUITE_SOCKET_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>


int socket_create(void);
void socket_destroy(int);
int socket_connect(int , struct sockaddr *);
int make_socket_non_blocking(int);

#endif