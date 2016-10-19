/*********************************************************************
 * libaeon - A simple, lightweight, cross platform networking library
 * Copyright 2006-2007 (c) Elden Armbrust
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
     * The Connect() method allows for a parameterless call to be made to either re-establish a connection
     * or to establish a new connection on an existing, filled CClientSocket object.
     * \author Elden Armbrust
     * \return A boolean value which corresponds to whether the connection succeeded (true) or failed (false).
     */
    bool CClientSocket::Connect()
    {

        if (this->sockfd < 0)
        {
            error_code = ERR_NOSOCKET;
            error_state = SOCK_CREATE;
            return false;
        }

        this->server = gethostbyname(this->remote_host.c_str());
        if (this->server == NULL)
        {
            error_code = ERR_NOHOST;
            error_state = SOCK_RESOLVE;
            return false;
        }
        this->remote_ip = inet_ntoa(*((struct in_addr *)server->h_addr));
        memset((char *) &serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        //flawfinder finds issue with this call, claiming that memcpy doesn't check the bounds
        //of the dest pointer...
        memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
        serv_addr.sin_port = htons(this->port);
        connect_code = connect(sockfd, (const sockaddr*)&serv_addr, sizeof(serv_addr));
        if (connect_code < 0)
        {
            error_code = connect_code;
            error_state = SOCK_CONNECT;
            return false;
        }
        this->connected = true;
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

    CClientSocket::CClientSocket(std::string *remote, int port)

    {
        this->sockfd = socket(CSocket::DefaultFamilyType, CSocket::StreamSocketType, 0);
        this->Connect(remote->c_str(), port);
    }

    CClientSocket::~CClientSocket()
    {}
}
#endif
