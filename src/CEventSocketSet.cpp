/*********************************************************************
 * libaeon - A simple, lightweight, cross platform networking library
 * Copyright 2006-2016 (c) Elden Armbrust
 * This software is licensed under the BSD software license.
 *********************************************************************/

#ifndef _CEVENTSOCKET_SET_CPP
#define _CEVENTSOCKET_SET_CPP
#include "libaeon.h"


#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif

namespace net
{
    /**
     * \brief Adds an existing socket to the socket set
     * \author Elden Armbrust
     * \param socket_ref A pointer to a CEventSocket object
     * \return A boolean value indicating success
     *
     * Add() allows you to add an existing socket to a socket set
     * so that it may be handled as a member of the class.
     */
    bool CEventSocketSet::Add(CEventSocket* socket_ref)
    {
        this->Sockets.push_back(socket_ref);
        return true;
    }


    bool CEventSocketSet::Add()
    {
        CEventSocket* socket_ref = new CEventSocket();
        this->Sockets.push_back(socket_ref);
        return true;
    }

    /**
     * \brief Removes a selected socket from the set
     * \author Elden Armbrust
     * \param index The index id of the socket to remove from the set
     * \return A boolean value indicating success
     *
     * Remove() allows you to remove a socket from the socket set so
     * that socket set functions will not effect the socket.
     * It should be noted that Remove() functions of the CSocketSet
     * class do NOT do any garbage collection, deletion, or connection
     * closing on the sockets to be removed.  Those tasks should be done
     * prior to removal for the most simple transition.
     */
    bool CEventSocketSet::Remove(unsigned int index)
    {
        if (index > this->Sockets.size()) return false;
        this->Sockets.erase(this->Sockets.begin() + index, this->Sockets.begin() + (index + 1));
        return true;
    }

    /**
     * \brief Removes selected sockets from the set
     * \author Elden Armbrust
     * \param index The index id of the socket to remove from the set
     * \param count The number of CEventSockets to remove from the set
     * \return A boolean value indicating success
     *
     * Remove() allows you to remove a socket from the socket set so
     * that socket set functions will not effect the socket.
     * It should be noted that Remove() functions of the CSocketSet
     * class do NOT do any garbage collection, deletion, or connection
     * closing on the sockets to be removed.  Those tasks should be done
     * prior to removal for the most simple transition.
     */
    bool CEventSocketSet::Remove(unsigned int index, unsigned int count)
    {
        if (count > this->Sockets.size() || (index + count) > this->Sockets.size()) return false;
        this->Sockets.erase(this->Sockets.begin() + index, this->Sockets.begin() + (index + count));
        return true;
    }

    int CEventSocketSet::Size()
    {
        return this->Sockets.size();
    }

    void CEventSocketSet::Poll()
    {
        for (unsigned int i = 0; i < Sockets.size(); ++i)
        {
            this->Sockets[i]->Poll();
        }
    }

    void CEventSocketSet::Cleanup()
    {
        //not yet complete...
    }

}

#endif
