//
// Created by Ishay Cena on 27/07/2018.
//

#include "../Headers/Client.h"
#include "winsock2.h"
#include "../includes/includes.h"
#include "../Headers/ClientHandlerEcho.h"

Client::Client(IConnection &connection, const char *address, const int &port)
        : m_connection(connection), mAddress(address), mPort(port) {

}

bool Client::InitWinSock() {
    WSAData wsaData;
    WORD DllVersion = MAKEWORD(2, 1);
    if (WSAStartup(DllVersion, &wsaData) != 0) //If WSAStartup returns anything other than 0, then that means an error has occured in the WinSock Startup.
    {
        MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
        return false;
    }
    return true;
}

Client::~Client() {
    WSACleanup();
}

void Client::Connect() {
    if (InitWinSock()) {
        printf("Attempting to connect to the server on {%s : %d}", mAddress, mPort);
        m_connection.Connect(mAddress, mPort);
    } else{
        printf("[-] Could not initiate WinSock. Quitting...");
        exit(0);
    }
}

