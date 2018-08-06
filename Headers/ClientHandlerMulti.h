//
// Created by Ishay Cena on 31/07/2018.
//

#ifndef INHERITANCE_CLIENT_ATTEMPT_1_MULTICLIENTHANDLER_H
#define INHERITANCE_CLIENT_ATTEMPT_1_MULTICLIENTHANDLER_H


#include "IClientHandler.h"
//#include "../includes/includes.h"
#include <string>
using namespace std;

class ClientHandlerMulti : public IClientHandler {
public:
    explicit ClientHandlerMulti(string &name) :m_name(name) {

    }

    void Handle(SSL *ssl) override;

private:
    SSL *m_ssl;
//    const char *m_name = "ishay";
    const string m_name;

    /**
     * @Threadable
     * Handles receiving messages
     */
    void ListenerThread();
    /**
     * @Threadable
     * Handles sending messages
     */
    void SenderThread();

    /**
     * Sends the user's name to the server. <p>
     * Called once the Sender Thread starts
     */
    void SendName();

    /**
     * tries to register with the current name. the server will notify us if the name is already taken.
     * @return if we've registered successfully
     */
    bool Register();

    /**
     * gets the addressee name from the user input
     * @return the addressee name
     */
    std::string InputAddressee();
    /**
     * gets the message's text from the user input
     * @return the message from the user
     */
    std::string InputMessage();

    /**
     * @Synchronised using this to write messages to the screen.
     * It's synchronised because received messages are coloured green, and it might get mixed up with
     * this user's messages, resulting in an entire greened screen.
     * @param message the user's message
     * @param isReceivedMessage if this is a received message. if it IS, the message will be displayed green.
     */
    void WriteLine(string message, bool isReceivedMessage = false);
};


#endif //INHERITANCE_CLIENT_ATTEMPT_1_MULTICLIENTHANDLER_H
