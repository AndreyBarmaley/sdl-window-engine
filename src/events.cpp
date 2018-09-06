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

#include "systems.h"
#include "events.h"

struct SignalChain
{
    int			code;
    const SignalMember*	sender;
    SignalMember*	receiver;

    SignalChain() : code(0), sender(NULL), receiver(NULL) {}
    SignalChain(const SignalMember & sm1, int sig, SignalMember & sm2) : code(sig), sender(& sm1), receiver(& sm2) {}

    bool isMember(const SignalMember & sm) const
    {
	return sender == &sm || receiver == &sm;
    }
};

namespace
{
    std::list<SignalChain> signalChains;
}

SignalMember::~SignalMember()
{
    signalUnsubscribe(*this);
}

void SignalMember::signalSubscribe(const SignalMember & sender, int sig, SignalMember & receiver)
{
    signalChains.push_back(SignalChain(sender, sig, receiver));
}

bool SignalMember::signalSubscribed(const SignalMember & sender, int sig)
{
    for(auto it = signalChains.begin(); it != signalChains.end(); ++it)
	if((*it).sender == & sender && (sig == 0 || (*it).code == sig)) return true;
    return false;
}

void SignalMember::signalSubscribe(const SignalMember & sender, int sig)
{
    signalChains.push_back(SignalChain(sender, sig, *this));
}

void SignalMember::signalUnsubscribe(const SignalMember & sender)
{
    signalChains.remove_if(std::bind2nd(std::mem_fun_ref(&SignalChain::isMember), sender));
}

void SignalMember::signalEmit(int sig)
{
    for(auto it = signalChains.begin(); it != signalChains.end(); ++it)
	if((*it).sender == this && (*it).code == sig)
	    (*it).receiver->signalReceive(sig, (*it).sender);
}

void SignalMember::signalEmit(const SignalMember & sender, int sig)
{
    for(auto it = signalChains.begin(); it != signalChains.end(); ++it)
	if((*it).sender == & sender && (*it).code == sig)
	    (*it).receiver->signalReceive(sig, (*it).sender);
}

namespace Engine
{
    void clearAllSignals(void)
    {
	signalChains.clear();
    }
}
