//324680438
//313306367
// Nitzan Fisher & David Monheit

#include <unistd.h>
#include <csignal>
#include "Server.h"
#include<unistd.h>
#include <chrono>
#include <thread>

Server::Server(int port) throw(const char *) {
    fileDis = socket(AF_INET, SOCK_STREAM, 0);
    boolStop = false;
    if (fileDis < 0) {
        throw "can't open socket";
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    if (bind(fileDis, (struct sockaddr *) &server, sizeof server) < 0) {
        throw "can't bind";
    }
    int checkListen = listen(fileDis, 4);
    if (checkListen < 0)
        throw "can't listen";
}

void handler(int num) {
    std::this_thread::sleep_for(std::chrono::milliseconds(3001));
}

void Server::start(ClientHandler &handlerOfClient) throw(const char *) {
    t = new std::thread([&handlerOfClient, this]() {
        signal(SIGALRM,handler);
        while (!boolStop) {
            socklen_t clintSize = sizeof(client);
            alarm(1);
            int clientNum = accept(fileDis, (struct sockaddr *) &client, &clintSize);
            if (clientNum > 0) {
                handlerOfClient.handle(clientNum);
                close(clientNum);
            }
            alarm(0);
        }
        close(fileDis);
    });
}

void Server::stop() {
    boolStop = true;
    t->join();
}

Server::~Server() {
    delete t;
}

string SocketIo::read() {
    std::string data = "";
    char input = 'y';
    while(input != '\n') {
        recv(clientNum, &input, sizeof(char), 0);
        data += input;
    }
    return data;
}

void SocketIo::write(std::string someString) {
    send(clientNum, someString.c_str(), strlen(someString.c_str()), 0);
}

void SocketIo::write(float floatValue) {
    std::ostringstream floatStr;
    floatStr << floatValue;
    string s(floatStr.str());
    write(s);
}

void SocketIo::read(float *x) {
    recv(clientNum, x, sizeof(float), 0);
}

