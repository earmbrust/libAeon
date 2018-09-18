/*********************************************************************
 * libaeon - A simple, lightweight, cross platform networking library
 * Copyright 2006-2018 (c) Elden Armbrust
 * This software is licensed under the BSD software license.
 *********************************************************************/

#ifndef _CSOCKET_UDP
#define _CSOCKET_UDP
#include "libaeon.h"

namespace net
{

    CSocketUDP::CSocketUDP()
    {
#ifdef WIN32
        this->wsaret = WSAStartup(MAKEWORD( 2, 0 ), &wsadata);
#endif
        this->net_family = CSocket::DefaultFamilyType;
        this->connected = false;
        this->sockfd = socket(CSocket::DefaultFamilyType, CSocket::DatagramSocketType, 0);
        if (this->sockfd < 0) {
            error_code = ERR_NOSOCKET;
            error_state = SOCK_CREATE;
            return;
        }
    }
    /**
     * A CSocket constructor which allows for the selection of the connections family type when calling.
     * \author Elden Armbrust
     * \param family_type The type of connection which should be made.
     */
    CSocket::CSocket(int family_type)
    {
#ifdef WIN32
        this->wsaret = WSAStartup(MAKEWORD( 2, 0 ), &wsadata);
#endif
        this->net_family = family_type;
        this->connected = false;
    }

    int CSocketUDP::Write(char* data, int size)
    {
        int bytesSent;
        bytesSent = sendto(sockfd, data, size, CSocket::NULLFlag, (struct sockaddr *) &this->remote_addr, sizeof (struct sockaddr_in));
        return bytesSent;
    }

    int CSocketUDP::Write(char* data)
    {
        int bytesSent;
        bytesSent = this->Write(data, strlen(data)*sizeof(char));
        return bytesSent;
    }

    int CSocketUDP::Write(std::string data)
    {
        int bytesSent;
        bytesSent = sendto(sockfd, data.c_str(), data.size()*sizeof(std::string::value_type), CSocket::NULLFlag, (struct sockaddr *) &this->remote_addr, sizeof (struct sockaddr_in));
        return bytesSent;
    }

    int CSocketUDP::Read(char* buffer, int size)
    {
        int bytesRead;
        this->ClearBuffer(buffer, size);
        socklen_t sockaddr_size = sizeof(struct sockaddr_in);
        bytesRead = (int)recvfrom(sockfd, buffer, size, CSocket::NULLFlag, (struct sockaddr *) &this->remote_addr, &sockaddr_size);
        this->n = bytesRead;
        if (bytesRead == 0) this->connected = false;
        return bytesRead;
    }

    std::string CSocketUDP::Read(int size)
    {
        int bytesRead;
        socklen_t sockaddr_size = sizeof(struct sockaddr_in);
        bytesRead = (int)recvfrom(sockfd, inbuffer, size, CSocket::NULLFlag, (struct sockaddr *) &this->remote_addr, &sockaddr_size);
        std::string retVal(inbuffer);
        this->n = bytesRead;
        if (bytesRead == 0) this->connected = false;
        return retVal;
    }

    int CSocketUDP::Read()
    {
        return this->Read(inbuffer, CSocket::MaxBufferSize-1);
    }


}

#endif
