/******************************************************************
 * hello-world.cpp - A simple simple "Hello World" server
 * Copyright (c) 2006-2016 Elden Armbrust
 ******************************************************************/
#include <iostream>
#include <libaeon.h>
#include <signal.h>

//define the port to use.
#define PORT 2300

//to reduce requirements, basic signal handling is used in this example
//however, a more robust signal handler should most likely be used
static void signal_handler(int sig);
static net::CServerSocket *__server;
static net::CSocket *__client;

int main()
{
    std::cout << "hello-world.cpp - A simple libaeon hello world server" << std::endl;
    std::cout << "2007-2016 (c) Elden Armbrust (BSD License)" << std::endl;
    std::cout << "Press Ctrl-C to exit." << std::endl;
    net::CServerSocket *server = new net::CServerSocket();
    __server = server;
    if (server->Listen(PORT) != true) { //check to see if we can open the port
        std::cout << "Error opening socket for listening on port " << PORT << "." << std::endl;
        exit(EXIT_FAILURE); //return with failure
    };

    //here we create a new client socket to handle communications
    net::CSocket *client = new net::CSocket();
    __client = client;
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        std::cout << "Cannot create signal handler." << std::endl;
        exit(EXIT_FAILURE);
    }

    while (1) {
        client = server->Accept();  //attempt to accept the connection
        if (client->connected != false) { //check if the connection succeeded
            std::cout << "Client accepted." << std::endl;
            client->Write("Hello world!\n"); //send our hello world
            client->Close(); //close the socket and prepare for another connection
        }

    }
    exit(EXIT_SUCCESS); //exit successfully
}


static void signal_handler(int sig)
{
    //std::cout << "Got signal!" << std::endl;
    switch (sig) {
    case SIGINT:
        std::cout << "Got SIGINT.  Shutting down gracefully." << std::endl;
        __client->Close();
        __server->Close();
        std::cout << "Shutdown complete." << std::endl;
        exit(EXIT_FAILURE);
        break;
    default:
        break;
    }
};
