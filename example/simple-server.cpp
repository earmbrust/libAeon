/******************************************************************
 * simple-server.cpp - A simple "Hello world!" server using libaeon
 * Copyright (c) 2006-2016 Elden Armbrust
 ******************************************************************/

#include <libaeon.h>
#define SERVER_PORT 2300
class childsock : public net::CEventSocket
{
    bool OnRead(const char* buffer, int size);
};

bool childsock::OnRead(const char* buffer, int size)
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
    //  childsock* client =
    // childsock* client;
    while (1) {
        socketset->Add((net::CEventSocket*)(new childsock));

        // childsock = socketset->Sockets[socketset->Size()-1];
        socketset->Sockets[socketset->Size() - 1] = static_cast<childsock*>(server)->Accept();
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
            printf("Sent message.\r\n");
            // socketset->Sockets[socketset->Size()-1]->Close();
            // printf("Connection closed.\r\n");
            // socketset->Sockets[socketset->Size()-1]->connected = false;
        }
        socketset->Poll();

        server->Close();
        delete server;
        delete socketset;
    }
}
