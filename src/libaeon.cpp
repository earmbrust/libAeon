/*********************************************************************
 * libaeon - A simple, lightweight, cross platform networking library
 * Copyright 2006-2018 (c) Elden Armbrust
 * This software is licensed under the BSD software license.
 *********************************************************************/

#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

#include "libaeon.h"

#include "CSocket.cpp"
#include "CClientSocket.cpp"
#include "CServerSocket.cpp"

#include "CSocketUDP.cpp"
#include "CEventSocket.cpp"
#include "CSocketSet.cpp"
#include "CEventSocketSet.cpp"

namespace net
{
    const char* GetLibraryVersion()
    {
        return VERSION;
    };
};
