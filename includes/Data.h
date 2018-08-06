//
// Created by Ishay Cena on 22/07/2018.
//

#ifndef ECHO_CLIENT_FINALE_DATA_H
#define ECHO_CLIENT_FINALE_DATA_H

#include <winsock2.h>

typedef SOCKET Socket;

typedef enum Status{
    OK, REGISTER, QUIT, ERR_FORCE_QUIT, ERR_REGISTER
//    OK, ERR, QUIT, USERS_LIST, REGISTER
}Status;
constexpr int MAX_NAME_SIZE = 15;

#pragma pack(push, 1)
typedef struct Header{
    // packet size
    int dataSize;

    // sender's name
    char source[MAX_NAME_SIZE];

    // destination user's name
    char destination[MAX_NAME_SIZE];

    // TODO consider using a message status
    Status status = OK;
}Header;

typedef struct Packet{
    char dataBuffer[512];
}Packet;

typedef struct Message{
    Header header;
    Packet packet;
}Message;
#pragma pack(pop)

const size_t HEADER_SIZE = sizeof(Header);

#endif //ECHO_SERVER_DATA_H
