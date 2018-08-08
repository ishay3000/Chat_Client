//
// Created by Ishay Cena on 31/07/2018.
//

#include <cstdio>
#include <openssl/ssl.h>
#include <iostream>
#include "../Headers/ClientHandlerMulti.h"
#include "../includes/mingw.thread.h"
#include "../includes/mingw.mutex.h"

#define CLEAR(object) (ZeroMemory(&(object), sizeof(object)))

ClientHandlerMulti::ClientHandlerMulti(string &name) : m_name(name) {

}

void ClientHandlerMulti::Handle(SSL *ssl) {
    m_ssl = ssl;

    // register name first
    Register();

    // listen for messages thread
    std::thread listenerThread(&ClientHandlerMulti::ListenerThread, this);
    // send messages from this user thread
    std::thread senderThread(&ClientHandlerMulti::SenderThread, this);
    // run both threads in background
    listenerThread.detach();
    senderThread.detach();
}

void ClientHandlerMulti::ListenerThread() {
    char *source, *text;
    while (true) {
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
    char msgInput[512], msgDestination[15]; //, *newlinePtr = nullptr;
    Header header;
    Packet packet;
    Message message;

    // get input from user and send it until he types "quit"
    while (true) {
        CLEAR(msgInput);
        CLEAR(msgDestination);
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

        if (!strcmp(msgInput, "quit")) {
            printf("Sending server QUIT status...\n");
            message.header.status = QUIT;

            // send quit message
            if (!m_sender.Send(m_ssl, message)) {
                printf("Failed to send message\n");
            }
            break;
        }
        // send message
        if (!m_sender.Send(m_ssl, message)) {
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
    if (nl != nullptr) {
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
    if (nl != nullptr) {
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
    if (isReceivedMessage) {
        // get stdout console
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        FlushConsoleInputBuffer(hConsole);
        // set text colour to green
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);

        cout << message;
        //set back to black background and white text
        SetConsoleTextAttribute(hConsole, 7);
    } else {
        cout << message;
    }
}

void ClientHandlerMulti::Register() {
    string name;
    Header header;
    Packet packet = {};
    Message responseMessage;
    bool flName = true;

    while (flName) {
        printf("Enter your name (4-14 characters long) >> ");
        getline(std::cin, name);
        strcpy(header.source, name.data());
        header.dataSize = 0;
        header.status = REGISTER;

        if (m_sender.Send(m_ssl, Message{header, packet})) {
            // if message was sent to the server, let's receive whether the name is not taken
            responseMessage = m_receiver.Receive(m_ssl);
            if (responseMessage.header.status == OK) {
                // if name is ok, it means that the name isn't taken
                // and we can exit the loop.
                printf("Register: Registered successfully!\n");
                flName = false;
            } else {
                printf("Register: Error, the name %s is taken.\n", name.data());
            }
        }
    }
}

