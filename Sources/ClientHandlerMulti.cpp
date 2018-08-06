//
// Created by Ishay Cena on 31/07/2018.
//

#include <cstdio>
#include <openssl/ssl.h>
#include <iostream>
#include "../Headers/ClientHandlerMulti.h"
#include "../includes/mingw.thread.h"
#include "../includes/mingw.mutex.h"
#define CLEAR(object) (ZeroMemory(&object, sizeof(object)))

void ClientHandlerMulti::Handle(SSL *ssl) {
    m_ssl = ssl;
    // listen for messages thread
    std::thread listenerThread(&ClientHandlerMulti::ListenerThread, this);
    // send messages from this user thread
    std::thread senderThread(&ClientHandlerMulti::SenderThread, this);

    // run both threads in background
    listenerThread.detach();
    senderThread.detach();
    // we need to wait for the user to choose to quit (sending a quit message, so senderThread)
//    senderThread.join();
}

void ClientHandlerMulti::ListenerThread() {
    char *source, *text;
    while(true){
        Message message = m_receiver.Receive(m_ssl);
        // get source
        source = message.header.source;
        // get message's text
        text = message.packet.dataBuffer;

/*         color your text in Windows console mode
         colors are 0=black 1=blue 2=green and so on to 15=white
         color attribute = foreground + background * 16
         to get red text on yellow use 4 + 14*16 = 228
         light red on yellow would be 12 + 14*16 = 236*/
        char formattedMessageBuffer[512] = {0};
        snprintf(formattedMessageBuffer, sizeof(formattedMessageBuffer), "\n%s >> %s\n", source, text);
        this->WriteLine(formattedMessageBuffer, true);
    }
}

void ClientHandlerMulti::SenderThread() {
    char msgInput[512], msgDestination[15], *nl = nullptr;

    // first we need to send our name
    this->SendName();

    // get input from user and send it until he types "quit"
    while (true) {
        memset(msgInput, 0, sizeof(msgInput));
        memset(msgDestination, 0, sizeof(msgDestination));
        Header header;// = {0};
        Packet packet;// = {0};
        Message message;// = {0};
        CLEAR(header);
        CLEAR(packet);
        CLEAR(message);

        // get addressee and message text
        strcpy(msgDestination, this->InputAddressee().data());
        strcpy(msgInput, this->InputMessage().data());

        // init header
        strcpy(header.source, m_name.data());
        strcpy(header.destination, msgDestination);
        header.dataSize = strlen(msgInput) + 1; /* + 1 due to null terminator '\0' */
        header.status = OK;
        // init packet
        strcpy(packet.dataBuffer, msgInput);
        // init message
        message.header = header;
        message.packet = packet;

        if (!strcmp(msgInput, "quit")){
            printf("Sending server QUIT status...\n");
            message.header.status = QUIT;

            // send quit message
            if(!m_sender.Send(m_ssl, message)){
                printf("Failed to send message\n");
            }
            break;
        }
        // send message
        if(!m_sender.Send(m_ssl, message)){
            printf("Failed to send message\n");
        }
    }
    // close the connection
    auto sock = static_cast<Socket>(SSL_get_fd(m_ssl));
    closesocket(sock);
    SSL_free(m_ssl);
}

std::string ClientHandlerMulti::InputAddressee() {
    char *nl;
    string name;
    this->WriteLine("Send to >> ");
    getline(cin, name);
    nl = strchr(name.data(), '\n');
    if (nl != nullptr){
        *nl = '\0';
    }
    return name;
}

std::string ClientHandlerMulti::InputMessage() {
    char *nl, cmessage[512];
    string message;
    this->WriteLine("Message >> ");
    getline(cin, message);
    nl = strchr(message.data(), '\n');
    if (nl != nullptr){
        *nl = '\0';
    }
    strcpy(cmessage, message.data());
    return message;
}

void ClientHandlerMulti::SendName() {
    Message nameMessage;// = {0};
    Header nameHeader;// = {0};
    CLEAR(nameMessage);
    CLEAR(nameHeader);
    strcpy(nameHeader.source, m_name.data());
    nameHeader.status = REGISTER;

    nameMessage.header = nameHeader;
    m_sender.Send(m_ssl, nameMessage);
}

static std::mutex writeMutex;

void ClientHandlerMulti::WriteLine(string message, bool isReceivedMessage) {
    unique_lock<mutex> lock(writeMutex);
    if (isReceivedMessage){
        // get stdout console
        HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        FlushConsoleInputBuffer(hConsole);
        // set text colour to green
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);

        cout << message;
        //set back to black background and white text
        SetConsoleTextAttribute(hConsole, 7);
    } else{
        cout << message;
    }
}

bool ClientHandlerMulti::Register() {
    return false;
}
