//
// Created by Ishay Cena on 27/07/2018.
//

#ifndef ECHO_CLIENT_FINALE_CLIENT_H
#define ECHO_CLIENT_FINALE_CLIENT_H


#include "IConnection.h"
#include "SSLConnection.h"

class Client {
public:
    explicit Client(IConnection &connection, const char *address = "127.0.0.1", const int &port = 6769);
    ~Client();

    /**
     * connects to the server
     */
    void Connect();

protected:
    IConnection &m_connection;
    const char *mAddress;
    const int mPort;

    /**
     * initiates the WinSock service
     * @return if WinSock init was successful
     */
    bool InitWinSock();
};


#endif //ECHO_CLIENT_FINALE_CLIENT_H
