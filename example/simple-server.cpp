/******************************************************************
 * simple-server.cpp - A simple "Hello world!" server using libaeon
 * Copyright (c) 2006-2018 Elden Armbrust
 ******************************************************************/

#include <libaeon.h>
#define SERVER_PORT 2300
class clientSocket : public net::CEventSocket
{
    bool OnRead(const char* buffer, int size);
};

bool clientSocket::OnRead(const char* buffer, int size)
{
    printf("Client said: %s\r\n", buffer);
    return true;
}

int main(void)
{
    int iConnectionCount = 0;
    net::CServerSocket* server = new net::CServerSocket();

    net::CEventSocketSet* socketset = new net::CEventSocketSet();
    if (server->Listen(SERVER_PORT) == false) {
        printf("Error when opening port.\r\n");
        delete server;
        delete socketset;
        return EXIT_FAILURE;
    }
    printf("Waiting for connection...\r\n");
    while (1) {
        socketset->Add((net::CEventSocket*)(new clientSocket));

        // clientSocket = socketset->Sockets[socketset->Size()-1];
        socketset->Sockets[socketset->Size() - 1] = (clientSocket*)server->Accept();
        if (socketset->Sockets[socketset->Size() - 1]->connected == true) {
            ++iConnectionCount;

            printf("Client %d accepted...\r\n", iConnectionCount);
        } else if (socketset->Sockets[socketset->Size() - 1]->sockfd < 0) {
            printf("Socket error!\r\n");
        }
        if (socketset->Sockets[socketset->Size() - 1]->connected == true) {
            if (socketset->Sockets[socketset->Size() - 1]->Write((char *)"Hello, world!\r\n") == -1) {
                printf("Error sending data to client!\r\n");
            }
        }
        socketset->Poll();

        // server->Close();

    }
    delete server;
    delete socketset;
}
