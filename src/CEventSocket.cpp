/*********************************************************************
 * libaeon - A simple, lightweight, cross platform networking library
 * Copyright 2006-2018 (c) Elden Armbrust
 * This software is licensed under the BSD software license.
 *********************************************************************/

#ifndef _CEVENT_SOCKET_CPP
#define _CEVENT_SOCKET_CPP
#include "libaeon.h"

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

namespace net
{

    /**
     * Polls for waiting data to receive.
     * \author Elden Armbrust
     * \return A boolean value indicating whether the call was a success or not.
     *
     * Poll() receives the data queued to the socket, buffers it, then calls the
     * OnRead() member function.
     */
    bool CEventSocket::Poll()
    {
        int bytesRead;
        this->ClearBuffer(inbuffer, MaxBufferSize);
        bytesRead = recv(sockfd, inbuffer, MaxBufferSize, 0);
        this->n = bytesRead;
        return this->OnRead(inbuffer, bytesRead);
    }


    /**
     * Writes character data to the socket.
     * \author Elden Armbrust
     * \param data The character (char) array to be written to the socket.
     * \return The number of bytes written to the socket.
     */
    int CEventSocket::Write(char* data)
    {
        int bytesSent;
        bytesSent = send(sockfd, data, strlen(data), 0);
        this->OnWrite(data, strlen(data), bytesSent);
        return bytesSent;
    }

    /**
     * Writes character data to the socket.
     * \author Elden Armbrust
     * \param data The character (std::string) to be written to the socket.
     * \return The number of bytes written to the socket.
     */
    int CEventSocket::Write(std::string data)
    {
        int bytesSent;
        bytesSent = send(sockfd, data.c_str(), data.size(), 0);
        this->OnWrite(data.c_str(), data.size(), bytesSent);
        return bytesSent;
    }


    bool CEventSocket::OnRead(const char* buffer, int size)
    {
        return false;
    };

    void CEventSocket::OnWrite(const char* buffer, int size, int sentsize) {};
}

#endif
