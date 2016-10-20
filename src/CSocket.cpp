/*********************************************************************
 * libaeon - A simple, lightweight, cross platform networking library
 * Copyright 2006-2016 (c) Elden Armbrust
 * This software is licensed under the BSD software license.
 *********************************************************************/
#ifndef _CSOCKET_CPP
#define _CSOCKET_CPP
#include "libaeon.h"

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif
namespace net
{

    int CSocket::operator<<(char* data)
    {
        int retVal;
        retVal = this->Write(data);
        return retVal;
    }
    /**
     * The iostream compatible << operator for writing data to the socket.
     * \author Elden Armbrust
     * \param data An std::string object containing text which is to be sent across the socket.
     * \return The number of bytes written to the socket.
     */
    int CSocket::operator<<(std::string data)
    {
        int retVal;
        retVal = this->Write(data.c_str());
        return retVal;
    }
    std::string CSocket::operator>>(std::string)
    {
        std::string retVal;
        retVal = this->Read(CSocket::MaxBufferSize);
        return retVal;
    }
    /**
     * GetState retrieves the current state of the CSocket parent.
     * \author Elden Armbrust
     * \return The current state of the socket.
     */
    int CSocket::GetState()
    {
        return this->error_state;
    }
    /**
     * GetError retrieves the current error code sotred in the CSocket object.
     * \author Elden Armbrust
     * \return The error code currently stored in the CSocket object.
     */
    int CSocket::GetError()
    {
        return this->error_code;
    }
    /**
     * SetError can set the current error code stored in the CSocket object.
     * \author Elden Armbrust
     * \param error The error code to set the error status to.
     */
    void CSocket::SetError(int error)
    {
        this->error_code = error;
    }


    CSocket::CSocket()
    {
#ifdef WIN32
        this->wsaret = WSAStartup(MAKEWORD(2, 0), &wsadata);
#endif
        this->net_family = CSocket::DefaultFamilyType;
        this->connected = false;
        this->sockfd = socket(CSocket::DefaultFamilyType, CSocket::DefaultSocketType, 0);
        if (this->sockfd < 0) {
            error_code = ERR_NOSOCKET;
            error_state = SOCK_CREATE;
            return;
        }
#ifdef __linux__
        flags = fcntl(sockfd, F_GETFL, 0);
#endif
    }

    /**
     * A CSocket constructor which allows for the selection of the connections family type when calling.
     * \author Elden Armbrust
     * \param family_type The type of connection which should be made.
     */


    /**
     * The generic destructor handles garbage collection (where available) and cleanup of data.
     */
    CSocket::~CSocket()
    {
#ifdef __linux__
        close(sockfd);
#endif
#ifdef _WIN32
        closesocket(sockfd);
        WSACleanup();
#endif
        sockfd = -1;
    }
    bool CSocket::Close()
    {
#ifdef __linux__
        if (this->connected == true)
            close(sockfd);
#endif
#ifdef _WIN32
        closesocket(sockfd);
        WSACleanup();
#endif
        this->connected = false;
        return true;
    }
    /**
     * Writes character data to the socket.
     * \author Elden Armbrust
     * \param data The character (char) array to be written to the socket.
     * \return The number of bytes written to the socket.
     */
    int CSocket::Write(char* data)
    {
        int bytesSent;
        bytesSent = send(sockfd, data, strlen(data), 0);
        return bytesSent;
    }


    /**
     * Writes character data to the socket.
     * \author Elden Armbrust
     * \param data The character (char) array to be written to the socket.
     * \return The number of bytes written to the socket.
     */
    int CSocket::Write(char* data, int size)
    {
        int bytesSent;
        bytesSent = send(sockfd, data, size * sizeof(char), CSocket::NULLFlag);
        return bytesSent;
    }

    /**
     * Writes character data to the socket.
     * \author Elden Armbrust
     * \param data The character (std::string) to be written to the socket.
     * \return The number of bytes written to the socket.
     */
    int CSocket::Write(std::string data)
    {
        int bytesSent;
        bytesSent = send(sockfd, data.c_str(), data.size() * sizeof(std::string::value_type), 0);
        return bytesSent;
    }
    /**
     * Reads character data from the socket without formatting.
     * \author Elden Armbrust
     * \param buffer A character array which will be filled with the incoming data.
     * \param size The number of bytes to be read.  This number must not be larger than the size of
     * buffer.
     * \return The number of bytes read from the socket.
     */
    int CSocket::Read(char* buffer, int size)
    {
        int bytesRead;
        this->ClearBuffer(buffer, size);
        bytesRead = recv(sockfd, buffer, size, 0);
        this->n = bytesRead;
        if (bytesRead == 0)
            this->connected = false;
        return bytesRead;
    }

    /**
     * Reads character data from the socket without formatting until size bytes are received.
     * \author Elden Armbrust
     * \param buffer A character array which will be filled with the incoming data.
     * \param size The number of bytes to be read.  This number must not be larger than the size of
     * buffer.
     * \return The number of bytes read from the socket.
     */
    int CSocket::ReadUntil(char* buffer, int size)
    {
        int bytesRead;
        std::string temp;
        char* tempbuffer;
        int totalbytes = 0;
        tempbuffer = (char*)malloc(sizeof(char) * size);
        while (totalbytes < size) {
            this->ClearBuffer(tempbuffer, size);
            bytesRead = recv(sockfd, tempbuffer, size, 0);
            this->n = bytesRead;
            if (bytesRead == 0) {
                this->connected = false;
                break;
            };
            if (bytesRead == 0)
                this->connected = false;
            totalbytes += bytesRead;
            if (bytesRead > 0) {
                temp += tempbuffer;
            };
        };
        strcpy(buffer, temp.c_str());
        return totalbytes;
    }

    int CSocket::Read()
    {
        return this->Read(inbuffer, CSocket::MaxBufferSize - 1);
    }
    /**
     * Reads character data from the socket without formatting.
     * \author Elden Armbrust
     * \param size The number of bytes to read from the the socket.
     * \return An std::string object containing bytes read.
     * \todo Complete method.
     */
    std::string CSocket::Read(int size)
    {
        int bytesRead;
        std::string retVal;
        bytesRead = recv(sockfd, this->inbuffer, CSocket::MaxBufferSize - 1, 0);
        retVal = this->inbuffer;
        this->n = bytesRead;
        if (bytesRead == 0)
            this->connected = false;
        if (bytesRead == 0)
            this->connected = false;
        return retVal;
    }

    /**
     * \internal
     */
    void CSocket::ClearBuffers()
    {
        memset(this->inbuffer, 0, CSocket::MaxBufferSize);
        memset(this->outbuffer, 0, CSocket::MaxBufferSize);
    }
    void CSocket::ClearBuffer(char* buffer, int size)
    {
        memset(buffer, 0, size);
    }


    int CSocket::ReadLine(char* buffer, int size)
    {
        int bytesRead;
        bool bCarriage = false;
        bool bLinefeed = false;
        char tempbuff[1];

        this->ClearBuffer(buffer, size);
        for (int i = 0; i < size; ++i) {
            memset(tempbuff, 0, sizeof(char));
            bytesRead = recv(sockfd, tempbuff, 1, 0);
            if (bytesRead == 0)
                this->connected = false;
            this->n += bytesRead;

            if (strcmp(tempbuff, "\r") == 0)
                bCarriage = true;
            if (strcmp(tempbuff, "\n") == 0)
                bLinefeed = true;
            if (bCarriage == true && bLinefeed == true)
                i = size;
            buffer[i] = tempbuff[0];
        }
        return this->n;
    };

    int CSocket::SetBlocking(bool flag)
    {
        if (flag == false) {
#ifdef _WIN32
            u_long mode = 1;
            ioctlsocket(sockfd, FIONBIO, &mode);
#endif
#ifdef __linux__
            fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
#endif
        } else {
#ifdef _WIN32
            u_long mode = 0;
            ioctlsocket(sockfd, FIONBIO, &mode);
#endif
#ifdef __linux__
            fcntl(sockfd, F_SETFL, flags);
#endif
        }
        this->blocking = flag;
        return this->blocking;
    }
}
#endif
