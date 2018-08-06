//
// Created by Ishay Cena on 27/07/2018.
//

#ifndef ECHO_CLIENT_FINALE_ICONNECTION_H
#define ECHO_CLIENT_FINALE_ICONNECTION_H


class IConnection {
public:
    IConnection() = default;
    /**
     * Connects to the address and port
     * @param address the ip address
     * @param port the port
     */
    virtual void Connect(const char *address, const int &port) = 0;
};


#endif //ECHO_CLIENT_FINALE_ICONNECTION_H
