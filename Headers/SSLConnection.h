//
// Created by Ishay Cena on 31/07/2018.
//

#ifndef INHERITANCE_CLIENT_ATTEMPT_1_SSLCONNECTION_H
#define INHERITANCE_CLIENT_ATTEMPT_1_SSLCONNECTION_H


#include <openssl/ossl_typ.h>
#include "IConnection.h"
#include "../includes/Data.h"
#include "IClientHandler.h"

class SSLConnection : public IConnection {
public:
    explicit SSLConnection(IClientHandler &handler);
    void Connect(const char *address, const int &port) override;

    ~SSLConnection() = default;

private:
    IClientHandler &m_clientHandle;
    SSL_CTX *m_ctx{};
    SSL *m_ssl{};
    Socket m_serverSock{};

    bool OpenConnection(const char *address, const int &port);
    void ShowCerts(SSL* ssl);
    SSL_CTX* InitCTX();
    /**
     * initiates the ssl methods in order to establish a secure connection
     * @return if we managed to initiate SSL
     */
    bool InitSSLMethods();
};


#endif //INHERITANCE_CLIENT_ATTEMPT_1_SSLCONNECTION_H
