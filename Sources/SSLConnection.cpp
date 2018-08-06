//
// Created by Ishay Cena on 31/07/2018.
//

#include <openssl/ssl.h>
#include <openssl/err.h>
#include "../Headers/SSLConnection.h"

SSLConnection::SSLConnection(IClientHandler &handler) : m_clientHandle(handler){

}

void SSLConnection::Connect(const char *address, const int &port) {
    if (OpenConnection(address, port)){
        if (InitSSLMethods()){
            printf("Connected with %s encryption\n", SSL_get_cipher(m_ssl));
            ShowCerts(m_ssl);        /* get any certs */
            m_clientHandle.Handle(m_ssl);
        } else{
            printf("Could not initiate SSL methods. Quitting...\n");
            exit(0);
        }
    }else {
        printf("[-] Could not connect to the specified address. Quitting...\n");
        exit(0);
    }
}

SSL_CTX *SSLConnection::InitCTX() {
    SSL_CTX *ctx;

    OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
    SSL_load_error_strings();   /* Bring in and register error messages */
    //method = TLSv1_2_client_method();  /* Create new client-method instance */
    ctx = SSL_CTX_new(SSLv3_client_method());   /* Create new context */
    if (ctx == NULL)
    {
        ERR_print_errors_fp(stdout);
        printf("Could not initiate SSL Context. Quitting...\n");
        exit(0);
//        abort();
    }
    return ctx;
}

void SSLConnection::ShowCerts(SSL *ssl) {
    X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
    if (cert != NULL)
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);

        printf("Subject: %s\n", line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);

        printf("Issuer: %s\n", line);
        X509_free(cert);     /* free the malloc'ed certificate copy */
    }
    else
        printf("Info: No client certificates configured.\n");
}

bool SSLConnection::InitSSLMethods() {
    SSL_library_init();
    m_ctx = InitCTX();
    m_ssl = SSL_new(m_ctx);
    SSL_set_fd(m_ssl, m_serverSock);
    if (SSL_connect(m_ssl) == -1) {
        ERR_print_errors_fp(stdout);
        return false;
    }
    return true;
}

bool SSLConnection::OpenConnection(const char *address, const int &port) {
    struct hostent *host;
    struct sockaddr_in addr;

    m_serverSock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((u_short) port);
    addr.sin_addr.S_un.S_addr = inet_addr(address);

    if (connect(m_serverSock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
    {
        closesocket(m_serverSock);
        perror(address);
        return false;
    }
    return true;
}