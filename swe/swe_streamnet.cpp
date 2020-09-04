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
#endif

int SDLNet_GetLocalAddresses(IPaddress* addresses, int maxcount)
{
    int count = 0;
#ifdef SIOCGIFCONF
    /* Defined on Mac OS X */
#ifndef _SIZEOF_ADDR_IFREQ
#define _SIZEOF_ADDR_IFREQ sizeof
#endif
    SOCKET sock;
    struct ifconf conf;
    char data[4096];
    struct ifreq* ifr;
    struct sockaddr_in* sock_addr;
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if(sock == INVALID_SOCKET)
        return 0;

    conf.ifc_len = sizeof(data);
    conf.ifc_buf = (caddr_t) data;

    if(ioctl(sock, SIOCGIFCONF, &conf) < 0)
    {
        closesocket(sock);
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

    closesocket(sock);
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
        const size_t chunk = 10;

        if(0 < timeout)
        {
            for(size_t it = 0; it < timeout; it += chunk)
            {
                bool res = sd && sdset && 0 < SDLNet_CheckSockets(sdset, 1) && 0 < SDLNet_SocketReady(sd);

                if(res) return true;

                Tools::delay(chunk);
            }
        }
        else
            return sd && sdset && 0 < SDLNet_CheckSockets(sdset, 1) && 0 < SDLNet_SocketReady(sd);

        return false;
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

    int StreamNetwork::recv(char* buf, int len) const
    {
        int total = 0;

        if(sd && buf && 0 < len)
        {
            int rcv;
            int bufsz = len;

            while((rcv = SDLNet_TCP_Recv(sd, buf, bufsz)) > 0 && rcv <= bufsz)
            {
                buf   += rcv;
                bufsz -= rcv;
                total += rcv;
            }

            if(total != len) setfail(true);
        }

        return total;
    }

    int StreamNetwork::send(const char* buf, int len)
    {
        int snd = 0;

        if(sd && buf && 0 < len)
        {
            snd = SDLNet_TCP_Send(sd, buf, len);

            if(snd != len) setfail(true);
        }

        return snd;
    }

    int StreamNetwork::get8(void) const
    {
        if(sd)
        {
            u8 ch = 0;

            if(1 != SDLNet_TCP_Recv(sd, & ch, 1))
                setfail(true);

            return ch;
        }

        return 0;
    }

    int StreamNetwork::getBE16(void) const
    {
        return (get8() << 8) | get8();
    }

    int StreamNetwork::getLE16(void) const
    {
        return get8() | (get8() << 8);
    }

    int StreamNetwork::getBE32(void) const
    {
        int hh = getBE16();
        int ll = getBE16();
        return (hh << 16) | ll;
    }

    int StreamNetwork::getLE32(void) const
    {
        int ll = getLE16();
        int hh = getLE16();
        return (hh << 16) | ll;
    }

    s64 StreamNetwork::getBE64(void) const
    {
        s64 hh = getBE32();
        s64 ll = getBE32();
        return (hh << 32) | ll;
    }

    s64 StreamNetwork::getLE64(void) const
    {
        s64 ll = getBE32();
        s64 hh = getBE32();
        return (hh << 32) | ll;
    }

    BinaryBuf StreamNetwork::get(size_t sz) const
    {
        BinaryBuf res;

        if(sz)
        {
            res.resize(sz);
            int rcv = recv(reinterpret_cast<char*>(res.data()), res.size());
            res.resize(rcv);
        }
        else
        {
            while(ready())
            {
                const size_t packet = 1024;
                size_t bufsz = res.size();
                res.resize(bufsz + packet);
                int rcv = recv(reinterpret_cast<char*>(res.data() + bufsz), packet);

                if(rcv < packet)
                    res.resize(0 < rcv ? bufsz + rcv : bufsz);

                if(fail()) break;
            }
        }

        return res;
    }

    void StreamNetwork::put8(char ch)
    {
        if(sd)
        {
            if(1 != SDLNet_TCP_Send(sd, & ch, 1))
                setfail(true);
        }
    }

    void StreamNetwork::putBE16(u16 v)
    {
        put8(v >> 8);
        put8(v);
    }

    void StreamNetwork::putLE16(u16 v)
    {
        put8(v);
        put8(v >> 8);
    }

    void StreamNetwork::putBE32(u32 v)
    {
        putBE16(v >> 16);
        putBE16(v & 0xFFFF);
    }

    void StreamNetwork::putLE32(u32 v)
    {
        putLE16(v & 0xFFFF);
        putLE16(v >> 16);
    }

    void StreamNetwork::putBE64(u64 v)
    {
        putBE32(v >> 32);
        putBE32(v & 0xFFFFFFFF);
    }

    void StreamNetwork::putLE64(u64 v)
    {
        putLE32(v & 0xFFFFFFFF);
        putLE32(v >> 32);
    }

    void StreamNetwork::put(const char* data, size_t sz)
    {
        send(data, sz);
    }
}
#endif
