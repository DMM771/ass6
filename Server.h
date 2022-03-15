//324680438
//313306367
// Nitzan Fisher & David Monheit

#ifndef SERVER_H_
#define SERVER_H_
#define ZERO 0

#include "CLI.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <thread>
#include <cstring>
#include <sstream>


class ClientHandler {
public:
    virtual void handle(int clientID) = 0;

};



class SocketIo : public DefaultIO {
    int clientNum;
public:
    SocketIo(int clientNum) : clientNum(clientNum) { }

    virtual string read();
    virtual void write(string someString);
    virtual void write(float floatValue);
    virtual void read(float* x);
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


class Server {
    std::thread *t;
    int fileD;
    sockaddr_in server;
    sockaddr_in client;
    volatile bool boolStop;

public:
    Server(int port) throw(const char *);

    virtual ~Server();

    void start(ClientHandler &handlerOfClient) throw(const char *);

    void stop();
};
#endif /* SERVER_H_ */