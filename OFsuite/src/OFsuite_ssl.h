/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */
#include <openssl/ssl.h>
#include <openssl/err.h>
#include "../OFsuite_performance.h"

SSL_CTX *InitCTX(void);
void ShowCerts(SSL *);