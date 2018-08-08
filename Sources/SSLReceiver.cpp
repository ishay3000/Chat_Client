//
// Created by Ishay Cena on 26/07/2018.
//

#include <cstdio>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include "../Headers/SSLReceiver.h"

Message SSLReceiver::Receive(SSL *sslClient) {
    Message message;
    Header header;
    Packet packet;

    header = ReceiveHeader(sslClient);
    packet = ReceivePacket(sslClient, header);
    message.header = header;
    message.packet = packet;

    return message;
}

Header SSLReceiver::ReceiveHeader(SSL *sslClient) {
    int totalBytes = HEADER_SIZE;
    int bytesReceived = 0;
    int bytesLeft = totalBytes;
    int bytes = 0;
    char buffer[HEADER_SIZE];
    Header header;

    while(bytesLeft) {
        bytes = SSL_read(sslClient, buffer, bytesLeft);
        if (bytes > 0) {
            bytesReceived += bytes;
            bytesLeft -= bytes;
        } else {
            ERR_print_errors_fp(stderr);
            exit(0);
        }
    }// finished reading header
    memcpy(&header, buffer, HEADER_SIZE);
    return header;
}

Packet SSLReceiver::ReceivePacket(SSL *sslClient, Header &header) {
    int totalBytes = header.dataSize;
    int bytesReceived = 0;
    int bytesLeft = totalBytes;
    int bytes = 0;
    char buffer[header.dataSize];
    Packet packet;

    while(bytesLeft) {
        bytes = SSL_read(sslClient, buffer, bytesLeft);
        if (bytes > 0) {
            bytesReceived += bytes;
            bytesLeft -= bytes;
        } else {
            ERR_print_errors_fp(stderr);
            exit(0);
        }
    }// finished reading packet
    memcpy(&packet, buffer, header.dataSize);
    return packet;
}
