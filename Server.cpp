#include <unistd.h>
#include <csignal>
#include "Server.h"

Server::Server(int port) throw(const char *) {
    fileDis = socket(AF_INET, SOCK_STREAM, 0);
    boolStop = false;
    if (fileDis < 0) {
        throw "can't open socket";
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    //int checkBind = bind(fileDis, (struct sockaddr *) &server, sizeof server);
    if (bind(fileDis, (struct sockaddr *) &server, sizeof server) < 0) {
        throw "can't bind";
    }
    int checkListen = listen(fileDis, 3);
    if (checkListen < 0)
        throw "can't listen";
}

void handler(int sigNum) {

}

void Server::start(ClientHandler &ch) throw(const char *) {
    t = new std::thread([&ch, this]() {
        signal(SIGALRM,handler);
        while (!boolStop) {
            socklen_t clintSize = sizeof(client);
            alarm(1);
            int clientNum = accept(fileDis, (struct sockaddr *) &client, &clintSize);
            if (clientNum > 0) {
                ch.handle(clientNum);
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
    char input = 0;
    while(input != '\n') {
        recv(clientNum, &input, sizeof(char), 0);
        data += input;
    }
    return data;
}

void SocketIo::write(std::string text) {
    send(clientNum, text.c_str(), strlen(text.c_str()), 0);
}

void SocketIo::write(float f) {
    std::ostringstream floatStr;
    floatStr << f;
    string s(floatStr.str());
    write(s);
}

void SocketIo::read(float *f) {
    recv(clientNum, f, sizeof(float), 0);
}

