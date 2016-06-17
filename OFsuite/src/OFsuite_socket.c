/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */



#include "OFsuite_socket.h"
#include <unistd.h>

int socket_create(void) {
    int sockfd= socket(PF_INET, SOCK_STREAM,0);
    return sockfd;
    
}

void socket_destroy(int sockfd) {
    close(sockfd);
    shutdown(sockfd, SHUT_RDWR);
}
  
int socket_connect(int sockfd, struct sockaddr *server_addr) {
    return connect(sockfd, server_addr, sizeof(struct sockaddr));
}

int make_socket_non_blocking(int sockfd) {
  
    int flags, s;

    flags = fcntl (sockfd, F_GETFL, 0);
    if (flags < 0) {
      return flags;
    }

    flags |= O_NONBLOCK;
    s = fcntl (sockfd, F_SETFL, flags);
    if (s < 0) {
      return s;
    }

    return 0;
}