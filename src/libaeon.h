/*********************************************************************
 * libaeon - A simple, lightweight, cross platform networking library
 * Copyright 2006-2016 (c) Elden Armbrust
 * This software is licensed under the BSD software license.
 *********************************************************************/

#ifndef _LIBAEON_H
#define _LIBAEON_H
/*!
 \file libaeon.h
 \author Elden Armbrust
 \brief The main libaeon include file.
 libaeon.h is the main include file for both libaeon as well as
 developers looking to develop against libaeon.
 \verbinclude documentation.h
 */


#ifdef WIN32
    #define WIN32_LEAN_AND_MEAN
    #define _CRT_SECURE_NO_DEPRECATE 1
    #include <winsock2.h>
    #include <ws2tcpip.h>    
    typedef int socklen_t;
#endif
#ifdef __linux__
    #include <sys/socket.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
    #include <sys/types.h>
    #include <unistd.h>
#endif

//multi-platform includes
#include <io.h>
#include <fcntl.h>
#include <string>
#include <vector>
#include <string.h>
#include <stdlib.h>

//generic error, state, and name defines.  *deprecating*
//define states
#define SOCK_RESOLVE 1
#define SOCK_ACCEPT 3
#define SOCK_CONNECT 4
#define SOCK_CREATE 2
//define codes
#define ERR_NONE 0
#define ERR_NOHOST 1
#define ERR_NOSOCKET 2
//various defines
//#define BUF_SIZE 256
#define AEON 1

//#define DEFAULT_NET AF_INET

/**
 * \namespace net
 * \author Elden Armbrust
 * \brief The container namepsace for all libaeon related classes and methods
 *
 * The net namespace is designed to encapsulate all of the network communications of
 * libaeon to prevent name collision with any other similar implementations, methods, or functions.
 * libaeon incorporates a unique namespace (to the STL) to prevent name collisions among
 * its classes and methods. This is done to create a more familiar feel for developers
 * utilizing libaeon.
 */
namespace net
{
    const char* GetLibraryVersion();

    /**
    * \class CSocket
    * \brief A generic socket class
    * \author Elden Armbrust
    *
    * The CSocket class is a generic socket implementation and is not designed to be used directly,
    * however, it may be inherited by a custom class to extend it's abilities.
    */
    class CSocket
    {
    public:
        static const int MaxBufferSize = 256;
        static const int StreamSocketType = SOCK_STREAM;
        static const int DatagramSocketType = SOCK_STREAM;
        static const int DefaultSocketType = CSocket::StreamSocketType;


        static const int DefaultFamilyType = AF_INET;
        int SetBlocking(bool flag);
        static const int NULLFlag = 0;
        int Write(char* data, int size);
        int Write(char* data);
        int Write(std::string data);
        int Read();
        int Read(char* buffer, int size);
        int ReadLine(char* buffer, int size);
        int ReadUntil(char* buffer, int size);
        std::string Read(int size);
        CSocket();
        CSocket(int family_type);
        ~CSocket();
        bool Close();
        int operator<<(char* data);
        int operator<<(std::string data);
        std::string operator>>(std::string);
        int sockfd, n;
        struct sockaddr_in remote_addr;
        bool connected;
    protected:
        int flags;
        int net_family;
        int token_size;
        bool blocking;
        char inbuffer[CSocket::MaxBufferSize];
        char outbuffer[CSocket::MaxBufferSize];
        std::string remote_host;
        std::string remote_ip;
        int connect_code;
        int error_code;
        int error_state;
        int GetState();
        int GetError();
        void SetError(int error);

        int port;

        void ClearBuffers();
        void ClearBuffer(char* buffer, int size);
#ifdef WIN32
        int wsaret;
        WSADATA wsadata;
#endif
    };


    /**
     * \brief A socket class to handle client based communications.
     * \author Elden Armbrust
     *
     * CClientSocket is a multipurpose TCP socket class designed to handle most, if not all, unencrypted
     * client communications over a connection.  It inherits from the CSocket class, while extending
     * its functionality to handle client system communications.
     */
    class CClientSocket : public CSocket
    {
    public:
        bool Connect();
        bool Connect(const char* remote, int port);
        CClientSocket();
        CClientSocket(std::string *remote, int port);
        CClientSocket(const char* remote, int port);
        ~CClientSocket();
    protected:
        struct sockaddr_in serv_addr;
        struct addrinfo *server;
    };

    /**
     * \brief A socket class to handle server based communications.
     * \author Elden Armbrust
     *
     * CServerSocket is a multipurpose TCP socket class designed to handle most, if not all, unencrypted
     * server connection handling over a connection.
     */
    class CServerSocket : public CSocket
    {
    public:
        CServerSocket();
        ~CServerSocket();
        bool Listen();
        bool Listen(int port);
        CSocket* Accept();
    protected:
        struct sockaddr_in serv_addr;
        struct hostent *server;
        int server_socket;
    };



    /**
      * \brief Inherited class allowing for polled updates.
      * \author Elden Armbrust
      *
      * CEventSocket is the base class for the CEvent* socket classes.
      * CEventSocket can be used to poll for data on the connection,
      * and when data arrives will call the OnRead() member function.
      * \note
      * CEventSocket and it's inherited classes should never be used directly.
      * They should be inherited and the OnRead() and/or OnWrite() member
      * functions overridden.
      */
    class CEventSocket: public CSocket
    {
    public:
        bool OnRead(const char* buffer, int size);
        void OnWrite(const char* buffer, int size, int sentsize);
        int Write(char* data);
        int Write(std::string data);
        bool Poll();

    };

    /**
      * \brief Inherited client class allowing for polled updates.
      * \author Elden Armbrust
      *
      * CEventClientSocket is the class equivalent to CClientSocket which
      * can be used to poll for data on the connection.
      * When data is polled, the OnRead() member function will be called.
      * When data is written the OnWrite member function will be called.
      * \note
      * CEventClientSocket should never be used directly.
      * CEventClientSocket should be inherited and the OnRead() and/or
      * OnWrite() member functions overridden.
      */
    class CEventClientSocket: public CClientSocket
        {};


    /**
      * \brief Inherited server class allowing for polled updates.
      * \author Elden Armbrust
      *
      * CEventServerSocket is the class equivalent to CServerSocket which
      * can be used to poll for data on the connection.
      * When data is polled, the OnRead() member function will be called.
      * When data is written the OnWrite member function will be called.
      * \note
      * CEventServerSocket should never be used directly.
      * CEventServerSocket should be inherited and the OnRead() and/or
      * OnWrite() member functions overridden.
      */
    class CEventServerSocket: public CServerSocket, public CEventSocket
        {};


    /**
      * \brief A class to encapsulate multiple CSocket objects
      * \author Elden Armbrust
      *
      * CSocketSet is a CSocket container class for handling sockets in an ordered
      * fashion.
      */
    class CSocketSet
    {
    public:
        std::vector<CSocket*> Sockets;
        bool Add(CSocket* socket_ref);
        bool Add();
        bool Remove(unsigned int index);
        bool Remove(unsigned int index, unsigned int count);
        int Size();
    };

    /**
      * \brief A class to encapsulate multiple CEventSocket objects
      * \author Elden Armbrust
      *
      * CEventSocketSet is a CEventSocket container class for handling sockets in an ordered
      * fashion.
      */
    class CEventSocketSet
    {
    public:
        std::vector<CEventSocket*> Sockets;
        bool Add(CEventSocket* socket_ref);
        bool Add();
        bool Remove(unsigned int index);
        bool Remove(unsigned int index, unsigned int count);
        int Size();
        void Poll();
        void Cleanup();

    };

    class CSocketUDP : public CSocket
    {
    public:
    CSocketUDP();
        int Write(char* data, int size);
        int Write(char* data);
        int Write(std::string data);
        int Read();
        int Read(char* buffer, int size);
        int ReadUntil(char* buffer, int size);
        std::string Read(int size);
        static const int DefaultSocketType = SOCK_DGRAM;
    };


}
#endif
