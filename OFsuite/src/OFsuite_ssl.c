/*
 *OFsuite_performance-Openflow SDN Controller Performance Test Tool
 */
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <sys/socket.h>
#include "OFsuite_ssl.h"
#include "../OFsuite_performance.h"

SSL_CTX* InitCTX(void) {
	const SSL_METHOD *method;
    SSL_CTX *ctx;

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    method = TLSv1_client_method();
    ctx = SSL_CTX_new(method);
    if ( ctx == NULL ) {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}

void ShowCerts(SSL* ssl) {
    X509 *cert;
    char *line;
 
    cert = SSL_get_peer_certificate(ssl); 
    if ( cert != NULL ) {
        printf("\nServer certificates:");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("\nSubject: %s", line);
        free(line);      
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("\nIssuer: %s", line);
        free(line);      
        X509_free(cert);     
    }
    else
        printf("\nNo certificates.");
}