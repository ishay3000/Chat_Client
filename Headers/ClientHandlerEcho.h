//
// Created by Ishay Cena on 31/07/2018.
//

#ifndef INHERITANCE_CLIENT_ATTEMPT_1_ECHOCLIENTHANDLER_H
#define INHERITANCE_CLIENT_ATTEMPT_1_ECHOCLIENTHANDLER_H


#include "IClientHandler.h"

class ClientHandlerEcho : public IClientHandler {
public:
    void Handle(SSL *ssl) override;

};


#endif //INHERITANCE_CLIENT_ATTEMPT_1_ECHOCLIENTHANDLER_H
