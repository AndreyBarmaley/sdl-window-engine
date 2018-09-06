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

#ifndef _SWE_SHAREDMAP_
#define _SWE_SHAREDMAP_

#include <map>
#include <unordered_map>
#include <iterator>
#include <memory>

class StreamBase;

template<typename T1, typename T2>
class SharedMap
{
    typedef std::unordered_map<T1, T2> basetype_t;
    std::shared_ptr<basetype_t> ptr;

    friend StreamBase & operator<< (StreamBase & sb, const SharedMap<T1, T2> & sv)
    {
	return sb << (*sv.ptr);
    }

    friend StreamBase & operator>> (StreamBase & sb, SharedMap<T1, T2> & sv)
    {
	return sb >> (*sv.ptr);
    }

public:
    typedef typename basetype_t::iterator iterator;
    typedef typename basetype_t::const_iterator const_iterator;

    SharedMap() : ptr(std::make_shared<basetype_t>()) {}
    SharedMap(const std::map<T1, T2> & v) : ptr(std::make_shared<basetype_t>(v.begin(), v.end())) {}
    SharedMap(const std::unordered_map<T1, T2> & v) : ptr(std::make_shared<basetype_t>(v.begin(), v.end())) {}

    size_t              size(void) const { return ptr->size(); }
    bool                empty(void) const { return ptr->empty(); }
    bool		unique(void) const { return ptr.unique(); }

    void                reset(void) { ptr = std::make_shared<basetype_t>(); }
    void                clear(void) { ptr->clear(); }
    void                reserve(size_t sz) { if(sz) ptr->reserve(sz); }
    void		swap(SharedMap & v) { ptr.swap(v.ptr); }

    template<class InputIterator>
    void		insert(InputIterator first, InputIterator last) { ptr->insert(first, last); }
    void		insert(const T1 & key, const T2 & val) { ptr->insert(std::make_pair(key, val)); }

    iterator            find(const T1 & key) { return ptr->find(key); }
    const_iterator      find(const T1 & key) const { return ptr->find(key); }

    const_iterator      begin(void) const { return ptr->begin(); }
    const_iterator      end(void) const { return ptr->end(); }

    iterator            begin(void) { return ptr->begin(); }
    iterator            end(void) { return ptr->end(); }

    T2 &                at(const T1 & key) { return ptr->at(key); }
    const T2 &          at(const T1 & key) const { return ptr->at(key); }

    T2 &                operator[] (const T1 & key) { return ptr->operator[](key); }
    const T2 &          operator[] (const T1 & key) const { return ptr->operator[](key); }

    SharedMap<T1, T2> &	operator= (const std::map<T1, T2> & v) { ptr->clear(); ptr->insert(v.begin(), v.end()); return *this; }
    SharedMap<T1, T2> &	operator= (const std::unordered_map<T1, T2> & v) { ptr->clear(); ptr->insert(v.begin(), v.end()); return *this; }
    SharedMap<T1, T2>	copy(void) const { return SharedMap(begin(), end()); }
    std::map<T1, T2>	toMap(void) const { return *ptr; }
};

#endif
