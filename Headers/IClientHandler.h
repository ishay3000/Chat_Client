//
// Created by Ishay Cena on 31/07/2018.
//

#ifndef INHERITANCE_CLIENT_ATTEMPT_1_ICLIENTHANDLER_H
#define INHERITANCE_CLIENT_ATTEMPT_1_ICLIENTHANDLER_H


#include <openssl/ossl_typ.h>
#include "SSLSender.h"
#include "SSLReceiver.h"

class IClientHandler {
public:
    /**
     * Handles the communication with the server using the ssl client socket
     * @param ssl the client socket
     */
    virtual void Handle(SSL *ssl) = 0;
    ~IClientHandler() = default;

protected:
    SSLSender m_sender;
    SSLReceiver m_receiver;
};


#endif //INHERITANCE_CLIENT_ATTEMPT_1_ICLIENTHANDLER_H
