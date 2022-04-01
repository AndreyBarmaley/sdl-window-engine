/***************************************************************************
 *   Copyright (C) 2017 by SWE team <sdl.window.engine@gmail.com>          *
 *                                                                         *
 *   Part of the SWE: SDL Window Engine:                                   *
 *   https://github.com/AndreyBarmaley/sdl-window-engine                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef SWE_DISABLE_NETWORK

#include "swe_tools.h"
#include "swe_cstring.h"
#include "swe_systems.h"

// SDLNet_GetLocalAddresses: SDL_net 1.2.8
#if (SDL_VERSIONNUM(SDL_NET_MAJOR_VERSION, SDL_NET_MINOR_VERSION, SDL_NET_PATCHLEVEL) < SDL_VERSIONNUM(1, 2, 8))

#ifdef __MINGW32__
#include <winsock2.h>
#include <iptypes.h>
#include <iphlpapi.h>
#undef ERROR
#undef DELETE
#else
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

int SDLNet_GetLocalAddresses(IPaddress* addresses, int maxcount)
{
    int count = 0;
#ifdef SIOCGIFCONF
    /* Defined on Mac OS X */
#ifndef _SIZEOF_ADDR_IFREQ
#define _SIZEOF_ADDR_IFREQ sizeof
#endif
    int sock;
    struct ifconf conf;
    char data[4096];
    struct ifreq* ifr;
    struct sockaddr_in* sock_addr;
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if(0 > sock)
    {
        ERROR("invalid socket");
        return 0;
    }

    conf.ifc_len = sizeof(data);
    conf.ifc_buf = (caddr_t) data;

    if(ioctl(sock, SIOCGIFCONF, &conf) < 0)
    {
        close(sock);
        ERROR("invalid ioctl");
        return 0;
    }

    ifr = (struct ifreq*)data;
    while((char*)ifr < data + conf.ifc_len)
    {
        if(ifr->ifr_addr.sa_family == AF_INET)
        {
            if(count < maxcount)
            {
                sock_addr = (struct sockaddr_in*)&ifr->ifr_addr;
                addresses[count].host = sock_addr->sin_addr.s_addr;
                addresses[count].port = sock_addr->sin_port;
            }

            ++count;
        }

        ifr = (struct ifreq*)((char*)ifr + _SIZEOF_ADDR_IFREQ(*ifr));
    }

    close(sock);
#elif defined(__WIN32__)
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter;
    PIP_ADDR_STRING pAddress;
    DWORD dwRetVal = 0;
    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO*) SDL_malloc(sizeof(IP_ADAPTER_INFO));

    if(pAdapterInfo == nullptr)
        return 0;

    if((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == ERROR_BUFFER_OVERFLOW)
    {
        pAdapterInfo = (IP_ADAPTER_INFO*) SDL_realloc(pAdapterInfo, ulOutBufLen);

        if(pAdapterInfo == nullptr)
            return 0;

        dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
    }

    if(dwRetVal == NO_ERROR)
    {
        for(pAdapter = pAdapterInfo; pAdapter; pAdapter = pAdapter->Next)
        {
            for(pAddress = &pAdapter->IpAddressList; pAddress; pAddress = pAddress->Next)
            {
                if(count < maxcount)
                {
                    addresses[count].host = inet_addr(pAddress->IpAddress.String);
                    addresses[count].port = 0;
                }

                ++count;
            }
        }
    }

    SDL_free(pAdapterInfo);
#endif
    return count;
}
#endif


#include "swe_streamnet.h"

namespace SWE
{
    StreamNetwork::StreamNetwork() : sd(nullptr), sdset(nullptr)
    {
    }

    StreamNetwork::StreamNetwork(TCPsocket sock) : sd(nullptr), sdset(nullptr)
    {
        open(sock);
    }

    StreamNetwork::StreamNetwork(const std::string & name, int port) : sd(nullptr), sdset(nullptr)
    {
        connect(name, port);
    }


    StreamNetwork::~StreamNetwork()
    {
        close();
    }

    StreamNetwork::StreamNetwork(StreamNetwork && sn) noexcept
    {
        sd = sn.sd;
        sdset = sn.sdset;
        sn.sd = nullptr;
        sn.sdset = nullptr;
    }

    StreamNetwork & StreamNetwork::operator= (StreamNetwork && sn) noexcept
    {
        close();
        sd = sn.sd;
        sdset = sn.sdset;
        sn.sd = nullptr;
        sn.sdset = nullptr;
        return *this;
    }

    bool StreamNetwork::ready(u32 timeout) const
    {
        return sd && sdset &&
            0 < SDLNet_CheckSockets(sdset, timeout) && 0 < SDLNet_SocketReady(sd);
    }

    bool StreamNetwork::connect(const std::string & name, int port)
    {
        IPaddress ip;

        if(0 > SDLNet_ResolveHost(&ip, name.size() ? name.c_str() : nullptr, port))
        {
            ERROR(SDLNet_GetError());
            return false;
        }

        return open(SDLNet_TCP_Open(&ip));
    }

    bool StreamNetwork::open(TCPsocket sock)
    {
        close();
        sd = sock;

        if(sd)
        {
            sdset = SDLNet_AllocSocketSet(1);

            if(sdset)
            {
                SDLNet_TCP_AddSocket(sdset, sd);
                return true;
            }
        }

        ERROR(SDLNet_GetError());
        return false;
    }

    bool StreamNetwork::listen(int port)
    {
        IPaddress ip;

        if(0 > SDLNet_ResolveHost(&ip, nullptr, port))
        {
            ERROR(SDLNet_GetError());
            return false;
        }

        close();
        sd = SDLNet_TCP_Open(&ip);

        if(! sd)
        {
            ERROR(SDLNet_GetError());
            return false;
        }

        return true;
    }

    void StreamNetwork::close(void)
    {
        if(sdset)
        {
            if(sd) SDLNet_TCP_DelSocket(sdset, sd);

            SDLNet_FreeSocketSet(sdset);
            sdset = nullptr;
        }

        if(sd)
        {
            SDLNet_TCP_Close(sd);
            sd = nullptr;
        }
    }

    TCPsocket StreamNetwork::accept(void)
    {
        TCPsocket sock = SDLNet_TCP_Accept(sd);
        return sock;
    }

    StringList StreamNetwork::localAddresses(void)
    {
        IPaddress addresses[4];
        StringList res;
        int count = SDLNet_GetLocalAddresses(addresses, 4);

        for(int it = 0; it < count; ++it)
        {
            res << StringFormat("%4.%3.%2.%1").
                arg(0xFF & (addresses[it].host >> 24)).
                arg(0xFF & (addresses[it].host >> 16)).
                arg(0xFF & (addresses[it].host >> 8)).
                arg(0xFF & addresses[it].host);
        }

        return res;
    }

    std::pair<std::string, int>
    StreamNetwork::peerAddress(TCPsocket sock)
    {
        std::pair<std::string, int> res;
        IPaddress* ipa = sock ? SDLNet_TCP_GetPeerAddress(sock) : nullptr;

        if(ipa)
        {
            res.second = ipa->port;
            res.first = StringFormat("%4.%3.%2.%1").
                        arg(0xFF & (ipa->host >> 24)).
                        arg(0xFF & (ipa->host >> 16)).
                        arg(0xFF & (ipa->host >> 8)).
                        arg(0xFF & ipa->host);
        }

        return res;
    }

    bool StreamNetwork::get(void* buf, size_t len) const
    {
        if(sd && buf)
        {
            size_t total = 0;
            while(total < len)
            {
                auto ptr = reinterpret_cast<u8*>(buf);
                int rcv = SDLNet_TCP_Recv(sd, ptr + total, len - total);

                if(0 >= rcv)
                {
                    setfail(true);
                    ERROR(SDLNet_GetError());
                    return false;
                }

                total += rcv;

                if(total < len)
                    SDL_Delay(1);
            }
            return true;
        }

        return false;
    }

    bool StreamNetwork::put(const void* buf, size_t len)
    {
        if(sd && buf)
        {
            int res = SDLNet_TCP_Send(sd, buf, len);
            if(res == len)
                return true;

            setfail(true);
            ERROR(SDLNet_GetError());
        }

        return false;
    }

    int StreamNetwork::get8(void) const
    {
        u8 v = 0;
        get(& v, sizeof(v));
        return v;
    }

    int StreamNetwork::getBE16(void) const
    {
        u16 v = 0;
        get(& v, sizeof(v));
        return SDL_SwapBE16(v);
    }

    int StreamNetwork::getLE16(void) const
    {
        u16 v = 0;
        get(& v, sizeof(v));
        return SDL_SwapLE16(v);
    }

    int StreamNetwork::getBE32(void) const
    {
        u32 v = 0;
        get(& v, sizeof(v));
        return SDL_SwapBE32(v);
    }

    int StreamNetwork::getLE32(void) const
    {
        u32 v = 0;
        get(& v, sizeof(v));
        return SDL_SwapLE32(v);
    }

    s64 StreamNetwork::getBE64(void) const
    {
        s64 v = 0;
        get(& v, sizeof(v));
        return SDL_SwapBE64(v);
    }

    s64 StreamNetwork::getLE64(void) const
    {
        s64 v = 0;
        get(& v, sizeof(v));
        return SDL_SwapLE64(v);
    }

    BinaryBuf StreamNetwork::get(size_t sz) const
    {
        BinaryBuf res;

        if(sz)
        {
            res.resize(sz, 0);
            get(res.data(), res.size());
        }
        else
        {
            res.reserve(1024);

            // no data, wait 10 ms
            while(ready(10))
            {
                u8 v = 0;
                if(! get(& v, sizeof(v)))
                    break;
                res.push_back(v);
            }
        }

        return res;
    }

    void StreamNetwork::put8(u8 v)
    {
        put(& v, sizeof(v));
    }

    void StreamNetwork::putBE16(u16 v)
    {
        v = SDL_SwapBE16(v);
        put(& v, sizeof(v));
    }

    void StreamNetwork::putLE16(u16 v)
    {
        v = SDL_SwapLE16(v);
        put(& v, sizeof(v));
    }

    void StreamNetwork::putBE32(u32 v)
    {
        v = SDL_SwapBE32(v);
        put(& v, sizeof(v));
    }

    void StreamNetwork::putLE32(u32 v)
    {
        v = SDL_SwapLE32(v);
        put(& v, sizeof(v));
    }

    void StreamNetwork::putBE64(u64 v)
    {
        v = SDL_SwapBE64(v);
        put(& v, sizeof(v));
    }

    void StreamNetwork::putLE64(u64 v)
    {
        v = SDL_SwapLE64(v);
        put(& v, sizeof(v));
    }
}
#endif
