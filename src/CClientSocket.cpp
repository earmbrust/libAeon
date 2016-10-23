/*********************************************************************
 * libaeon - A simple, lightweight, cross platform networking library
 * Copyright 2006-2016 (c) Elden Armbrust
 * This software is licensed under the BSD software license.
 *********************************************************************/
#ifndef _CCLIENT_SOCKET_CPP
#define _CCLIENT_SOCKET_CPP
#include "libaeon.h"

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif
namespace net
{
    bool CClientSocket::Connect(const char* remote_addr, int remote_port)
    {
        this->remote_host = remote_addr;
        this->port = remote_port;
        return this->Connect();
    }
    /**
     * The Connect() method allows for a parameterless call to be made to either re-establish a
     * connection
     * or to establish a new connection on an existing, filled CClientSocket object.
     * \author Elden Armbrust
     * \return A boolean value which corresponds to whether the connection succeeded (true) or failed
     * (false).
     */
    bool CClientSocket::Connect()
    {
        struct addrinfo hints, *connection;
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC; // use AF_INET6 to force IPv6
        hints.ai_socktype = SOCK_STREAM;
        int rv;


        if (this->sockfd < 0) {
            error_code = ERR_NOSOCKET;
            error_state = SOCK_CREATE;
            return false;
        }

        if ((rv = getaddrinfo(this->remote_host.c_str(), std::to_string(this->port).c_str(), &hints, &this->server)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            exit(1);
        }

        for(connection = this->server; connection != NULL; connection = connection->ai_next) {
            if ((this->sockfd = socket(connection->ai_family, connection->ai_socktype,
                                       connection->ai_protocol)) == -1) {
                perror("socket");
                continue;
            }

            if (connect(this->sockfd, connection->ai_addr, connection->ai_addrlen) == -1) {
                perror("connect");
                _close(this->sockfd);
                continue;
            }

            break; // if we get here, we must have connected successfully
        }


        this->connected = true;
        freeaddrinfo(this->server); // all done with this structure
        this->ClearBuffers();
        return this->connected;
    }

    CClientSocket::CClientSocket()
    {
        this->sockfd = socket(CSocket::DefaultFamilyType, CSocket::StreamSocketType, 0);
    }

    CClientSocket::CClientSocket(const char* remote, int port)
    {
        this->sockfd = socket(CSocket::DefaultFamilyType, CSocket::StreamSocketType, 0);
        this->Connect(remote, port);
    }

    CClientSocket::CClientSocket(std::string* remote, int port)

    {
        this->sockfd = socket(CSocket::DefaultFamilyType, CSocket::StreamSocketType, 0);
        this->Connect(remote->c_str(), port);
    }

    CClientSocket::~CClientSocket()
    {
    }
}
#endif
