#ifndef SERVER_H_
#define SERVER_H_

#include "CLI.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <thread>
#include <cstring>
#include <sstream>


struct sockAddress {
    uint8_t sin_len;
    sa_family_t family;
    in_port_t port;
    struct in_addr address;
    char sin_zero[8];
};

//class SocketIO : public DefaultIO {
//    int clientNum;
//public:
//    SocketIO(int clientNum) : clientNum(clientNum) {};
//};

// edit your ClientHandler interface here:
class ClientHandler {
public:
    virtual void handle(int clientID) = 0;

};


// you can add helper classes here and implement on the cpp file

class SocketIo : public DefaultIO {
    int clientNum;
public:
    SocketIo(int clientNum) : clientNum(clientNum) { }

    virtual string read();
    virtual void write(string str);
    virtual void write(float f);
    virtual void read(float* f);
};
// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler : public ClientHandler {
public:
    virtual void handle(int clientID) {
        SocketIo io(clientID);
        CLI cli(&io);
        cli.start();
    }
};


// implement on Server.cpp
class Server {
    std::thread *t; // the thread to run the start() method in
    int fileDis;
    sockaddr_in server;
    sockaddr_in client;
    int clientNum;
    volatile bool boolStop;
    // you may add data members

public:
    Server(int port) throw(const char *);

    virtual ~Server();

    void start(ClientHandler &ch) throw(const char *);

    void stop();
};
#endif /* SERVER_H_ */