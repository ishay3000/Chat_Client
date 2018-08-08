//
// Created by Ishay Cena on 31/07/2018.
//

#include <iostream>
#include "../Headers/Client.h"
#include "../Headers/ClientHandlerMulti.h"

#define addr "127.0.0.1"
#define ip 6769

/*class Foo{
public:
    virtual void Test() = 0;
};

class Bar : public Foo{
    void Test() override{
        printf("Bar!");
    }
};*/

int main(){
//    ClientHandlerEcho handler;
    string name;
/*    while(name.size() <= 3 || name.size() >= 15){
        printf("Enter your name (4-14 characters long) >> ");
        getline(std::cin, name);
    }*/
    ClientHandlerMulti handler(name);
    SSLConnection connection(handler);
    Client client(connection, addr);
    client.Connect();

    while(true);

// TODO consider converting from References to Pointers, thus successfully creating two subclasses to "server" class
//    Foo *foo;
//    foo = new Bar;
//
//    foo->Test();
//    delete foo;
}
