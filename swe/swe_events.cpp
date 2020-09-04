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

#include <list>
#include <algorithm>
#include <functional>

#include "swe_systems.h"
#include "swe_events.h"

namespace SWE
{
    struct SignalChain
    {
        int			code;
        const SignalMember*	sender;
        SignalMember*		receiver;

        SignalChain() : code(0), sender(nullptr), receiver(nullptr) {}
        SignalChain(const SignalMember & sm1, int sig, SignalMember & sm2) : code(sig), sender(& sm1), receiver(& sm2) {}

        bool isMember(const SignalMember & sm) const
        {
            return sender == &sm || receiver == &sm;
        }
    };

    std::list<SignalChain> signalChains;


    void clearAllSignals(void)
    {
        signalChains.clear();
    }
}

SWE::SignalMember::~SignalMember()
{
    signalUnsubscribe(*this);
}

void SWE::SignalMember::signalSubscribe(const SignalMember & sender, int sig, SignalMember & receiver)
{
    signalChains.push_back(SignalChain(sender, sig, receiver));
}

bool SWE::SignalMember::signalSubscribed(const SignalMember & sender, int sig)
{
    return std::any_of(signalChains.begin(), signalChains.end(),
	    [&](const SignalChain & memb) { return memb.sender == & sender && (sig == 0 || memb.code == sig); });
}

void SWE::SignalMember::signalSubscribe(const SignalMember & sender, int sig)
{
    signalChains.push_back(SignalChain(sender, sig, *this));
}

void SWE::SignalMember::signalUnsubscribe(const SignalMember & sender)
{
    signalChains.remove_if([&](const SignalChain & memb){ return memb.isMember(sender); });
}

void SWE::SignalMember::signalEmit(int sig)
{
    for(auto & memb : signalChains)
    {
        if(memb.sender == this && memb.code == sig)
            memb.receiver->signalReceive(sig, memb.sender);
    }
}

void SWE::SignalMember::signalEmit(const SignalMember & sender, int sig)
{
    for(auto & memb : signalChains)
    {
	if(memb.sender == & sender && memb.code == sig)
            memb.receiver->signalReceive(sig, memb.sender);
    }
}
