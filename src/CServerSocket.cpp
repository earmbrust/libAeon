/*********************************************************************
 * libaeon - A simple, lightweight, cross platform networking library
 * Copyright 2006-2007 (c) Elden Armbrust
 * This software is licensed under the BSD software license.
 *********************************************************************/
#ifndef _CSERVER_SOCKET_CPP
#define _CSERVER_SOCKET_CPP
#include "libaeon.h"

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif
namespace net
{
    CServerSocket::CServerSocket() {
    }

    bool CServerSocket::Listen()
    {
        return this->Listen(this->port);
    }
    /**
     * Listens for incoming connections on a given port
     * \author Elden Armbrust
     * \param port And integer specifying the port to listen on
     * \return A boolean value indicating whether the call was a success or not.
     * \todo Requires testing
     */
    bool CServerSocket::Listen(int port)
    {
        this->port = port;
        this->server_socket = socket(CSocket::DefaultFamilyType, CSocket::DefaultSocketType, 0);
        if (this->server_socket < 0) return false;
        memset(&serv_addr, '\0', sizeof(serv_addr)); //it's anyone's guess why i was zeroing AFTER building...
        this->serv_addr.sin_family = CSocket::DefaultFamilyType;
        this->serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        this->serv_addr.sin_port = htons(port);
        int bindret = bind(server_socket, (struct sockaddr *) &this->serv_addr, sizeof(this->serv_addr));
        if (bindret < 0) return false;
        int listenret = listen(this->server_socket, 5);
        if (listenret < 0) return false;
        return true;
    }

    /**
     * Accepts an incoming connection from a remote host
     * \author Elden Armbrust
     * \return A connected CSocket object
     * \todo Complete this function.
     */
    CSocket* CServerSocket::Accept()
    {
        CSocket* sockClient = new CSocket;
        struct sockaddr_in remote_addr;
        socklen_t sin_size = sizeof(remote_addr);
        int acceptret = accept(this->server_socket, (struct sockaddr *) &sockClient->remote_addr, &sin_size);
        if (acceptret < 0) { sockClient->connected = false; return sockClient; };
        sockClient->sockfd = acceptret;
        sockClient->connected = true;
        return sockClient;
    }
}
#endif
