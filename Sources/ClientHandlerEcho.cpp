//
// Created by Ishay Cena on 31/07/2018.
//

#include <cstdio>
#include <openssl/ssl.h>
#include "../Headers/ClientHandlerEcho.h"

void ClientHandlerEcho::Handle(SSL *ssl) {
    char input[512], *nl = nullptr;

    while (true) {
        memset(input, 0, sizeof(input));
        printf("You >> ");
        fgets(input, 511, stdin);
        nl = strchr(input, '\n');
        if (nl != nullptr){
            *nl = '\0';
        }

        Header header;
        Packet packet;
        Message message;

        // init message
        header.dataSize = strlen(input) + 1/* due to null terminator '\0' */;
        strcpy(packet.dataBuffer, input);

        message.header = header;
        message.packet = packet;

        // send message
        if(!m_sender.Send(ssl, message)){
            printf("Failed to send message\n");
        }else {
            // if we sent quit, we're not receiving an echo back
            if (!strcmp(input, "quit")){
                printf("quitting");
                break;
            }
            // receive echo
            message = m_receiver.Receive(ssl);
            printf("Server >> %s\n", message.packet.dataBuffer);
        }

    }
    // close the connection
    Socket sock = static_cast<Socket>(SSL_get_fd(ssl));
    closesocket(sock);
    SSL_free(ssl);
}
